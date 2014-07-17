//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::InterruptPin
//  
//*************************************************************************************************

#include "Bt/Mcu/InterruptPin.hpp"

namespace Bt {
namespace Mcu {


//-------------------------------------------------------------------------------------------------

InterruptPin::InterruptPin(uint8_t iPinId, Edge iEdge) : InterruptPinPlatform(iPinId, iEdge) {

}

//-------------------------------------------------------------------------------------------------

InterruptPin::~InterruptPin() {

}

//-------------------------------------------------------------------------------------------------

void InterruptPin::enable(std::function<void()> iInterruptHandler) {
   InterruptPinPlatform::enable(iInterruptHandler);
}

//-------------------------------------------------------------------------------------------------

void InterruptPin::disable() {
   InterruptPinPlatform::disable();
}

//-------------------------------------------------------------------------------------------------

bool InterruptPin::read() {
   return InterruptPinPlatform::read();
}

//-------------------------------------------------------------------------------------------------

} // namespace Mcu
} // namespace Bt
