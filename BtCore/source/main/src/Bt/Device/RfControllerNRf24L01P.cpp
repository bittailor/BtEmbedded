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

namespace Bt {
namespace Device {


//-------------------------------------------------------------------------------------------------

RfControllerNRf24L01P::RfControllerNRf24L01P(I_DeviceNRf24L01P& pDevice)
: mDevice(&pDevice), mPowerDown(*this), mStandbyI(*this), mRxMode(*this), mTxMode(*this), mCurrentState(&mPowerDown) {

   mDevice->autoRetransmitDelay(0x5);
   mDevice->autoRetransmitCount(0xf);
}

//-------------------------------------------------------------------------------------------------

RfControllerNRf24L01P::~RfControllerNRf24L01P() {

}

//-------------------------------------------------------------------------------------------------

size_t RfControllerNRf24L01P::write(I_DeviceNRf24L01P::Pipe pPipe, uint8_t* data, size_t size) {

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

void RfControllerNRf24L01P::startListening() {
   mCurrentState->ToRxMode();
}

//-------------------------------------------------------------------------------------------------

bool RfControllerNRf24L01P::isDataAvailable() {
   return !mDevice->isReceiveFifoEmpty();
}

//-------------------------------------------------------------------------------------------------

size_t RfControllerNRf24L01P::read(I_DeviceNRf24L01P::Pipe& pPipe, uint8_t* data, size_t size) {
   if (mDevice->isReceiveFifoEmpty())
   {
      return 0;
   }
   return mDevice->readReceivePayload(pPipe, data, size);
}

//-------------------------------------------------------------------------------------------------

void RfControllerNRf24L01P::PowerDown::ToStandbyI() {
   mController->mDevice->powerUp(true);
   Util::delayInMicroseconds(1500); // TODO (BT) can reduce ext oszi ?
   mController->mDevice->receivePayloadSize(I_DeviceNRf24L01P::PIPE_0,32);
   mController->mDevice->receivePayloadSize(I_DeviceNRf24L01P::PIPE_1,32);
   mController->mDevice->receivePayloadSize(I_DeviceNRf24L01P::PIPE_2,32);
   mController->mDevice->receivePayloadSize(I_DeviceNRf24L01P::PIPE_3,32);
   mController->mDevice->receivePayloadSize(I_DeviceNRf24L01P::PIPE_4,32);
   mController->mDevice->receivePayloadSize(I_DeviceNRf24L01P::PIPE_5,32);
   mController->mCurrentState = &mController->mStandbyI;
}

//-------------------------------------------------------------------------------------------------

void RfControllerNRf24L01P::PowerDown::ToRxMode() {
   ToStandbyI();
   mController->mCurrentState->ToRxMode();
}

//-------------------------------------------------------------------------------------------------

void RfControllerNRf24L01P::PowerDown::ToTxMode() {
   ToStandbyI();
   mController->mCurrentState->ToTxMode();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void RfControllerNRf24L01P::StandbyI::ToPowerDown() {
   mController->mDevice->powerUp(false);
   mController->mCurrentState = &mController->mPowerDown;
}

//-------------------------------------------------------------------------------------------------

void RfControllerNRf24L01P::StandbyI::ToRxMode() {
   mController->mDevice->clearDataReady();
   mController->mDevice->clearDataSent();
   mController->mDevice->clearRetransmitsExceeded();

   mController->mDevice->transceiverMode(I_DeviceNRf24L01P::RX_MODE);
   mController->mDevice->chipEnable(true);
   Util::delayInMicroseconds(130);
   mController->mCurrentState = &mController->mRxMode;
}

//-------------------------------------------------------------------------------------------------

void RfControllerNRf24L01P::StandbyI::ToTxMode() {
   if (mController->mDevice->isTransmitFifoEmpty())
   {
      printf("StandbyI::ToTxMode: transmit fifo is empty => StandbyI ! \n");
      return;
   }

   mController->mDevice->clearDataReady();
   mController->mDevice->clearDataSent();
   mController->mDevice->clearRetransmitsExceeded();

   mController->mDevice->transceiverMode(I_DeviceNRf24L01P::TX_MODE);
   mController->mDevice->chipEnable(true);
   Util::delayInMicroseconds(140);
   mController->mCurrentState = &mController->mTxMode;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void RfControllerNRf24L01P::RxMode::ToPowerDown() {
   ToStandbyI();
   mController->mCurrentState->ToPowerDown();
}


//-------------------------------------------------------------------------------------------------

void RfControllerNRf24L01P::RxMode::ToStandbyI() {
   mController->mDevice->chipEnable(true);
   mController->mCurrentState = &mController->mStandbyI;
}

//-------------------------------------------------------------------------------------------------

void RfControllerNRf24L01P::RxMode::ToTxMode() {
   ToStandbyI();
   mController->mCurrentState->ToTxMode();
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void RfControllerNRf24L01P::TxMode::ToPowerDown() {
   ToStandbyI();
   mController->mCurrentState->ToPowerDown();
}

//-------------------------------------------------------------------------------------------------

void RfControllerNRf24L01P::TxMode::ToStandbyI() {
   I_DeviceNRf24L01P::Status status = mController->mDevice->status();
   while(!(status.dataSent() || status.retransmitsExceeded()) ) {
      status = mController->mDevice->status();
      printf("TxMode::ToStandbyI: autoRetransmitCounter = %u \n", (mController->mDevice->autoRetransmitCounter() & 0xff) );
      Util::delayInMicroseconds(10);
   }

   if (status.dataSent()) {
      mController->mDevice->clearDataSent();
   }
   if (status.retransmitsExceeded()) {
         mController->mDevice->clearRetransmitsExceeded();
         printf("TxMode::ToStandbyI: send failed retransmits exceeded ! \n");

   }

   mController->mCurrentState = &mController->mStandbyI;
}

//-------------------------------------------------------------------------------------------------

void RfControllerNRf24L01P::TxMode::ToRxMode() {
   ToStandbyI();
   mController->mCurrentState->ToRxMode();
}

//-------------------------------------------------------------------------------------------------

} // namespace Device
} // namespace Bt

