//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Timeout
//  
//*************************************************************************************************

#include <Bt/Log/Logging.hpp>
#include "Bt/Util/Timeout.hpp"
#include "Bt/Util/Timing.hpp"

namespace Bt {
namespace Util {


//-------------------------------------------------------------------------------------------------

Timeout::Timeout(uint32_t iMilliseconds) : mTimedOut(false),  mMilliseconds(iMilliseconds), mStart(milliseconds()){


}

//-------------------------------------------------------------------------------------------------

Timeout::~Timeout() {

}

//-------------------------------------------------------------------------------------------------

bool Timeout::check() {
   if (mTimedOut) {
      return true;
   }

   uint32_t up = milliseconds() - mStart;
   //BT_LOG(DEBUG) << "timeout check - " << up << " > " << mMilliseconds;
   if (up > mMilliseconds) {
      mTimedOut = true;
      return true;
   }

   return false;
}

//-------------------------------------------------------------------------------------------------

bool Timeout::lastChekTimedOut() const {
   return mTimedOut;
}

//-------------------------------------------------------------------------------------------------

} // namespace Util
} // namespace Bt
