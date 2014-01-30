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

Rf24Controller::Rf24Controller(I_Rf24Device& pDevice)
: mDevice(&pDevice), mPowerDown(*this), mStandbyI(*this), mRxMode(*this), mTxMode(*this), mCurrentState(&mPowerDown) {
}

//-------------------------------------------------------------------------------------------------

Rf24Controller::~Rf24Controller() {

}

//-------------------------------------------------------------------------------------------------

void Rf24Controller::configure(const Configuration& pConfiguration) {

   StateBase* originalState = mCurrentState;
   mCurrentState->ToPowerDown();
   mConfiguration = pConfiguration;
   originalState->ApplyTo(*mCurrentState);
}

//-------------------------------------------------------------------------------------------------

bool Rf24Controller::write(RfPipe pPipe, Packet& pPacket) {
   size_t size = write(pPipe, pPacket.buffer(), pPacket.size());
   return size != 0;
}

//-------------------------------------------------------------------------------------------------

size_t Rf24Controller::write(RfPipe pPipe, uint8_t* pData, size_t pSize) {

   StateBase* originalState = mCurrentState;
   mCurrentState->ToStandbyI();

   RfAddress backupPipe0 = mDevice->receiveAddress(RfPipe::PIPE_0);
   RfAddress transmitPipeAddress = mDevice->receiveAddress(pPipe);
   mDevice->transmitAddress(transmitPipeAddress);
   mDevice->receiveAddress(RfPipe::PIPE_0, transmitPipeAddress);

   mDevice->writeTransmitPayload(pData, pSize);

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
      printf("TxMode::ToStandbyI: send failed retransmits exceeded ! \n");
      sentSize = 0;
   }


   if (timeout.isTimedOut()) {
      mDevice->clearRetransmitsExceeded();
      mDevice->flushTransmitFifo();
      printf("TxMode::ToStandbyI: send failed timeout ! \n");
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

void Rf24Controller::startListening() {
   mCurrentState->ToRxMode();
}

//-------------------------------------------------------------------------------------------------

void Rf24Controller::stopListening() {
   mCurrentState->ToStandbyI();
}

//-------------------------------------------------------------------------------------------------

bool Rf24Controller::isDataAvailable() {
   return mDevice->status().dataReady();
}

//-------------------------------------------------------------------------------------------------

bool Rf24Controller::read(Packet& pPacket) {
   RfPipe pipe;
   return read(pPacket, pipe);
}

//-------------------------------------------------------------------------------------------------

bool Rf24Controller::read(Packet& pPacket, RfPipe& pPipe) {
   if (mDevice->isReceiveFifoEmpty())
   {
      return false;
   }
   size_t size = mDevice->readReceivePayload(pPipe, pPacket.buffer(), Packet::BUFFER_CAPACITY);
   pPacket.size(size);
   if(mDevice->isReceiveFifoEmpty()) {
      mDevice->clearDataReady();
   }
   return true;
}

//-------------------------------------------------------------------------------------------------

size_t Rf24Controller::read(uint8_t* pBuffer, size_t pSize)
{
   RfPipe pipe;
   return read(pBuffer, pSize, pipe);
}

//-------------------------------------------------------------------------------------------------

size_t Rf24Controller::read(uint8_t* pData, size_t pSize, RfPipe& pPipe) {
   if (mDevice->isReceiveFifoEmpty())
   {
      return 0;
   }
   return mDevice->readReceivePayload(pPipe, pData, pSize);
}

//-------------------------------------------------------------------------------------------------

void Rf24Controller::configureDevice() {

   mDevice->dynamicPayloadFeatureEnabled(true);
   mDevice->autoRetransmitDelay(0x5);
   mDevice->autoRetransmitCount(0xf);

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

//   for (auto pipe : RfPipes::ALL_PIPES) {
//         mDevice->receivePayloadSize(pipe, I_Rf24Device::MAX_PAYLOAD_SIZE);
//         mDevice->receivePipeEnabled(pipe, true);
//         mDevice->dynamicPayloadEnabled(pipe, true);
//   }
   
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void Rf24Controller::PowerDown::ToStandbyI() {
   mController->mDevice->powerUp(true);
   // Util::delayInMicroseconds(1500); // TODO (BT) can reduce ext oszi ?
   Util::delayInMicroseconds(150); // TODO (BT) can reduce ext oszi ?

   mController->configureDevice();

   mController->mCurrentState = &mController->mStandbyI;
}

//-------------------------------------------------------------------------------------------------

void Rf24Controller::PowerDown::ToRxMode() {
   ToStandbyI();
   mController->mCurrentState->ToRxMode();
}

//-------------------------------------------------------------------------------------------------

void Rf24Controller::PowerDown::ToTxMode() {
   ToStandbyI();
   mController->mCurrentState->ToTxMode();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void Rf24Controller::StandbyI::ToPowerDown() {
   mController->mDevice->powerUp(false);
   mController->mCurrentState = &mController->mPowerDown;
}

//-------------------------------------------------------------------------------------------------

void Rf24Controller::StandbyI::ToRxMode() {
   mController->mDevice->clearDataReady();
   mController->mDevice->clearDataSent();
   mController->mDevice->clearRetransmitsExceeded();

   mController->mDevice->transceiverMode(I_Rf24Device::RX_MODE);
   mController->mDevice->chipEnable(true);
   Util::delayInMicroseconds(130);
   mController->mCurrentState = &mController->mRxMode;
}

//-------------------------------------------------------------------------------------------------

void Rf24Controller::StandbyI::ToTxMode() {
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

void Rf24Controller::RxMode::ToPowerDown() {
   ToStandbyI();
   mController->mCurrentState->ToPowerDown();
}


//-------------------------------------------------------------------------------------------------

void Rf24Controller::RxMode::ToStandbyI() {
   mController->mDevice->chipEnable(false);
   mController->mDevice->transceiverMode(I_Rf24Device::TX_MODE);
   Util::delayInMicroseconds(10);
   mController->mCurrentState = &mController->mStandbyI;
}

//-------------------------------------------------------------------------------------------------

void Rf24Controller::RxMode::ToTxMode() {
   ToStandbyI();
   mController->mCurrentState->ToTxMode();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void Rf24Controller::TxMode::ToPowerDown() {
   ToStandbyI();
   mController->mCurrentState->ToPowerDown();
}

//-------------------------------------------------------------------------------------------------

void Rf24Controller::TxMode::ToStandbyI() {
   mController->mDevice->chipEnable(false);
   Util::delayInMicroseconds(10);
   mController->mCurrentState = &mController->mStandbyI;
}

//-------------------------------------------------------------------------------------------------

void Rf24Controller::TxMode::ToRxMode() {
   ToStandbyI();
   mController->mCurrentState->ToRxMode();
}

//-------------------------------------------------------------------------------------------------

} // namespace Device
} // namespace Bt

