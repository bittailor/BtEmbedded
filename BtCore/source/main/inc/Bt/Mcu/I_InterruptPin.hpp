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

};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_I_InterruptPin__hpp
