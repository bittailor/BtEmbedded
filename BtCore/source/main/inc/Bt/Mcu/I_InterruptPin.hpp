//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::I_InterruptPin
//  
//*************************************************************************************************

#ifndef INC__Bt_Mcu_I_InterruptPin__hpp
#define INC__Bt_Mcu_I_InterruptPin__hpp

#include <functional>

namespace Bt {
namespace Mcu {

class I_InterruptPin {
   public:
      enum class Edge
      {
         FALLING,
         RISING,
         BOTH
      };

      virtual ~I_InterruptPin() {}

      virtual void enable(std::function<void()> iInterruptHandler) = 0;
      virtual void disable() = 0;

      virtual bool read() =0;

};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_I_InterruptPin__hpp
