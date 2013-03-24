//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::Spi
//  
//*************************************************************************************************

#include "Bt/Mcu/Spi.hpp"

namespace Bt {
namespace Mcu {


//-------------------------------------------------------------------------------------------------

Spi::Spi(BitOrder pBitOrder, Mode pSpiMode, Speed pSpeed, I_Pin& pChipSelectPin)
   : SpiPlatform(pBitOrder,pSpiMode,pSpeed)
   , mChipSelectPin(&pChipSelectPin) {
   mChipSelectPin->mode(I_Pin::MODE_OUTPUT);
   mChipSelectPin->write(true);
}

//-------------------------------------------------------------------------------------------------

Spi::~Spi() {

}

//-------------------------------------------------------------------------------------------------


void Spi::chipSelect(bool pSelect) {
   mChipSelectPin->write(!pSelect);
}

//-------------------------------------------------------------------------------------------------

uint8_t Spi::transfer(uint8_t pData) {
   return SpiPlatform::transfer(pData);
}

//-------------------------------------------------------------------------------------------------



} // namespace Mcu
} // namespace Bt
