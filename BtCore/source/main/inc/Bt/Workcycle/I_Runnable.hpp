//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Workcycle::I_Runnable
//  
//*************************************************************************************************

#ifndef INC__Bt_Workcycle_I_Runnable__hpp
#define INC__Bt_Workcycle_I_Runnable__hpp

#include "Bt/Util/IntrusiveList.hpp"

namespace Bt {
namespace Workcycle {

class I_Runnable : public Util::IntrusiveList<I_Runnable>::Element {
   public:
      virtual ~I_Runnable() {}
      
      virtual void workcycle() = 0;
};

} // namespace Workcycle
} // namespace Bt

#endif // INC__Bt_Workcycle_I_Runnable__hpp
