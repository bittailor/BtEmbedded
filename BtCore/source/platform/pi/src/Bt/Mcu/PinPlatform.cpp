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
   bcm2835FunctionSelect translateMode(I_Pin::Mode iMode) {
      switch (iMode) {
         case I_Pin::MODE_INPUT : return BCM2835_GPIO_FSEL_INPT;
         case I_Pin::MODE_OUTPUT : return BCM2835_GPIO_FSEL_OUTP;
         default : return BCM2835_GPIO_FSEL_OUTP;
      }
   }
}

namespace Bt {
namespace Mcu {


//-------------------------------------------------------------------------------------------------

PinPlatform::PinPlatform(uint8_t iPinId, I_Pin::Mode iInitialMode) : mPinId(iPinId) {
   GpioLibrary::ensureIsInitialized();
   mode(iInitialMode);
}

//-------------------------------------------------------------------------------------------------

void PinPlatform::mode(I_Pin::Mode iMode) {
   bcm2835_gpio_fsel(mPinId, translateMode(iMode));
}

//-------------------------------------------------------------------------------------------------

void PinPlatform::write(bool iHigh) {
   bcm2835_gpio_write(mPinId,iHigh);
}

//-------------------------------------------------------------------------------------------------

bool PinPlatform::read() {
   return bcm2835_gpio_lev(mPinId) == HIGH;
}

//-------------------------------------------------------------------------------------------------

} // namespace Mcu
} // namespace Bt
