//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Workcycle::I_Workcycle
//  
//*************************************************************************************************

#ifndef INC__Bt_Workcycle_I_Workcycle__hpp
#define INC__Bt_Workcycle_I_Workcycle__hpp

#include "Bt/Workcycle/I_Runnable.hpp"

namespace Bt {
namespace Workcycle {

class I_Workcycle {
   public:
      virtual ~I_Workcycle() {}
      
      virtual void add(I_Runnable& iRunnable) = 0;
      virtual void remove(I_Runnable& iRunnable) = 0;
};

} // namespace Workcycle
} // namespace Bt

#endif // INC__Bt_Workcycle_I_Workcycle__hpp
