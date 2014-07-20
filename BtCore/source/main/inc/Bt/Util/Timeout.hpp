//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Timeout
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_Timeout__hpp
#define INC__Bt_Util_Timeout__hpp

#include <stdint.h>
#include "Bt/Util/TimeoutPlatform.hpp"

namespace Bt {
namespace Util {

class Timeout
{


   public:
      Timeout(uint32_t iMilliseconds);
      ~Timeout();

      bool check();

      bool lastChekTimedOut() const;
   
   private:
      bool mTimedOut;
      uint32_t mMilliseconds;
      uint32_t mStart;

};

} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_Timeout__hpp
