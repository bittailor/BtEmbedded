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

#include "Bt/Mcu/IPin.hpp"
#include "Bt/Mcu/GpioLibrary.hpp"

using Bt::Mcu::IPin;


namespace {
   bcm2835FunctionSelect translateMode(IPin::Mode pMode) {
      switch (pMode) {
         case IPin::MODE_INPUT : return BCM2835_GPIO_FSEL_INPT;
         case IPin::MODE_OUTPUT : return BCM2835_GPIO_FSEL_OUTP;
      }
   }
}

namespace Bt {
namespace Mcu {


//-------------------------------------------------------------------------------------------------

PinPlatform::PinPlatform(uint8_t pPinId, IPin::Mode pInitialMode) : mPinId(pPinId) {
   GpioLibrary::ensureIsInitialized();
   mode(pInitialMode);
}

//-------------------------------------------------------------------------------------------------

void PinPlatform::mode(IPin::Mode pMode) {
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
