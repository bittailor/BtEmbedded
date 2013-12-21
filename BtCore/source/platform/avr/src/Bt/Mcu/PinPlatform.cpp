//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::PinPlatform
//  
//*************************************************************************************************

#include "Bt/Mcu/PinPlatform.hpp"

#include "Bt/Mcu/I_Pin.hpp"

using Bt::Mcu::I_Pin;



namespace Bt {
namespace Mcu {


//-------------------------------------------------------------------------------------------------

PinPlatform::PinPlatform(uint8_t pPinId, I_Pin::Mode pInitialMode) : mPinId(pPinId) {
   mode(pInitialMode);
   //TODO (BT) implement for avr
}

//-------------------------------------------------------------------------------------------------

void PinPlatform::mode(I_Pin::Mode pMode) {
   //TODO (BT) implement for avr
}

//-------------------------------------------------------------------------------------------------

void PinPlatform::write(bool gHigh) {
   //TODO (BT) implement for avr
}

//-------------------------------------------------------------------------------------------------

bool PinPlatform::read() {
   //TODO (BT) implement for avr
   return false;
}

//-------------------------------------------------------------------------------------------------

} // namespace Mcu
} // namespace Bt
