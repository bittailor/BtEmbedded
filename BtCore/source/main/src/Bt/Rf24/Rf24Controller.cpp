//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::Rf24Controller
//  
//*************************************************************************************************

#include <stdio.h>
#include <iostream>
#include <typeinfo>
#include <sstream>

#include <Bt/Log/Logging.hpp>

#include "Bt/Rf24/Rf24Controller.hpp"

#include "Bt/Util/Timing.hpp"
#include "Bt/Util/Timeout.hpp"

namespace Bt {
namespace Rf24 {

//-------------------------------------------------------------------------------------------------

Rf24DeviceController::Rf24DeviceController(I_Rf24Device& iDevice)
: mDevice(&iDevice), mInterruptPin(24, Mcu::I_InterruptPin::Edge::FALLING) , mPowerDown(*this), mStandbyI(*this), mRxMode(*this), mTxMode(*this),
  mCurrentState(&mPowerDown), mLogTimer(Bt::Util::milliseconds() + 1000){
}

//-------------------------------------------------------------------------------------------------

Rf24DeviceController::~Rf24DeviceController() {

}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::configure(const Configuration& iConfiguration) {

   StateBase* originalState = mCurrentState;
   mCurrentState->ToPowerDown();
   mConfiguration = iConfiguration;
   originalState->ApplyTo(*mCurrentState);
}

//-------------------------------------------------------------------------------------------------

bool Rf24DeviceController::write(RfPipe iPipe, Packet& iPacket) {
   size_t size = write(iPipe, iPacket.buffer(), iPacket.size());
   return size != 0;
}

//-------------------------------------------------------------------------------------------------

size_t Rf24DeviceController::write(RfPipe iPipe, uint8_t* iData, size_t iSize) {
   StateBase* originalState = mCurrentState;
   BT_LOG(DEBUG) << "transceiverMode " << mDevice->transceiverMode() ;
   BT_LOG(DEBUG) << "write current state is " << typeid(*mCurrentState).name() ;
   mCurrentState->ToStandbyI();

   RfAddress backupPipe0 = mDevice->receiveAddress(RfPipe::PIPE_0);
   RfAddress transmitPipeAddress = mDevice->receiveAddress(iPipe);
   mDevice->transmitAddress(transmitPipeAddress);
   mDevice->receiveAddress(RfPipe::PIPE_0, transmitPipeAddress);

   mDevice->writeTransmitPayload(iData, iSize);

   while(mDevice->isTransmitFifoEmpty()) {
      BT_LOG(WARNING) << "transmit FIFO empty after sending payload ==> try again " ;
      mDevice->writeTransmitPayload(iData, iSize);
   }

   if(!mInterruptPin.read()) {
      BT_LOG(ERROR) << " **** IRQ already set before transmit - status " << mDevice->status();
   }

   mCurrentState->ToTxMode();
   Util::Timeout timeout(100); // MaxRetry [15] * ( MaxRetryDelay [4ms] + MaxTrasnmittionTime [0.5ms]) => ~100ms

   while(mInterruptPin.read() && !timeout.check() ) {
      BT_LOG(DEBUG) << "retransmit counter " << (int)mDevice->autoRetransmitCounter();
      Util::delayInMicroseconds(1);
   }

   I_Rf24Device::Status status = mDevice->status();
   BT_LOG(DEBUG) << "status after IRQ " << status;

   size_t sentSize = iSize;
   bool flushTransmitFifo = false;

   if (status.retransmitsExceeded()) {
      mDevice->clearRetransmitsExceeded();
      flushTransmitFifo = true;
      BT_LOG(WARNING) << "write: send failed retransmits exceeded!";
      sentSize = 0;
   }

   if (timeout.lastChekTimedOut()) {
      flushTransmitFifo = true;
      BT_LOG(WARNING) << "write: send failed timeout!";
      sentSize = 0;
   }

   if (status.dataSent()) {
      mDevice->clearDataSent();
   }

   mCurrentState->ToStandbyI();

   if(!mInterruptPin.read()) {
      BT_LOG(ERROR) << " **** IRQ still set after clean - status =  " << mDevice->status();
   }


   if (flushTransmitFifo) {
      mDevice->flushTransmitFifo();
   }

   mDevice->receiveAddress(RfPipe::PIPE_0, backupPipe0);

   originalState->ApplyTo(*mCurrentState);

   return sentSize;
}


//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::startListening() {
   mCurrentState->ToRxMode();
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::stopListening() {
   mCurrentState->ToStandbyI();
}

//-------------------------------------------------------------------------------------------------

bool Rf24DeviceController::isDataAvailable() {
//   if (mLogTimer < Bt::Util::milliseconds()) {
//      BT_LOG(DEBUG) << "isDataAvailable: transceiverMode " << mDevice->transceiverMode() ;
//      BT_LOG(DEBUG) << "isDataAvailable: current state is " << typeid(*mCurrentState).name() ;
//      mLogTimer = Bt::Util::milliseconds() + 1000;
//   }


   if (mDevice->status().receiveFifoEmpty()) {
      return false;
   }

   if (mDevice->availableReceivePayload() > MAX_PAYLOAD_SIZE) {
      mDevice->flushReceiveFifo();
      return false;
   }

   return true;
}

//-------------------------------------------------------------------------------------------------

bool Rf24DeviceController::read(Packet& oPacket) {
   RfPipe pipe;
   return read(oPacket, pipe);
}

//-------------------------------------------------------------------------------------------------

bool Rf24DeviceController::read(Packet& oPacket, RfPipe& oPipe) {
   if (mDevice->isReceiveFifoEmpty())
   {
      return false;
   }
   size_t size = mDevice->readReceivePayload(oPipe, oPacket.buffer(), Packet::BUFFER_CAPACITY);
   oPacket.size(size);

   std::stringstream message;
   for (size_t i = 0 ; i < 3 ; i++) {
      message << (int)oPacket.buffer()[i];
      message << ",";
   }

   for (size_t i = 0 ; i < oPacket.size() ; i++) {
      if(isprint((char) oPacket.buffer()[i])) {
         message << (char) oPacket.buffer()[i];
      } else {
         message << (int)oPacket.buffer()[i];
      }
      message << ",";
   }
   message << " end";


   BT_LOG(DEBUG) << "read: " << message.str();


   if(mDevice->isReceiveFifoEmpty()) {
      mDevice->clearDataReady();
   }
   return true;
}

//-------------------------------------------------------------------------------------------------

size_t Rf24DeviceController::read(uint8_t* oBuffer, size_t iSize)
{
   RfPipe pipe;
   return read(oBuffer, iSize, pipe);
}

//-------------------------------------------------------------------------------------------------

size_t Rf24DeviceController::read(uint8_t* oData, size_t iSize, RfPipe& oPipe) {
   if (mDevice->isReceiveFifoEmpty())
   {
      return 0;
   }
   return mDevice->readReceivePayload(oPipe, oData, iSize);
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::configureDevice() {

   mDevice->dynamicPayloadFeatureEnabled(true);
   mDevice->autoRetransmitDelay(0x15);
   mDevice->autoRetransmitCount(0xf);
   mDevice->channel(10);
   mDevice->dataRate(I_Rf24Device::DR_1_MBPS);


   for (auto pipe : RfPipes::ALL_PIPES) {
      auto pipeConfiguration = mConfiguration[pipe];
      if(pipeConfiguration.mEnabled) {
         mDevice->receivePayloadSize(pipe, I_Rf24Device::MAX_PAYLOAD_SIZE);
         mDevice->receivePipeEnabled(pipe, true);
         mDevice->dynamicPayloadEnabled(pipe, true);
         mDevice->receiveAddress(pipe,pipeConfiguration.mAddress);
      } else {
         mDevice->receivePipeEnabled(pipe, false);
      }
   }
   
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::onInterrupt() {
   BT_LOG(INFO) << " --- onInterrupt ----";
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::PowerDown::ToStandbyI() {

   mController->mDevice->flushReceiveFifo();
   mController->mDevice->flushTransmitFifo();
   mController->configureDevice();
   mController->mInterruptPin.enable(std::bind(&Rf24DeviceController::onInterrupt,mController));
   mController->mDevice->powerUp(true);
   Util::delayInMicroseconds(150);

   mController->mCurrentState = &mController->mStandbyI;
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::PowerDown::ToRxMode() {
   ToStandbyI();
   mController->mCurrentState->ToRxMode();
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::PowerDown::ToTxMode() {
   ToStandbyI();
   mController->mCurrentState->ToTxMode();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::StandbyI::ToPowerDown() {
   mController->mDevice->powerUp(false);
   mController->mInterruptPin.disable();
   mController->mCurrentState = &mController->mPowerDown;
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::StandbyI::ToRxMode() {
   mController->mDevice->clearDataReady();
   mController->mDevice->clearDataSent();
   mController->mDevice->clearRetransmitsExceeded();

   mController->mDevice->transceiverMode(I_Rf24Device::RX_MODE);
   mController->mDevice->chipEnable(true);
   Util::delayInMicroseconds(130);
   mController->mCurrentState = &mController->mRxMode;
   BT_LOG(DEBUG) << "RxMode";
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::StandbyI::ToTxMode() {
   if (mController->mDevice->isTransmitFifoEmpty())
   {
      BT_LOG(WARNING) << "StandbyI::ToTxMode: transmit fifo is empty => StandbyI !";
      return;
   }

   mController->mDevice->clearDataReady();
   mController->mDevice->clearDataSent();
   mController->mDevice->clearRetransmitsExceeded();

   mController->mDevice->transceiverMode(I_Rf24Device::TX_MODE);
   mController->mDevice->chipEnable(true);
   Util::delayInMicroseconds(140);
   mController->mCurrentState = &mController->mTxMode;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::RxMode::ToPowerDown() {
   ToStandbyI();
   mController->mCurrentState->ToPowerDown();
}


//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::RxMode::ToStandbyI() {
   mController->mDevice->chipEnable(false);
   mController->mDevice->transceiverMode(I_Rf24Device::TX_MODE);
   Util::delayInMicroseconds(10);
   mController->mCurrentState = &mController->mStandbyI;
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::RxMode::ToTxMode() {
   ToStandbyI();
   mController->mCurrentState->ToTxMode();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::TxMode::ToPowerDown() {
   ToStandbyI();
   mController->mCurrentState->ToPowerDown();
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::TxMode::ToStandbyI() {
   mController->mDevice->chipEnable(false);
   Util::delayInMicroseconds(10);
   mController->mCurrentState = &mController->mStandbyI;
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::TxMode::ToRxMode() {
   ToStandbyI();
   mController->mCurrentState->ToRxMode();
}

//-------------------------------------------------------------------------------------------------

} // namespace Device
} // namespace Bt

