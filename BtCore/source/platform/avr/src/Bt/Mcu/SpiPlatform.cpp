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

namespace Bt {
namespace Mcu {


//-------------------------------------------------------------------------------------------------

SpiPlatform::SpiPlatform(I_Spi::BitOrder pBitOrder, I_Spi::Mode pSpiMode, I_Spi::Speed pSpeed) {
   //TODO (BT) implement for avr
}

//-------------------------------------------------------------------------------------------------

SpiPlatform::~SpiPlatform() {
   //TODO (BT) implement for avr
}

//-------------------------------------------------------------------------------------------------

uint8_t SpiPlatform::transfer(uint8_t pData)
{
   //TODO (BT) implement for avr
   return 0;
}

//-------------------------------------------------------------------------------------------------

} // namespace Mcu
} // namespace Bt
