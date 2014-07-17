//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::InterruptPin
//  
//*************************************************************************************************

#ifndef INC__Bt_Mcu_InterruptPin__hpp
#define INC__Bt_Mcu_InterruptPin__hpp

#include <Bt/Mcu/I_InterruptPin.hpp>
#include <Bt/Mcu/InterruptPinPlatform.hpp>

#include <cstdint>

namespace Bt {
namespace Mcu {

class InterruptPin :  public I_InterruptPin, private InterruptPinPlatform
{
   public:
      InterruptPin(uint8_t iPinId, Edge iEdge);
      virtual ~InterruptPin();

      virtual void enable(std::function<void()> iInterruptHandler);
      virtual void disable();

      virtual bool read();

   
   private:
   	  // Constructor to prohibit copy construction.
      InterruptPin(const InterruptPin&);

      // Operator= to prohibit copy assignment
      InterruptPin& operator=(const InterruptPin&);
};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_InterruptPin__hpp
