//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::SpiPlatform
//  
//*************************************************************************************************

#include "Bt/Mcu/SpiPlatform.hpp"

#include <bcm2835.h>

#include "Bt/Mcu/GpioLibrary.hpp"


namespace Bt {
namespace Mcu {


//-------------------------------------------------------------------------------------------------

SpiPlatform::SpiPlatform(I_Spi::BitOrder pBitOrder, I_Spi::Mode pSpiMode, I_Spi::Speed pSpeed, I_Pin& pChipSelectPin)
   : mChipSelectPin(&pChipSelectPin){
   GpioLibrary::ensureIsInitialized();
   bcm2835_spi_begin();
   bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);

   switch (pBitOrder) {
      case I_Spi::BIT_ORDER_LSBFIRST : bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST); break;
      case I_Spi::BIT_ORDER_MSBFIRST : bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); break;
   }

   switch (pSpiMode) {
      case I_Spi::MODE_0 : bcm2835_spi_setDataMode(BCM2835_SPI_MODE0); break;
      case I_Spi::MODE_1 : bcm2835_spi_setDataMode(BCM2835_SPI_MODE1); break;
      case I_Spi::MODE_2 : bcm2835_spi_setDataMode(BCM2835_SPI_MODE2); break;
      case I_Spi::MODE_3 : bcm2835_spi_setDataMode(BCM2835_SPI_MODE3); break;
   }

   switch (pSpeed) {
      case I_Spi::SPEED_4_MHZ : bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); break;
   }

}

//-------------------------------------------------------------------------------------------------

SpiPlatform::~SpiPlatform() {
   bcm2835_spi_end();
}

//-------------------------------------------------------------------------------------------------

uint8_t SpiPlatform::transfer(uint8_t pData)
{
   return bcm2835_spi_transfer(pData);
}

//-------------------------------------------------------------------------------------------------

} // namespace Mcu
} // namespace Bt
