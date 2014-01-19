//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::SpiPlatform
//  
//*************************************************************************************************

#include <Arduino.h>
#include <SPI.h>

#include "Bt/Mcu/SpiPlatform.hpp"

namespace Bt {
namespace Mcu {


//-------------------------------------------------------------------------------------------------

SpiPlatform::SpiPlatform(I_Spi::BitOrder pBitOrder, I_Spi::Mode pSpiMode, I_Spi::Speed pSpeed) {
   SPI.begin();


   switch (pBitOrder) {
        case I_Spi::BIT_ORDER_LSBFIRST : SPI.setBitOrder(LSBFIRST); break;
        case I_Spi::BIT_ORDER_MSBFIRST : SPI.setBitOrder(MSBFIRST); break;
     }

     switch (pSpiMode) {
        case I_Spi::MODE_0 : SPI.setDataMode(SPI_MODE0); break;
        case I_Spi::MODE_1 : SPI.setDataMode(SPI_MODE1); break;
        case I_Spi::MODE_2 : SPI.setDataMode(SPI_MODE2); break;
        case I_Spi::MODE_3 : SPI.setDataMode(SPI_MODE3); break;
     }

     //TODO (BT) check/calculate from  F_CPU

     switch (pSpeed) {
        case I_Spi::SPEED_4_MHZ : SPI.setClockDivider(SPI_CLOCK_DIV4); break;
        case I_Spi::SPEED_8_MHZ : SPI.setClockDivider(SPI_CLOCK_DIV2); break;
     }
}

//-------------------------------------------------------------------------------------------------

SpiPlatform::~SpiPlatform() {
}

//-------------------------------------------------------------------------------------------------

uint8_t SpiPlatform::transfer(uint8_t pData)
{
   return SPI.transfer(pData);
}

//-------------------------------------------------------------------------------------------------

} // namespace Mcu
} // namespace Bt
