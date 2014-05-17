//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::InterruptPinPlatform
//  
//*************************************************************************************************

#ifndef INC__Bt_Mcu_InterruptPinPlatform__hpp
#define INC__Bt_Mcu_InterruptPinPlatform__hpp



#include <Bt/Mcu/I_InterruptPin.hpp>

namespace Bt {
namespace Mcu {

class InterruptPinPlatform 
{
   public:
      InterruptPinPlatform(uint8_t iPinId, I_InterruptPin::Edge iEdge);
      ~InterruptPinPlatform();
   
      void enable(std::function<void()> iInterruptHandler);
      void disable();

   private:
   	  // Constructor to prohibit copy construction.
      InterruptPinPlatform(const InterruptPinPlatform&);

      // Operator= to prohibit copy assignment
      InterruptPinPlatform& operator=(const InterruptPinPlatform&);

};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_InterruptPinPlatform__hpp
