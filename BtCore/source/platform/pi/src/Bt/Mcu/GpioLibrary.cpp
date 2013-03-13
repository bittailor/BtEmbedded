//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::GpioLibrary
//  
//*************************************************************************************************

#include "Bt/Mcu/GpioLibrary.hpp"

#include <iostream>

#include <bcm2835.h>

namespace Bt {
namespace Mcu {


//-------------------------------------------------------------------------------------------------

std::atomic<bool> GpioLibrary::sIsInitialized(false);

//-------------------------------------------------------------------------------------------------

void GpioLibrary::ensureIsInitialized() {

   bool expected = false;
   if (sIsInitialized.compare_exchange_strong(expected,true)) {
      if(!bcm2835_init()) {
         std::cerr << "Failed to initialize GPIO library (bcm2835)" << std::endl;
      }
   }
}

//-------------------------------------------------------------------------------------------------

} // namespace Mcu
} // namespace Bt
