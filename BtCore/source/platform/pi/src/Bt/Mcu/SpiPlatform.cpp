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

SpiPlatform::SpiPlatform(ISpi::BitOrder pBitOrder, ISpi::Mode pSpiMode, ISpi::Speed pSpeed) {
   GpioLibrary::ensureIsInitialized();
   bcm2835_spi_begin();
   bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);

   switch (pBitOrder) {
      case ISpi::BIT_ORDER_LSBFIRST : bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_LSBFIRST); break;
      case ISpi::BIT_ORDER_MSBFIRST : bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST); break;
   }

   switch (pSpiMode) {
      case ISpi::MODE_0 : bcm2835_spi_setDataMode(BCM2835_SPI_MODE0); break;
      case ISpi::MODE_1 : bcm2835_spi_setDataMode(BCM2835_SPI_MODE1); break;
      case ISpi::MODE_2 : bcm2835_spi_setDataMode(BCM2835_SPI_MODE2); break;
      case ISpi::MODE_3 : bcm2835_spi_setDataMode(BCM2835_SPI_MODE3); break;
   }

   switch (pSpeed) {
      case ISpi::SPEED_4_MHZ : bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_64); break;
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
