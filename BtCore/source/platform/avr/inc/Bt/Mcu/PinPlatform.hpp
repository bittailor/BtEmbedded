//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::PinPlatform
//  
//*************************************************************************************************

#ifndef INC__Bt_Mcu_PinPlatform__hpp
#define INC__Bt_Mcu_PinPlatform__hpp

#include <stdint.h>

#include "Bt/Mcu/I_Pin.hpp"

namespace Bt {
namespace Mcu {

class PinPlatform 
{
   public:

      PinPlatform(uint8_t pPinId, I_Pin::Mode pInitialMode);


      virtual void mode(I_Pin::Mode pMode);

      virtual void write(bool gHigh);

      virtual bool read();

   
   private:
   	  // Constructor to prohibit copy construction.
      PinPlatform(const PinPlatform&);

      // Operator= to prohibit copy assignment
      PinPlatform& operator=(const PinPlatform&);

      const uint8_t mPinId;
};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_PinPlatform__hpp
