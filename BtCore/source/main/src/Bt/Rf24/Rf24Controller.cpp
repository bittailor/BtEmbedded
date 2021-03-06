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

#include "Bt/Rf24/Rf24Controller.hpp"

#include <stdio.h>
#include <iostream>
#include <typeinfo>
#include <sstream>
#include <memory>
#include <numeric>

#include <boost/lexical_cast.hpp>
#include <Bt/Log/Logging.hpp>
#include <Bt/Util/Timing.hpp>
#include <Bt/Util/Timeout.hpp>

namespace Bt {
namespace Rf24 {

//-------------------------------------------------------------------------------------------------

Rf24DeviceController::Rf24DeviceController(I_Rf24Device& iDevice, Mcu::I_InterruptPin& iInterruptPin, uint8_t iChannel,
                                           Concurrency::I_ExecutionContext& iExecutionContext)
: mDevice(iDevice), mInterruptPin(iInterruptPin), mChannel(iChannel), mExecutionContext(iExecutionContext) , mPowerDown(*this),
  mStandbyI(*this), mRxMode(*this), mTxMode(*this), mCurrentState(&mPowerDown),
  mInterruptState(InterruptState::Ignore), mEmptyReceiveCounter(0){
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
   size_t size = transmitPacket(iPipe, iPacket);
   return size != 0;
}

//-------------------------------------------------------------------------------------------------

size_t Rf24DeviceController::transmitPacket(RfPipe iPipe, Packet& iPacket) {
   StateBase* originalState = mCurrentState;
   BT_LOG(DEBUG) << "...send payload of size " << iPacket.size() << " with " << std::accumulate(iPacket.buffer(), iPacket.buffer() + iPacket.size(), std::string() , [](std::string s, uint8_t c) -> std::string {
      std::string result = s +" "+ boost::lexical_cast<std::string>(static_cast<int>(c));
      if (0x20 <= c && c <= 0x7E) {
         result = result + "["+ static_cast<char>(c) +"]";
      }
      return result;
   });

   BT_LOG(DEBUG) << "transceiverMode " << mDevice.transceiverMode() ;
   BT_LOG(DEBUG) << "write current state is " << typeid(*mCurrentState).name() ;
   mCurrentState->ToStandbyI();

   RfAddress backupPipe0 = mDevice.receiveAddress(RfPipe::PIPE_0);
   RfAddress transmitPipeAddress = mDevice.receiveAddress(iPipe);
   mDevice.transmitAddress(transmitPipeAddress);
   mDevice.receiveAddress(RfPipe::PIPE_0, transmitPipeAddress);

   mDevice.writeTransmitPayload(iPacket.buffer(), iPacket.size());

   while(mDevice.isTransmitFifoEmpty()) {
      BT_LOG(WARNING) << "transmit FIFO empty after sending payload ==> try again " ;
      mDevice.writeTransmitPayload(iPacket.buffer(), iPacket.size());
   }

   if(!mInterruptPin.read()) {
      BT_LOG(ERROR) << "IRQ already set before transmit - status " << mDevice.status();
   }

   mCurrentState->ToTxMode();
   Util::Timeout timeout(100); // MaxRetry [15] * ( MaxRetryDelay [4ms] + MaxTrasnmittionTime [0.5ms]) => ~100ms

   while(mInterruptPin.read() && !timeout.check() ) {
      BT_LOG(DEBUG) << "retransmit counter " << (int)mDevice.autoRetransmitCounter();
      Util::delayInMicroseconds(1);
   }

   mCurrentState->ToStandbyI();

   I_Rf24Device::Status status = mDevice.status();
   BT_LOG(DEBUG) << "status after IRQ " << status;

   size_t sentSize = iPacket.size();
   bool flushTransmitFifo = false;

   if (status.retransmitsExceeded()) {
      mDevice.clearRetransmitsExceeded();
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
      mDevice.clearDataSent();
   }

   if(!mInterruptPin.read()) {
      BT_LOG(ERROR) << "IRQ still set after transmit - status =  " << mDevice.status();
   }

   if (flushTransmitFifo) {
      mDevice.flushTransmitFifo();
   }

   mDevice.receiveAddress(RfPipe::PIPE_0, backupPipe0);

   originalState->ApplyTo(*mCurrentState);

   return sentSize;
}


//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::startListening(std::function<void(RfPipe iPipe, Packet& iPacket)> iCallback) {
   mCallback = iCallback;
   mCurrentState->ToRxMode();
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::stopListening() {
   mCurrentState->ToStandbyI();
   mCallback = std::function<void(RfPipe iPipe, Packet& iPacket)>();
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::reboot() {
   StateBase* originalState = mCurrentState;
   mCurrentState->ToPowerDown();
   Util::delayInMicroseconds(100);
   originalState->ApplyTo(*mCurrentState);
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::configureDevice() {

   BT_LOG(INFO) << "Rf24DeviceController::configureDevice - channel is " << static_cast<int>(mChannel) ;

   mDevice.dynamicPayloadFeatureEnabled(true);
   mDevice.autoRetransmitDelay(0x15);
   mDevice.autoRetransmitCount(0xf);
   mDevice.channel(mChannel);
   mDevice.dataRate(I_Rf24Device::DR_2_MBPS);


   for (auto pipe : RfPipes::ALL_PIPES) {
      auto pipeConfiguration = mConfiguration[pipe];
      if(pipeConfiguration.mEnabled) {
         mDevice.receivePayloadSize(pipe, I_Rf24Device::MAX_PAYLOAD_SIZE);
         mDevice.receivePipeEnabled(pipe, true);
         mDevice.dynamicPayloadEnabled(pipe, true);
         mDevice.receiveAddress(pipe,pipeConfiguration.mAddress);
      } else {
         mDevice.receivePipeEnabled(pipe, false);
      }
   }
   
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::onInterrupt() {
   BT_LOG(DEBUG) << "onInterrupt - InterruptState = " << static_cast<int>(mInterruptState.load());
   switch(mInterruptState.load()) {
      case InterruptState::Ignore : {
         BT_LOG(WARNING) << "IRQ in InterruptState::Ignore";
         return;
      }
      case InterruptState::Rx     : {
         mExecutionContext.invoke(std::bind(&Rf24DeviceController::readReceiveData,this));
         return;
      }
      case InterruptState::Tx     : {
         return;
      }
   }
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::readReceiveData() {
   std::size_t receivedCounter = 0;
   while(!mDevice.isReceiveFifoEmpty()) {
      BT_LOG(DEBUG) << "receive payload ...";
      RfPipe pipe;
      Packet packet;
      size_t size = mDevice.readReceivePayload(pipe,packet.buffer(),Packet::CAPACITY);
      if (size <= 0) {
         BT_LOG(ERROR) << "invalid read size of " << size << " => drop packet" ;
      } else {
         packet.size(size);
         receivedCounter++;
         BT_LOG(DEBUG) << "... payload received of size " << size << " with " << std::accumulate(packet.buffer(), packet.buffer() + size, std::string() , [](std::string s, uint8_t c) -> std::string {
            std::string result = s +" "+ boost::lexical_cast<std::string>(static_cast<int>(c));
            if (0x20 <= c && c <= 0x7E) {
               result = result + "["+ static_cast<char>(c) +"]";
            }
            return result;
         });
         std::function<void()> handle = std::bind(&Rf24DeviceController::handleReceiveData,this,pipe,packet);
         mExecutionContext.invoke(handle);
      }
      mDevice.clearDataReady();
   }
   if(receivedCounter < 1) {
      mEmptyReceiveCounter++;
   } else {
      mEmptyReceiveCounter = 0;
   }

   if(mEmptyReceiveCounter > 10) { // TODO currently just a hard coded fallback after 5 minutes to work around nRf24 device driver issues.
      mEmptyReceiveCounter = 0;
      BT_LOG(WARNING) << "more that 10 empty receive cycles => reset the nrf24 device";
      reboot();
   }
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::handleReceiveData(RfPipe iPipe, Packet& iPacket) {
   if(mCallback) {
      mCallback(iPipe,iPacket);
   }
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::PowerDown::ToStandbyI() {

   mController->mDevice.flushReceiveFifo();
   mController->mDevice.flushTransmitFifo();
   mController->configureDevice();
   mController->mInterruptPin.enable(std::bind(&Rf24DeviceController::onInterrupt,mController));
   mController->mDevice.powerUp(true);
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
   mController->mDevice.powerUp(false);
   mController->mInterruptPin.disable();
   mController->mCurrentState = &mController->mPowerDown;
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::StandbyI::ToRxMode() {
   if(!mController->mInterruptPin.read()) {
      BT_LOG(WARNING) << "IRQ already set before StandbyI::ToRxMode transition - status =  " << mController->mDevice.status();
   }

   mController->mDevice.clearDataReady();
   mController->mDevice.clearDataSent();
   mController->mDevice.clearRetransmitsExceeded();

   if(!mController->mInterruptPin.read()) {
      BT_LOG(ERROR) << "IRQ still set even after a clear before StandbyI::ToRxMode transition - status =  " << mController->mDevice.status();
   }

   mController->mInterruptState.store(InterruptState::Rx);

   mController->mDevice.transceiverMode(I_Rf24Device::RX_MODE);
   mController->mDevice.chipEnable(true);
   Util::delayInMicroseconds(130);
   mController->mCurrentState = &mController->mRxMode;
   BT_LOG(DEBUG) << "RxMode";
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::StandbyI::ToTxMode() {
   if (mController->mDevice.isTransmitFifoEmpty())
   {
      BT_LOG(WARNING) << "StandbyI::ToTxMode: transmit fifo is empty => StandbyI !";
      return;
   }

   if(!mController->mInterruptPin.read()){
      BT_LOG(WARNING) << "StandbyI::ToTxMode: InterruptPin still set => clear all !";
      mController->mDevice.clearDataReady();
      mController->mDevice.clearDataSent();
      mController->mDevice.clearRetransmitsExceeded();
   }

   mController->mInterruptState.store(InterruptState::Tx);
   mController->mDevice.transceiverMode(I_Rf24Device::TX_MODE);
   mController->mDevice.chipEnable(true);
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
   mController->mDevice.chipEnable(false);
   mController->mDevice.transceiverMode(I_Rf24Device::TX_MODE);

   mController->mInterruptState.store(InterruptState::Ignore);
   if(!mController->mInterruptPin.read()){
      mController->readReceiveData();
      mController->mDevice.clearDataReady();
      mController->mDevice.clearDataSent();
      mController->mDevice.clearRetransmitsExceeded();
   }

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
   mController->mDevice.chipEnable(false);
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

