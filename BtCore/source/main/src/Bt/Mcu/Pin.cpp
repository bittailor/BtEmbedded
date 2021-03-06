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

Pin::Pin(uint8_t iPinId, Mode iInitialMode) : PinPlatform(iPinId, iInitialMode) {

}

//-------------------------------------------------------------------------------------------------

Pin::~Pin() {

}

//-------------------------------------------------------------------------------------------------

void Pin::mode(Mode iMode) {
   PinPlatform::mode(iMode);
}

//-------------------------------------------------------------------------------------------------

void Pin::write(bool iHigh) {
   PinPlatform::write(iHigh);
}

//-------------------------------------------------------------------------------------------------

bool Pin::read() {
   return PinPlatform::read();
}

//-------------------------------------------------------------------------------------------------

} // namespace Mcu
} // namespace Bt
