//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::NRf24L01P
//  
//*************************************************************************************************

#include "Bt/Device/NRf24L01P.hpp"

#include <stdint.h>
#include "Bt/Mcu/I_Spi.hpp"

namespace Bt {
namespace Device {

namespace {

enum Masks {
   MASK_REGISTER_CMD = 0x1F
};

enum Commands {
   CMD_R_REGISTER = 0x00,
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


uint8_t readRegister(Bt::Mcu::I_Spi& pSpi, OneByteRegister pRegister)
{
   uint8_t cmd = CMD_R_REGISTER | (pRegister & MASK_REGISTER_CMD);
   pSpi.chipSelect(true);
   pSpi.transfer(cmd);
   uint8_t value = pSpi.transfer(CMD_NOP);
   pSpi.chipSelect(false);
   return value;
}

} // namespace


//-------------------------------------------------------------------------------------------------

NRf24L01P::NRf24L01P(Mcu::I_Spi& pSpi, Mcu::I_Pin& pChipEnable)
   : mSpi(&pSpi), mChipEnable(&pChipEnable)  {

}

//-------------------------------------------------------------------------------------------------

NRf24L01P::~NRf24L01P() {

}

//-------------------------------------------------------------------------------------------------

} // namespace Device
} // namespace Bt
