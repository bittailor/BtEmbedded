//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::RfControllerNRf24L01P
//  
//*************************************************************************************************

#include <stdio.h>

#include "Bt/Device/RfControllerNRf24L01P.hpp"

#include "Bt/Util/Delay.hpp"
#include "Bt/Util/Timeout.hpp"

namespace Bt {
namespace Device {


//-------------------------------------------------------------------------------------------------

Rf24Controller::Rf24Controller(I_Rf24Device& pDevice)
: mDevice(&pDevice), mPowerDown(*this), mStandbyI(*this), mRxMode(*this), mTxMode(*this), mCurrentState(&mPowerDown) {

   mDevice->autoRetransmitDelay(0x5);
   //mDevice->autoRetransmitCount(0xf);
}

//-------------------------------------------------------------------------------------------------

Rf24Controller::~Rf24Controller() {

}

//-------------------------------------------------------------------------------------------------

size_t Rf24Controller::write(I_Rf24Device::Pipe pPipe, uint8_t* data, size_t size) {

   StateBase* originalState = mCurrentState;

   mCurrentState->ToStandbyI();
   mDevice->transmitAddress(mDevice->receiveAddress(pPipe));
   mDevice->writeTransmitPayload(data, size);
   mCurrentState->ToTxMode();




   mCurrentState->ToStandbyI();
   originalState->ApplyTo(*mCurrentState);

   return size;
}

//-------------------------------------------------------------------------------------------------

void Rf24Controller::startListening() {
   mCurrentState->ToRxMode();
}

//-------------------------------------------------------------------------------------------------

bool Rf24Controller::isDataAvailable() {
   return !mDevice->isReceiveFifoEmpty();
}

//-------------------------------------------------------------------------------------------------

size_t Rf24Controller::read(uint8_t* buffer, size_t size)
{
   I_Rf24Device::Pipe pipe;
   return read(buffer, size, pipe);
}

//-------------------------------------------------------------------------------------------------

size_t Rf24Controller::read(uint8_t* data, size_t size, I_Rf24Device::Pipe& pPipe) {
   if (mDevice->isReceiveFifoEmpty())
   {
      return 0;
   }
   return mDevice->readReceivePayload(pPipe, data, size);
}

//-------------------------------------------------------------------------------------------------

void Rf24Controller::PowerDown::ToStandbyI() {
   mController->mDevice->powerUp(true);
   Util::delayInMicroseconds(1500); // TODO (BT) can reduce ext oszi ?
   mController->mDevice->receivePayloadSize(I_Rf24Device::PIPE_0,32);
   mController->mDevice->receivePayloadSize(I_Rf24Device::PIPE_1,32);
   mController->mDevice->receivePayloadSize(I_Rf24Device::PIPE_2,32);
   mController->mDevice->receivePayloadSize(I_Rf24Device::PIPE_3,32);
   mController->mDevice->receivePayloadSize(I_Rf24Device::PIPE_4,32);
   mController->mDevice->receivePayloadSize(I_Rf24Device::PIPE_5,32);
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
   I_Rf24Device::Status status = mController->mDevice->status();
   Util::Timeout timeout(10000);
   while(!(status.dataSent() || status.retransmitsExceeded() || timeout) ) {
      status = mController->mDevice->status();
      printf("TxMode::ToStandbyI: autoRetransmitCounter = %u \n", (mController->mDevice->autoRetransmitCounter()) );
      Util::delayInMicroseconds(10);
   }

   if (timeout) {
      mController->mDevice->clearRetransmitsExceeded();
      printf("TxMode::ToStandbyI: send failed timeout ! \n");

   }

   if (status.dataSent()) {
      mController->mDevice->clearDataSent();
   }
   if (status.retransmitsExceeded()) {
         mController->mDevice->clearRetransmitsExceeded();
         printf("TxMode::ToStandbyI: send failed retransmits exceeded ! \n");

   }

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

