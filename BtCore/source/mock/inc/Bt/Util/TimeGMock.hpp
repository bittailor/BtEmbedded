//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::TimeGMock
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_TimeGMock__hpp
#define INC__Bt_Util_TimeGMock__hpp

#include <gmock/gmock.h>

#include "Bt/Util/I_Time.hpp"

namespace Bt {
namespace Util {

class TimeGMock : public I_Time {
   public:

      MOCK_METHOD0(microseconds, unsigned long ());
};

} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_TimeGMock__hpp
