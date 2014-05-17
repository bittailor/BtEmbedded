//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Timeout
//  
//*************************************************************************************************

#include "Bt/Util/Timeout.hpp"
#include "Bt/Util/Timing.hpp"

namespace Bt {
namespace Util {


//-------------------------------------------------------------------------------------------------

Timeout::Timeout(uint32_t pMilliseconds) : mTimedOut(false),  mMilliseconds(pMilliseconds), mStart(milliseconds()){


}

//-------------------------------------------------------------------------------------------------

Timeout::~Timeout() {

}

//-------------------------------------------------------------------------------------------------

Timeout::operator bool() const {
   if (mTimedOut) {
      return true;
   }

   if ((milliseconds() - mStart) > mMilliseconds) {
      mTimedOut = true;
      return true;
   }

   return false;
}


} // namespace Util
} // namespace Bt
