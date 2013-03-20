//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::Pin
//  
//*************************************************************************************************

#ifndef INC__Bt_Mcu_Pin__hpp
#define INC__Bt_Mcu_Pin__hpp

#include "Bt/Mcu/IPin.hpp"
#include "Bt/Mcu/PinPlatform.hpp"

namespace Bt {
namespace Mcu {

class Pin : public IPin, private PinPlatform
{
   public:
      Pin(uint8_t pPinId, Mode pInitialMode);
      ~Pin();

      virtual void mode(Mode pMode);

      virtual void write(bool pHigh);

      virtual bool read();

   private:
   	  // Constructor to prohibit copy construction.
      Pin(const Pin&);

      // Operator= to prohibit copy assignment
      Pin& operator=(const Pin&);
};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_Pin__hpp
