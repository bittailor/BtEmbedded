//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::Rf24Device
//  
//*************************************************************************************************

#include "Bt/Rf24/Rf24Device.hpp"

#include <stdint.h>
#include <stdio.h>
#include <iomanip>
#include <cstring>

#include <Bt/Log/Logging.hpp>


#include "Bt/Mcu/I_Spi.hpp"
#include "Bt/Util/Timing.hpp"
#include "Bt/Util/StaticArray.hpp"


namespace Bt {
namespace Rf24 {

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
   CMD_FLUSH_TX      = 0xE1,
   CMD_FLUSH_RX      = 0xE2,
   CMD_R_RX_PL_WID   = 0x60,
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

inline uint8_t calculateMask(uint8_t iBitSize, uint8_t iOffset) {
   uint8_t mask = ((1 << iBitSize) - 1) << iOffset;
   return mask;
}

//-------------------------------------------------------------------------------------------------

uint8_t readRegister(Bt::Mcu::I_Spi& iSpi, OneByteRegister iRegister)
{
   uint8_t cmd = CMD_R_REGISTER | (iRegister & MASK_REGISTER_CMD);
   uint8_t txBuffer[2] = {cmd,CMD_NOP};
   uint8_t rxBuffer[2] = {0};

   iSpi.transfer(txBuffer,rxBuffer,2);

   return rxBuffer[1];
}



//-------------------------------------------------------------------------------------------------

uint8_t readSubRegister(Bt::Mcu::I_Spi& iSpi, OneByteRegister iRegister,
                        uint8_t iBitSize, uint8_t iOffset)
{
   uint8_t mask = calculateMask(iBitSize,iOffset);
   uint8_t value = readRegister(iSpi, iRegister);
   value &= mask;
   value >>= iOffset;
   return value;
}

//-------------------------------------------------------------------------------------------------

Util::StaticArray<uint8_t,5> readRegister(Bt::Mcu::I_Spi& iSpi, FiveByteRegister iRegister)
{
   Util::StaticArray<uint8_t,5> value;
   uint8_t cmd = CMD_R_REGISTER | (iRegister & MASK_REGISTER_CMD);

   uint8_t txBuffer[6] = {cmd,CMD_NOP,CMD_NOP,CMD_NOP,CMD_NOP,CMD_NOP};
   uint8_t rxBuffer[6] = {0};

   iSpi.transfer(txBuffer,rxBuffer,6);

   for (size_t i = 0 ; i < value.size()  ; ++i) {
      value[i] = rxBuffer[i+1];
   }

   return value;
}

//-------------------------------------------------------------------------------------------------

uint8_t writeRegister(Bt::Mcu::I_Spi& iSpi, OneByteRegister iRegister, uint8_t iValue)
{
   uint8_t cmd = CMD_W_REGISTER | (iRegister & MASK_REGISTER_CMD);

   uint8_t txBuffer[2] = {cmd,iValue};
   uint8_t rxBuffer[2] = {0};

   iSpi.transfer(txBuffer,rxBuffer,2);

   return rxBuffer[0];
}

//-------------------------------------------------------------------------------------------------

uint8_t writeSubRegister(Bt::Mcu::I_Spi& iSpi, OneByteRegister iRegister, uint8_t iValue,
                         uint8_t iBitSize, uint8_t iOffset)
{
   uint8_t mask = calculateMask(iBitSize,iOffset);
   uint8_t value = readRegister(iSpi, iRegister) ;
   value = value & ~mask;
   iValue = iValue << iOffset;
   value = value | (iValue & mask) ;
   return writeRegister(iSpi, iRegister, value);
}

//-------------------------------------------------------------------------------------------------

uint8_t writeRegister(Bt::Mcu::I_Spi& iSpi, FiveByteRegister iRegister, Util::StaticArray<uint8_t,5> iValue)
{
   uint8_t cmd = CMD_W_REGISTER | (iRegister & MASK_REGISTER_CMD);

   uint8_t txBuffer[6] = {cmd,iValue[0],iValue[1],iValue[2],iValue[3],iValue[4]};
   uint8_t rxBuffer[6] = {0};

   iSpi.transfer(txBuffer,rxBuffer,6);

   return rxBuffer[0];
}

//-------------------------------------------------------------------------------------------------

} // namespace


//-------------------------------------------------------------------------------------------------

Rf24Device::Rf24Device(Mcu::I_Spi& iSpi, Mcu::I_Pin& iChipEnable)
   : mSpi(&iSpi), mChipEnable(&iChipEnable)  {
   mChipEnable->write(false);
   Util::delayInMilliseconds(5);
}

//-------------------------------------------------------------------------------------------------

Rf24Device::~Rf24Device() {

}



//-------------------------------------------------------------------------------------------------

Rf24Device::Status Rf24Device::status()
{
   static uint8_t sLastStatus = 0x00;
   uint8_t status = readRegister(*mSpi, REGISTER_STATUS);
   if(status != sLastStatus) {
      sLastStatus = status;
      BT_LOG(DEBUG) << "status = " << Log::HexLogStreamer<int>{4,status};
   }
   return Status(status);
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::clearDataReady() {
   writeSubRegister(*mSpi, REGISTER_STATUS,1,1,6);
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::clearDataSent() {
   writeSubRegister(*mSpi, REGISTER_STATUS,1,1,5);
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::clearRetransmitsExceeded() {
   writeSubRegister(*mSpi, REGISTER_STATUS,1,1,4);
}

//-------------------------------------------------------------------------------------------------

bool Rf24Device::powerUp() {
   return readSubRegister(*mSpi, REGISTER_CONFIG,1,1);
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::powerUp(bool iValue) {
   writeSubRegister(*mSpi, REGISTER_CONFIG,iValue,1,1);
}

//-------------------------------------------------------------------------------------------------

Rf24Device::TransceiverMode Rf24Device::transceiverMode() {
   return (readSubRegister(*mSpi, REGISTER_CONFIG,1,0)) ? RX_MODE : TX_MODE;
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::transceiverMode(TransceiverMode iMode) {
   writeSubRegister(*mSpi, REGISTER_CONFIG,iMode == RX_MODE,1,0);
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::chipEnable(bool iValue) {
   mChipEnable->write(iValue);
}

//-------------------------------------------------------------------------------------------------

uint8_t Rf24Device::autoRetransmitDelay() {
   return readSubRegister(*mSpi, REGISTER_SETUP_RETR,4,4);
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::autoRetransmitDelay(uint8_t iDelay) {
   writeSubRegister(*mSpi, REGISTER_SETUP_RETR, iDelay, 4, 4);
}

//-------------------------------------------------------------------------------------------------

uint8_t Rf24Device::autoRetransmitCount() {
   return readSubRegister(*mSpi, REGISTER_SETUP_RETR,4,0);
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::autoRetransmitCount(uint8_t iCount) {
   writeSubRegister(*mSpi, REGISTER_SETUP_RETR, iCount, 4, 0);
}

//-------------------------------------------------------------------------------------------------

uint8_t Rf24Device::autoRetransmitCounter(){
   return readSubRegister(*mSpi, REGISTER_OBSERVE_TX,4,0);
}

//-------------------------------------------------------------------------------------------------

uint8_t Rf24Device::channel() {
   uint8_t value = readRegister(*mSpi, REGISTER_RF_CH) & MASK_RF_CH;
   return value & 0x7F;
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::channel(uint8_t iChannel) {
   uint8_t value = iChannel & MASK_RF_CH;
   writeRegister(*mSpi, REGISTER_RF_CH, value);
}

//-------------------------------------------------------------------------------------------------

I_Rf24Device::DataRate Rf24Device::dataRate() {
   uint8_t low = readSubRegister(*mSpi, REGISTER_RF_SETUP, 1, 5);
   uint8_t high = readSubRegister(*mSpi, REGISTER_RF_SETUP, 1, 3);
   if(low) {
      return I_Rf24Device::DR_250_KBPS;
   }
   if(high) {
      return I_Rf24Device::DR_2_MBPS;
   }
   return I_Rf24Device::DR_1_MBPS;

}

//-------------------------------------------------------------------------------------------------

void Rf24Device::dataRate(DataRate iDataRate) {
   uint8_t low = (iDataRate == I_Rf24Device::DR_250_KBPS);
   uint8_t high = (iDataRate == I_Rf24Device::DR_2_MBPS);
   writeSubRegister(*mSpi, REGISTER_RF_SETUP, low, 1, 5);
   writeSubRegister(*mSpi, REGISTER_RF_SETUP, high, 1, 3);
}

//-------------------------------------------------------------------------------------------------

RfAddress Rf24Device::receiveAddress(RfPipe iPipe) {
   switch (iPipe) {
      case RfPipe::PIPE_0:
         return readRegister(*mSpi, REGISTER_RX_ADDR_P0);
      case RfPipe::PIPE_1:
         return readRegister(*mSpi, REGISTER_RX_ADDR_P1);
      default:
         break;
   }

   RfAddress base = readRegister(*mSpi, REGISTER_RX_ADDR_P1);

   OneByteRegister oneByteRegister = REGISTER_RX_ADDR_P2;
   switch (iPipe) {
      case RfPipe::PIPE_2 : oneByteRegister = REGISTER_RX_ADDR_P2; break;
      case RfPipe::PIPE_3 : oneByteRegister = REGISTER_RX_ADDR_P3; break;
      case RfPipe::PIPE_4 : oneByteRegister = REGISTER_RX_ADDR_P4; break;
      case RfPipe::PIPE_5 : oneByteRegister = REGISTER_RX_ADDR_P5; break;
      default     : oneByteRegister = REGISTER_RX_ADDR_P2; break;
   }

   base.raw()[0] = readRegister(*mSpi, oneByteRegister);

   return base;
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::receiveAddress(RfPipe iPipe, RfAddress iAddress) {
   switch (iPipe) {
      case RfPipe::PIPE_0:
         writeRegister(*mSpi, REGISTER_RX_ADDR_P0, iAddress.raw());
         return;
      case RfPipe::PIPE_1:
         writeRegister(*mSpi, REGISTER_RX_ADDR_P1, iAddress.raw());
         return;
      default:
         break;
   }

   OneByteRegister oneByteRegister = REGISTER_RX_ADDR_P2;
   switch (iPipe) {
      case RfPipe::PIPE_2 : oneByteRegister = REGISTER_RX_ADDR_P2; break;
      case RfPipe::PIPE_3 : oneByteRegister = REGISTER_RX_ADDR_P3; break;
      case RfPipe::PIPE_4 : oneByteRegister = REGISTER_RX_ADDR_P4; break;
      case RfPipe::PIPE_5 : oneByteRegister = REGISTER_RX_ADDR_P5; break;
      default     : oneByteRegister = REGISTER_RX_ADDR_P2; break;
   }

   writeRegister(*mSpi, oneByteRegister, iAddress.raw()[0]);
}

//-------------------------------------------------------------------------------------------------

bool Rf24Device::receivePipeEnabled(RfPipe iPipe) {
   uint8_t offset = 0;

   switch (iPipe) {
      case RfPipe::PIPE_0 : offset = 0; break;
      case RfPipe::PIPE_1 : offset = 1; break;
      case RfPipe::PIPE_2 : offset = 2; break;
      case RfPipe::PIPE_3 : offset = 3; break;
      case RfPipe::PIPE_4 : offset = 4; break;
      case RfPipe::PIPE_5 : offset = 5; break;
      default     : offset = 0; break;
   }

   return readSubRegister(*mSpi, REGISTER_EN_RXADDR, 1, offset);
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::receivePipeEnabled(RfPipe iPipe, bool iValue) {
   uint8_t offset = 0;

   switch (iPipe) {
      case RfPipe::PIPE_0 : offset = 0; break;
      case RfPipe::PIPE_1 : offset = 1; break;
      case RfPipe::PIPE_2 : offset = 2; break;
      case RfPipe::PIPE_3 : offset = 3; break;
      case RfPipe::PIPE_4 : offset = 4; break;
      case RfPipe::PIPE_5 : offset = 5; break;
      default     : offset = 0; break;
   }

   writeSubRegister(*mSpi, REGISTER_EN_RXADDR, iValue, 1, offset);
}

//-------------------------------------------------------------------------------------------------

uint8_t Rf24Device::receivePayloadSize(RfPipe iPipe) {
   OneByteRegister oneByteRegister;
   switch (iPipe) {
      case RfPipe::PIPE_0 : oneByteRegister = REGISTER_RX_PW_P0; break;
      case RfPipe::PIPE_1 : oneByteRegister = REGISTER_RX_PW_P1; break;
      case RfPipe::PIPE_2 : oneByteRegister = REGISTER_RX_PW_P2; break;
      case RfPipe::PIPE_3 : oneByteRegister = REGISTER_RX_PW_P3; break;
      case RfPipe::PIPE_4 : oneByteRegister = REGISTER_RX_PW_P4; break;
      case RfPipe::PIPE_5 : oneByteRegister = REGISTER_RX_PW_P5; break;
      default     : oneByteRegister = REGISTER_RX_PW_P0; break;
   }

   return readSubRegister(*mSpi,oneByteRegister,6,0);
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::receivePayloadSize(RfPipe iPipe, uint8_t iSize) {
   OneByteRegister oneByteRegister;
   switch (iPipe) {
      case RfPipe::PIPE_0 : oneByteRegister = REGISTER_RX_PW_P0; break;
      case RfPipe::PIPE_1 : oneByteRegister = REGISTER_RX_PW_P1; break;
      case RfPipe::PIPE_2 : oneByteRegister = REGISTER_RX_PW_P2; break;
      case RfPipe::PIPE_3 : oneByteRegister = REGISTER_RX_PW_P3; break;
      case RfPipe::PIPE_4 : oneByteRegister = REGISTER_RX_PW_P4; break;
      case RfPipe::PIPE_5 : oneByteRegister = REGISTER_RX_PW_P5; break;
      default     : oneByteRegister = REGISTER_RX_PW_P0; break;
   }
   writeSubRegister(*mSpi,oneByteRegister,iSize,6,0);
}

//-------------------------------------------------------------------------------------------------

RfAddress Rf24Device::transmitAddress() {
   return readRegister(*mSpi, REGISTER_TX_ADDR);
}


//-------------------------------------------------------------------------------------------------

void Rf24Device::transmitAddress(RfAddress iAddress) {
   writeRegister(*mSpi, REGISTER_TX_ADDR, iAddress.raw());
}

//-------------------------------------------------------------------------------------------------

bool Rf24Device::isTransmitFifoEmpty() {
   return readSubRegister(*mSpi, REGISTER_FIFO_STATUS,1,4);
}

//-------------------------------------------------------------------------------------------------

bool Rf24Device::isTransmitFifoFull() {
   return readSubRegister(*mSpi, REGISTER_FIFO_STATUS,1,5);
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::flushTransmitFifo() {
   mSpi->transfer(CMD_FLUSH_TX);
}

//-------------------------------------------------------------------------------------------------

bool Rf24Device::isReceiveFifoEmpty() {
   return readSubRegister(*mSpi, REGISTER_FIFO_STATUS,1,0);
}

//-------------------------------------------------------------------------------------------------

bool Rf24Device::isReceiveFifoFull() {
   return readSubRegister(*mSpi, REGISTER_FIFO_STATUS,1,1);
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::flushReceiveFifo() {
   mSpi->transfer(CMD_FLUSH_RX);
}

//-------------------------------------------------------------------------------------------------

size_t Rf24Device::writeTransmitPayload(uint8_t* iData, size_t iSize) {

   size_t dataSize;

   if (iSize <= MAX_PAYLOAD_SIZE) {
      dataSize = iSize;
   } else {
      dataSize = MAX_PAYLOAD_SIZE;
   }

   uint8_t txBuffer[MAX_PAYLOAD_SIZE + 1] = {CMD_W_TX_PAYLOAD};
   uint8_t rxBuffer[MAX_PAYLOAD_SIZE + 1] = {0};

   std::memcpy(txBuffer+1,iData,dataSize);

   mSpi->transfer(txBuffer,rxBuffer,dataSize+1);

   return dataSize;
}

//-------------------------------------------------------------------------------------------------

size_t Rf24Device::availableReceivePayload() {

   uint8_t txBuffer[2] = {CMD_R_RX_PL_WID,CMD_NOP};
   uint8_t rxBuffer[2] = {0};

   mSpi->transfer(txBuffer,rxBuffer,2);

   return rxBuffer[1];
}

//-------------------------------------------------------------------------------------------------

size_t Rf24Device::readReceivePayload(RfPipe& oPipe, uint8_t* oData, size_t iSize) {

   switch(readSubRegister(*mSpi,REGISTER_STATUS,3,1))
   {
      case 0 : oPipe = RfPipe::PIPE_0 ; break;
      case 1 : oPipe = RfPipe::PIPE_1 ; break;
      case 2 : oPipe = RfPipe::PIPE_2 ; break;
      case 3 : oPipe = RfPipe::PIPE_3 ; break;
      case 4 : oPipe = RfPipe::PIPE_4 ; break;
      case 5 : oPipe = RfPipe::PIPE_5 ; break;
   }

   size_t availableSize = availableReceivePayload();
   if (0 >= availableSize ||  availableSize > MAX_PAYLOAD_SIZE) {
      BT_LOG(WARNING) << "invalid availableSize : 0 >= " << availableSize << " > "<< MAX_PAYLOAD_SIZE << " => retry read available receive payload";
      availableSize = availableReceivePayload();
   }
   if (0 >= availableSize ||  availableSize > MAX_PAYLOAD_SIZE) {
      BT_LOG(ERROR) << "invalid availableSize : 0 >= " << availableSize << " > "<< MAX_PAYLOAD_SIZE <<  " => flush the RX FIFO!";
      flushReceiveFifo();
      return 0;
   }

   size_t dataSize;

   if (iSize >= availableSize) {
      dataSize = availableSize;
   } else {
      dataSize = iSize;
   }

   uint8_t txBuffer[MAX_PAYLOAD_SIZE + 1] = {CMD_R_RX_PAYLOAD};
   uint8_t rxBuffer[MAX_PAYLOAD_SIZE + 1] = {0};
   std::fill(txBuffer + 1,txBuffer + MAX_PAYLOAD_SIZE + 1,CMD_NOP);

   mSpi->transfer(txBuffer,rxBuffer,availableSize + 1);

   std::memcpy(oData, rxBuffer+1, dataSize);

   return dataSize;
}

//-------------------------------------------------------------------------------------------------

bool Rf24Device::dynamicPayloadFeatureEnabled() {
   return readSubRegister(*mSpi, REGISTER_FEATURE, 1, 2);
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::dynamicPayloadFeatureEnabled(bool iValue) {
   writeSubRegister(*mSpi, REGISTER_FEATURE, iValue, 1, 2);
}

//-------------------------------------------------------------------------------------------------

bool Rf24Device::dynamicPayloadEnabled(RfPipe iPipe) {
   uint8_t offset = 0;

   switch (iPipe) {
      case RfPipe::PIPE_0 : offset = 0; break;
      case RfPipe::PIPE_1 : offset = 1; break;
      case RfPipe::PIPE_2 : offset = 2; break;
      case RfPipe::PIPE_3 : offset = 3; break;
      case RfPipe::PIPE_4 : offset = 4; break;
      case RfPipe::PIPE_5 : offset = 5; break;
      default     : offset = 0; break;
   }

   return readSubRegister(*mSpi, REGISTER_DYNPD, 1, offset);
}

//-------------------------------------------------------------------------------------------------

void Rf24Device::dynamicPayloadEnabled(RfPipe iPipe, bool iValue) {
   uint8_t offset = 0;

   switch (iPipe) {
      case RfPipe::PIPE_0 : offset = 0; break;
      case RfPipe::PIPE_1 : offset = 1; break;
      case RfPipe::PIPE_2 : offset = 2; break;
      case RfPipe::PIPE_3 : offset = 3; break;
      case RfPipe::PIPE_4 : offset = 4; break;
      case RfPipe::PIPE_5 : offset = 5; break;
      default     : offset = 0; break;
   }

   writeSubRegister(*mSpi, REGISTER_DYNPD, iValue, 1, offset);
}

//-------------------------------------------------------------------------------------------------


} // namespace Device
} // namespace Bt
