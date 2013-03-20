//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::Pin
//  
//*************************************************************************************************

#include "Bt/Mcu/Pin.hpp"

namespace Bt {
namespace Mcu {


//-------------------------------------------------------------------------------------------------

Pin::Pin(uint8_t pPinId, Mode pInitialMode) : PinPlatform(pPinId, pInitialMode) {

}

//-------------------------------------------------------------------------------------------------

Pin::~Pin() {

}

//-------------------------------------------------------------------------------------------------

void Pin::mode(Mode pMode) {
   PinPlatform::mode(pMode);
}

//-------------------------------------------------------------------------------------------------

void Pin::write(bool gHigh) {
   PinPlatform::write(gHigh);
}

//-------------------------------------------------------------------------------------------------

bool Pin::read() {
   return PinPlatform::read();
}

//-------------------------------------------------------------------------------------------------

} // namespace Mcu
} // namespace Bt
