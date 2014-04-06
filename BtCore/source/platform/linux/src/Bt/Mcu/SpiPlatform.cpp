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

}

//-------------------------------------------------------------------------------------------------

SpiPlatform::~SpiPlatform() {

}

//-------------------------------------------------------------------------------------------------

uint8_t SpiPlatform::transfer(uint8_t data) {
   return 0u;
}

//-------------------------------------------------------------------------------------------------

} // namespace Mcu
} // namespace Bt