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

#include <bcm2835.h>

#include "Bt/Mcu/I_Pin.hpp"
#include "Bt/Mcu/GpioLibrary.hpp"

using Bt::Mcu::I_Pin;


namespace {
   bcm2835FunctionSelect translateMode(I_Pin::Mode pMode) {
      switch (pMode) {
         case I_Pin::MODE_INPUT : return BCM2835_GPIO_FSEL_INPT;
         case I_Pin::MODE_OUTPUT : return BCM2835_GPIO_FSEL_OUTP;
      }
   }
}

namespace Bt {
namespace Mcu {


//-------------------------------------------------------------------------------------------------

PinPlatform::PinPlatform(uint8_t pPinId, I_Pin::Mode pInitialMode) : mPinId(pPinId) {
   GpioLibrary::ensureIsInitialized();
   mode(pInitialMode);
}

//-------------------------------------------------------------------------------------------------

void PinPlatform::mode(I_Pin::Mode pMode) {
   bcm2835_gpio_fsel(mPinId, translateMode(pMode));
}

//-------------------------------------------------------------------------------------------------

void PinPlatform::write(bool gHigh) {
   bcm2835_gpio_write(mPinId,gHigh);
}

//-------------------------------------------------------------------------------------------------

bool PinPlatform::read() {
   bcm2835_gpio_lev(mPinId) == HIGH;
}

//-------------------------------------------------------------------------------------------------

} // namespace Mcu
} // namespace Bt
