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

#include "Bt/Rf24/Rf24Controller.hpp"

#include "Bt/Util/Timing.hpp"
#include "Bt/Util/Timeout.hpp"

namespace Bt {
namespace Rf24 {

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

namespace{

template<typename T>
void printArray(const T& pArray)
{
  printf(" 0x");
  for(size_t i = pArray.size() ; i > 0 ; i--)
  {
    printf("%02x",pArray[i-1]);
  }
}

}

//-------------------------------------------------------------------------------------------------

Rf24DeviceController::Rf24DeviceController(I_Rf24Device& pDevice)
: mDevice(&pDevice), mPowerDown(*this), mStandbyI(*this), mRxMode(*this), mTxMode(*this), mCurrentState(&mPowerDown), mLogTimer(Bt::Util::milliseconds() + 1000) {
}

//-------------------------------------------------------------------------------------------------

Rf24DeviceController::~Rf24DeviceController() {

}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::configure(const Configuration& pConfiguration) {

   StateBase* originalState = mCurrentState;
   mCurrentState->ToPowerDown();
   mConfiguration = pConfiguration;
   originalState->ApplyTo(*mCurrentState);
}

//-------------------------------------------------------------------------------------------------

bool Rf24DeviceController::write(RfPipe pPipe, Packet& pPacket) {
   size_t size = write(pPipe, pPacket.buffer(), pPacket.size());
   return size != 0;
}

//-------------------------------------------------------------------------------------------------

size_t Rf24DeviceController::write(RfPipe pPipe, uint8_t* pData, size_t pSize) {

   StateBase* originalState = mCurrentState;
   std::cout << "transceiverMode " << mDevice->transceiverMode() << std::endl;
   std::cout << "write current state is " << typeid(*mCurrentState).name() << std::endl;
   mCurrentState->ToStandbyI();

   RfAddress backupPipe0 = mDevice->receiveAddress(RfPipe::PIPE_0);
   RfAddress transmitPipeAddress = mDevice->receiveAddress(pPipe);
   mDevice->transmitAddress(transmitPipeAddress);
   mDevice->receiveAddress(RfPipe::PIPE_0, transmitPipeAddress);

   mDevice->writeTransmitPayload(pData, pSize);

   while(mDevice->isTransmitFifoEmpty()) {
      std::cout << "transmit FIFO empty after sending payload ==> try again " << std::endl;
      mDevice->writeTransmitPayload(pData, pSize);
   }


   mCurrentState->ToTxMode();

   I_Rf24Device::Status status = mDevice->status();
   Util::Timeout timeout(10000);
   while(!(status.dataSent() || status.retransmitsExceeded() || !timeout.checkForTimeout() ) ) {
      status = mDevice->status();
      // printf("TxMode::ToStandbyI: autoRetransmitCounter = %u \n", (mDevice->autoRetransmitCounter()) );
      Util::delayInMicroseconds(10);
   }

   size_t sentSize = pSize;

   if (status.retransmitsExceeded()) {
      mDevice->clearRetransmitsExceeded();
      mDevice->flushTransmitFifo();
      printf("write: send failed retransmits exceeded ! \n");
      sentSize = 0;
   }


   if (timeout.isTimedOut()) {
      mDevice->clearRetransmitsExceeded();
      mDevice->flushTransmitFifo();
      printf("write: send failed timeout ! \n");
      sentSize = 0;
   }

   if (status.dataSent()) {
      mDevice->clearDataSent();
   }

   mDevice->receiveAddress(RfPipe::PIPE_0, backupPipe0);

   mCurrentState->ToStandbyI();
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
//      std::cout << "isDataAvailable: transceiverMode " << mDevice->transceiverMode() << std::endl;
//      std::cout << "isDataAvailable: current state is " << typeid(*mCurrentState).name() << std::endl;
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

bool Rf24DeviceController::read(Packet& pPacket) {
   RfPipe pipe;
   return read(pPacket, pipe);
}

//-------------------------------------------------------------------------------------------------

bool Rf24DeviceController::read(Packet& pPacket, RfPipe& pPipe) {
   if (mDevice->isReceiveFifoEmpty())
   {
      return false;
   }
   size_t size = mDevice->readReceivePayload(pPipe, pPacket.buffer(), Packet::BUFFER_CAPACITY);
   pPacket.size(size);

   std::cout << "read: ";
   for (size_t i = 0 ; i < 3 ; i++) {
      std::cout << (int)pPacket.buffer()[i];
      std::cout << ",";
   }

   for (size_t i = 0 ; i < pPacket.size() ; i++) {
      if(isprint((char) pPacket.buffer()[i])) {
         std::cout << (char) pPacket.buffer()[i];
      } else {
         std::cout << (int)pPacket.buffer()[i];
      }
      std::cout << ",";
   }
   std::cout <<" end";


   if(mDevice->isReceiveFifoEmpty()) {
      mDevice->clearDataReady();
   }
   return true;
}

//-------------------------------------------------------------------------------------------------

size_t Rf24DeviceController::read(uint8_t* pBuffer, size_t pSize)
{
   RfPipe pipe;
   return read(pBuffer, pSize, pipe);
}

//-------------------------------------------------------------------------------------------------

size_t Rf24DeviceController::read(uint8_t* pData, size_t pSize, RfPipe& pPipe) {
   if (mDevice->isReceiveFifoEmpty())
   {
      return 0;
   }
   return mDevice->readReceivePayload(pPipe, pData, pSize);
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::configureDevice() {

   mDevice->dynamicPayloadFeatureEnabled(true);
   mDevice->autoRetransmitDelay(0x6);
   mDevice->autoRetransmitCount(0xf);
   mDevice->channel(80);
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
//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::PowerDown::ToStandbyI() {

   mController->mDevice->flushReceiveFifo();
   mController->mDevice->flushTransmitFifo();
   mController->configureDevice();

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
}

//-------------------------------------------------------------------------------------------------

void Rf24DeviceController::StandbyI::ToTxMode() {
   if (mController->mDevice->isTransmitFifoEmpty())
   {
      printf("StandbyI::ToTxMode: transmit fifo is empty => StandbyI ! \n");
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

