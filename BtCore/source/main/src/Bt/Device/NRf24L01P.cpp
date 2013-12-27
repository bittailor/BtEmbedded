//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::NRf24L01P
//  
//*************************************************************************************************

#include "Bt/Device/NRf24L01P.hpp"

#include <stdint.h>
#include <stdio.h>

#include "Bt/Mcu/I_Spi.hpp"
#include "Bt/Util/Delay.hpp"
#include "Bt/Util/StaticArray.hpp"


namespace Bt {
namespace Device {

namespace {

enum Masks {
   MASK_REGISTER_CMD = 0x1F,
   MASK_RF_CH = 0x7F,
   MASK_ARC = 0x0F,
   MASK_ARD = 0xF0
};

enum Commands {
   CMD_R_REGISTER    = 0x00,
   CMD_W_REGISTER    = 0x20,
   CMD_R_RX_PAYLOAD  = 0x61,
   CMD_W_TX_PAYLOAD  = 0xA0,
   CMD_NOP = 0xFF
};

enum OneByteRegister
{
   REGISTER_CONFIG      = 0x00,
   REGISTER_EN_AA       = 0x01,
   REGISTER_EN_RXADDR   = 0x02,
   REGISTER_SETUP_AW    = 0x03,
   REGISTER_SETUP_RETR  = 0x04,
   REGISTER_RF_CH       = 0x05,
   REGISTER_RF_SETUP    = 0x06,
   REGISTER_STATUS      = 0x07,
   REGISTER_OBSERVE_TX  = 0x08,
   REGISTER_RPD         = 0x09,
   REGISTER_RX_ADDR_P2  = 0x0C,
   REGISTER_RX_ADDR_P3  = 0x0D,
   REGISTER_RX_ADDR_P4  = 0x0E,
   REGISTER_RX_ADDR_P5  = 0x0F,
   REGISTER_RX_PW_P0    = 0x11,
   REGISTER_RX_PW_P1    = 0x12,
   REGISTER_RX_PW_P2    = 0x13,
   REGISTER_RX_PW_P3    = 0x14,
   REGISTER_RX_PW_P4    = 0x15,
   REGISTER_RX_PW_P5    = 0x16,
   REGISTER_FIFO_STATUS = 0x17,
   REGISTER_DYNPD       = 0x1C,
   REGISTER_FEATURE     = 0x1D
};

enum FiveByteRegister
{
   REGISTER_RX_ADDR_P0  = 0x0A,
   REGISTER_RX_ADDR_P1  = 0x0B,
   REGISTER_TX_ADDR     = 0x10
};

//-------------------------------------------------------------------------------------------------

inline uint8_t calculateMask(uint8_t pBitSize, uint8_t pOffset) {
   uint8_t mask = ((1 << pBitSize) - 1) << pOffset;
   return mask;
}

//-------------------------------------------------------------------------------------------------

uint8_t readRegister(Bt::Mcu::I_Spi& pSpi, OneByteRegister pRegister)
{
   uint8_t cmd = CMD_R_REGISTER | (pRegister & MASK_REGISTER_CMD);
   pSpi.chipSelect(true);
   pSpi.transfer(cmd);
   uint8_t value = pSpi.transfer(CMD_NOP);
   pSpi.chipSelect(false);
   return value;
}



//-------------------------------------------------------------------------------------------------

uint8_t readSubRegister(Bt::Mcu::I_Spi& pSpi, OneByteRegister pRegister,
                        uint8_t pBitSize, uint8_t pOffset)
{
   uint8_t mask = calculateMask(pBitSize,pOffset);
   uint8_t value = readRegister(pSpi, pRegister);
   value &= mask;
   value >>= pOffset;
   return value;
}

//-------------------------------------------------------------------------------------------------

Util::StaticArray<uint8_t,5> readRegister(Bt::Mcu::I_Spi& pSpi, FiveByteRegister pRegister)
{
   Util::StaticArray<uint8_t,5> value;
   uint8_t cmd = CMD_R_REGISTER | (pRegister & MASK_REGISTER_CMD);

   pSpi.chipSelect(true);
   pSpi.transfer(cmd);
   for (size_t i = 0; i < value.size() ; ++i) {
      value[i] = pSpi.transfer(CMD_NOP);
   }
   pSpi.chipSelect(false);

   return value;
}

//-------------------------------------------------------------------------------------------------

uint8_t writeRegister(Bt::Mcu::I_Spi& pSpi, OneByteRegister pRegister, uint8_t pValue)
{
   uint8_t cmd = CMD_W_REGISTER | (pRegister & MASK_REGISTER_CMD);

   pSpi.chipSelect(true);
   uint8_t status = pSpi.transfer(cmd);
   pSpi.transfer(pValue);
   pSpi.chipSelect(false);
   return status;
}

//-------------------------------------------------------------------------------------------------

uint8_t writeSubRegister(Bt::Mcu::I_Spi& pSpi, OneByteRegister pRegister, uint8_t pValue,
                         uint8_t pBitSize, uint8_t pOffset)
{
   uint8_t mask = calculateMask(pBitSize,pOffset);
   uint8_t value = readRegister(pSpi, pRegister) ;
   value = value & ~mask;
   pValue = pValue << pOffset;
   value = value | (pValue & mask) ;
   return writeRegister(pSpi, pRegister, value);
}

//-------------------------------------------------------------------------------------------------

uint8_t writeRegister(Bt::Mcu::I_Spi& pSpi, FiveByteRegister pRegister, Util::StaticArray<uint8_t,5> pValue)
{
   uint8_t cmd = CMD_W_REGISTER | (pRegister & MASK_REGISTER_CMD);

   pSpi.chipSelect(true);
   uint8_t status = pSpi.transfer(cmd);
   for (size_t i = 0; i < pValue.size() ; ++i) {
      pSpi.transfer(pValue[i]);
   }
   pSpi.chipSelect(false);
   return status;
}

//-------------------------------------------------------------------------------------------------

} // namespace


//-------------------------------------------------------------------------------------------------

NRf24L01P::NRf24L01P(Mcu::I_Spi& pSpi, Mcu::I_Pin& pChipEnable)
   : mSpi(&pSpi), mChipEnable(&pChipEnable)  {
   mChipEnable->write(false);
   Util::delayInMilliseconds(5);
}

//-------------------------------------------------------------------------------------------------

NRf24L01P::~NRf24L01P() {

}

//-------------------------------------------------------------------------------------------------

NRf24L01P::Status NRf24L01P::status()
{
   uint8_t status = readRegister(*mSpi, REGISTER_STATUS);
   printf("status = %02x \n",status);
   return Status(status & 0x40, status & 0x20, status & 0x10);
}

//-------------------------------------------------------------------------------------------------

void NRf24L01P::clearDataReady() {
   writeSubRegister(*mSpi, REGISTER_STATUS,1,1,6);
}

//-------------------------------------------------------------------------------------------------

void NRf24L01P::clearDataSent() {
   writeSubRegister(*mSpi, REGISTER_STATUS,1,1,5);
}

//-------------------------------------------------------------------------------------------------

void NRf24L01P::clearRetransmitsExceeded() {
   writeSubRegister(*mSpi, REGISTER_STATUS,1,1,4);
}

//-------------------------------------------------------------------------------------------------

bool NRf24L01P::powerUp() {
   return readSubRegister(*mSpi, REGISTER_CONFIG,1,1);
}

//-------------------------------------------------------------------------------------------------

void NRf24L01P::powerUp(bool pValue) {
   writeSubRegister(*mSpi, REGISTER_CONFIG,pValue,1,1);
}

//-------------------------------------------------------------------------------------------------

NRf24L01P::TransceiverMode NRf24L01P::transceiverMode() {
   return (readSubRegister(*mSpi, REGISTER_CONFIG,1,0)) ? RX_MODE : TX_MODE;
}

//-------------------------------------------------------------------------------------------------

void NRf24L01P::transceiverMode(TransceiverMode pMode) {
   writeSubRegister(*mSpi, REGISTER_CONFIG,pMode == RX_MODE,1,0);
}

//-------------------------------------------------------------------------------------------------

void NRf24L01P::chipEnable(bool pValue) {
   mChipEnable->write(pValue);
}

//-------------------------------------------------------------------------------------------------

uint8_t NRf24L01P::autoRetransmitDelay() {
   return readSubRegister(*mSpi, REGISTER_SETUP_RETR,4,4);
}

//-------------------------------------------------------------------------------------------------

void NRf24L01P::autoRetransmitDelay(uint8_t pDelay) {
   writeSubRegister(*mSpi, REGISTER_SETUP_RETR, pDelay, 4, 4);
}

//-------------------------------------------------------------------------------------------------

uint8_t NRf24L01P::autoRetransmitCount() {
   return readSubRegister(*mSpi, REGISTER_SETUP_RETR,4,0);
}

//-------------------------------------------------------------------------------------------------

void NRf24L01P::autoRetransmitCount(uint8_t pCount) {
   writeSubRegister(*mSpi, REGISTER_SETUP_RETR, pCount, 4, 0);
}

//-------------------------------------------------------------------------------------------------

uint8_t NRf24L01P::autoRetransmitCounter(){
   return readSubRegister(*mSpi, REGISTER_OBSERVE_TX,4,0);
}

//-------------------------------------------------------------------------------------------------

uint8_t NRf24L01P::channel() {
   uint8_t value = readRegister(*mSpi, REGISTER_RF_CH) & MASK_RF_CH;
   printf("RF_CH = %02x \n",value);
   return value & 0x7F;
}

//-------------------------------------------------------------------------------------------------

void NRf24L01P::channel(uint8_t pChannel) {
   uint8_t value = pChannel & MASK_RF_CH;
   uint8_t status = writeRegister(*mSpi, REGISTER_RF_CH, value);
   printf("write RF_CH status = %02x \n",status);
}

//-------------------------------------------------------------------------------------------------

NRf24L01P::Address NRf24L01P::receiveAddress(Pipe pPipe) {
   switch (pPipe) {
      case PIPE_0:
         return readRegister(*mSpi, REGISTER_RX_ADDR_P0);
      case PIPE_1:
         return readRegister(*mSpi, REGISTER_RX_ADDR_P1);
      default:
         break;
   }

   Address base = readRegister(*mSpi, REGISTER_RX_ADDR_P1);

   OneByteRegister oneByteRegister = REGISTER_RX_ADDR_P2;
   switch (pPipe) {
      case PIPE_2 : oneByteRegister = REGISTER_RX_ADDR_P2; break;
      case PIPE_3 : oneByteRegister = REGISTER_RX_ADDR_P3; break;
      case PIPE_4 : oneByteRegister = REGISTER_RX_ADDR_P4; break;
      case PIPE_5 : oneByteRegister = REGISTER_RX_ADDR_P5; break;
      default     : oneByteRegister = REGISTER_RX_ADDR_P2; break;
   }

   base.raw()[0] = readRegister(*mSpi, oneByteRegister);

   return base;
}

//-------------------------------------------------------------------------------------------------

void NRf24L01P::receiveAddress(Pipe pPipe, Address pAddress) {
   switch (pPipe) {
      case PIPE_0:
         writeRegister(*mSpi, REGISTER_RX_ADDR_P0, pAddress.raw());
         return;
      case PIPE_1:
         writeRegister(*mSpi, REGISTER_RX_ADDR_P1, pAddress.raw());
         return;
      default:
         break;
   }

   OneByteRegister oneByteRegister = REGISTER_RX_ADDR_P2;
   switch (pPipe) {
      case PIPE_2 : oneByteRegister = REGISTER_RX_ADDR_P2; break;
      case PIPE_3 : oneByteRegister = REGISTER_RX_ADDR_P3; break;
      case PIPE_4 : oneByteRegister = REGISTER_RX_ADDR_P4; break;
      case PIPE_5 : oneByteRegister = REGISTER_RX_ADDR_P5; break;
      default     : oneByteRegister = REGISTER_RX_ADDR_P2; break;
   }

   writeRegister(*mSpi, oneByteRegister, pAddress.raw()           [0]);
}

//-------------------------------------------------------------------------------------------------

uint8_t NRf24L01P::receivePayloadSize(Pipe pPipe) {
   OneByteRegister oneByteRegister;
   switch (pPipe) {
      case PIPE_0 : oneByteRegister = REGISTER_RX_PW_P0; break;
      case PIPE_1 : oneByteRegister = REGISTER_RX_PW_P1; break;
      case PIPE_2 : oneByteRegister = REGISTER_RX_PW_P2; break;
      case PIPE_3 : oneByteRegister = REGISTER_RX_PW_P3; break;
      case PIPE_4 : oneByteRegister = REGISTER_RX_PW_P4; break;
      case PIPE_5 : oneByteRegister = REGISTER_RX_PW_P5; break;
      default     : oneByteRegister = REGISTER_RX_PW_P0; break;
   }

   return readSubRegister(*mSpi,oneByteRegister,6,0);
}

//-------------------------------------------------------------------------------------------------

void NRf24L01P::receivePayloadSize(Pipe pPipe, uint8_t pSize) {
   OneByteRegister oneByteRegister;
   switch (pPipe) {
      case PIPE_0 : oneByteRegister = REGISTER_RX_PW_P0; break;
      case PIPE_1 : oneByteRegister = REGISTER_RX_PW_P1; break;
      case PIPE_2 : oneByteRegister = REGISTER_RX_PW_P2; break;
      case PIPE_3 : oneByteRegister = REGISTER_RX_PW_P3; break;
      case PIPE_4 : oneByteRegister = REGISTER_RX_PW_P4; break;
      case PIPE_5 : oneByteRegister = REGISTER_RX_PW_P5; break;
      default     : oneByteRegister = REGISTER_RX_PW_P0; break;
   }
   writeSubRegister(*mSpi,oneByteRegister,pSize,6,0);
}

//-------------------------------------------------------------------------------------------------

NRf24L01P::Address NRf24L01P::transmitAddress() {
   return readRegister(*mSpi, REGISTER_TX_ADDR);
}


//-------------------------------------------------------------------------------------------------

void NRf24L01P::transmitAddress(Address pAddress) {
   writeRegister(*mSpi, REGISTER_TX_ADDR, pAddress.raw());
}

//-------------------------------------------------------------------------------------------------

bool NRf24L01P::isTransmitFifoEmpty() {
   return readSubRegister(*mSpi, REGISTER_FIFO_STATUS,1,4);
}

//-------------------------------------------------------------------------------------------------

bool NRf24L01P::isTransmitFifoFull() {
   return readSubRegister(*mSpi, REGISTER_FIFO_STATUS,1,5);
}

//-------------------------------------------------------------------------------------------------

bool NRf24L01P::isReceiveFifoEmpty() {
   return readSubRegister(*mSpi, REGISTER_FIFO_STATUS,1,0);
}

//-------------------------------------------------------------------------------------------------

bool NRf24L01P::isReceiveFifoFull() {
   return readSubRegister(*mSpi, REGISTER_FIFO_STATUS,1,1);
}

//-------------------------------------------------------------------------------------------------

size_t NRf24L01P::writeTransmitPayload(uint8_t* pData, size_t pSize) {

   size_t dataSize;
   size_t stuffingSize;

   if (pSize >= PAYLOAD_SIZE) {
      dataSize = PAYLOAD_SIZE;
      stuffingSize = 0;
   } else {
      dataSize = pSize;
      stuffingSize = PAYLOAD_SIZE - pSize;
   }

   mSpi->chipSelect(true);
   mSpi->transfer(CMD_W_TX_PAYLOAD);
   for (size_t i = 0; i < dataSize ; ++i) {
      mSpi->transfer(pData[i]);
   }
   for (size_t i = 0; i < stuffingSize ; ++i) {
      mSpi->transfer(0);
   }
   mSpi->chipSelect(false);

   return dataSize;
}

//-------------------------------------------------------------------------------------------------

size_t NRf24L01P::readReceivePayload(Pipe& pPipe, uint8_t* pData, size_t pSize) {
   size_t dataSize;
   size_t stuffingSize;

   switch(readSubRegister(*mSpi,REGISTER_STATUS,3,1))
   {
      case 0 : pPipe = PIPE_0 ; break;
      case 1 : pPipe = PIPE_1 ; break;
      case 2 : pPipe = PIPE_2 ; break;
      case 3 : pPipe = PIPE_3 ; break;
      case 4 : pPipe = PIPE_4 ; break;
      case 5 : pPipe = PIPE_5 ; break;
   }

   if (pSize >= PAYLOAD_SIZE) {
      dataSize = PAYLOAD_SIZE;
      stuffingSize = 0;
   } else {
      dataSize = pSize;
      stuffingSize = PAYLOAD_SIZE - pSize;
   }

   mSpi->chipSelect(true);
   mSpi->transfer(CMD_R_RX_PAYLOAD);
   for (size_t i = 0; i < dataSize ; ++i) {
      pData[i] = mSpi->transfer(CMD_NOP);
   }
   for (size_t i = 0; i < stuffingSize ; ++i) {
      mSpi->transfer(CMD_NOP);
   }
   mSpi->chipSelect(false);
   return dataSize;
}

//-------------------------------------------------------------------------------------------------

bool NRf24L01P::dynamicPayloadFeatureEnabled() {
   return readSubRegister(*mSpi, REGISTER_FEATURE, 1, 2);
}

//-------------------------------------------------------------------------------------------------

void NRf24L01P::dynamicPayloadFeatureEnabled(bool pValue) {
   writeSubRegister(*mSpi, REGISTER_FEATURE, pValue, 1, 2);
}

//-------------------------------------------------------------------------------------------------

bool NRf24L01P::dynamicPayloadEnabled(Pipe pPipe) {
   uint8_t offset = 0;

   switch (pPipe) {
      case PIPE_0 : offset = 0; break;
      case PIPE_1 : offset = 1; break;
      case PIPE_2 : offset = 2; break;
      case PIPE_3 : offset = 3; break;
      case PIPE_4 : offset = 4; break;
      case PIPE_5 : offset = 5; break;
      default     : offset = 0; break;
   }

   return readSubRegister(*mSpi, REGISTER_DYNPD, 1, offset);
}

//-------------------------------------------------------------------------------------------------

void NRf24L01P::dynamicPayloadEnabled(Pipe pPipe, bool pValue) {
   uint8_t offset = 0;

   switch (pPipe) {
      case PIPE_0 : offset = 0; break;
      case PIPE_1 : offset = 1; break;
      case PIPE_2 : offset = 2; break;
      case PIPE_3 : offset = 3; break;
      case PIPE_4 : offset = 4; break;
      case PIPE_5 : offset = 5; break;
      default     : offset = 0; break;
   }

   writeSubRegister(*mSpi, REGISTER_DYNPD, pValue, 1, offset);
}

//-------------------------------------------------------------------------------------------------


} // namespace Device
} // namespace Bt
