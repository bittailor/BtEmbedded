//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::PinPlatform
//  
//*************************************************************************************************

#include <Arduino.h>

#include "Bt/Mcu/PinPlatform.hpp"
#include "Bt/Mcu/I_Pin.hpp"


namespace Bt {
namespace Mcu {

namespace {

uint8_t translateMode(I_Pin::Mode pMode) {
   switch (pMode) {
      case I_Pin::MODE_INPUT : return INPUT;
      case I_Pin::MODE_OUTPUT : return OUTPUT;
      default : return OUTPUT;
   }
}

}

//-------------------------------------------------------------------------------------------------

PinPlatform::PinPlatform(uint8_t pPinId, I_Pin::Mode pInitialMode) : mPinId(pPinId) {
   mode(pInitialMode);
}

//-------------------------------------------------------------------------------------------------

void PinPlatform::mode(I_Pin::Mode pMode) {
   pinMode(mPinId, translateMode(pMode));
}

//-------------------------------------------------------------------------------------------------

void PinPlatform::write(bool gHigh) {
   digitalWrite(mPinId, gHigh ? HIGH : LOW);
}

//-------------------------------------------------------------------------------------------------

bool PinPlatform::read() {
   return digitalRead(mPinId);
}

//-------------------------------------------------------------------------------------------------

} // namespace Mcu
} // namespace Bt
