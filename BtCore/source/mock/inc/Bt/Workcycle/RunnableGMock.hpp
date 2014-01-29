//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Workcycle::RunnableGMock
//  
//*************************************************************************************************

#ifndef INC__Bt_Workcycle_RunnableGMock__hpp
#define INC__Bt_Workcycle_RunnableGMock__hpp

#include <gmock/gmock.h>

#include "Bt/Workcycle/I_Runnable.hpp"

namespace Bt {
namespace Workcycle {

class RunnableGMock : public I_Runnable {
   public:
      MOCK_METHOD0(workcycle,void ());
};

} // namespace Workcycle
} // namespace Bt

#endif // INC__Bt_Workcycle_RunnableGMock__hpp
