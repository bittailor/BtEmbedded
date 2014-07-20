//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::TimeoutPlatform
//  
//*************************************************************************************************

#include "Bt/Util/TimeoutPlatform.hpp"

namespace Bt {
namespace Util {


//-------------------------------------------------------------------------------------------------

TimeoutPlatform::TimeoutPlatform(unsigned int iMilliseconds) : mDuration(iMilliseconds), mStart(std::chrono::high_resolution_clock::now()) {

}

//-------------------------------------------------------------------------------------------------

TimeoutPlatform::~TimeoutPlatform() {

}

//-------------------------------------------------------------------------------------------------

bool TimeoutPlatform::check() const {
   return (std::chrono::high_resolution_clock::now() - mStart) > mDuration;
}

//-------------------------------------------------------------------------------------------------

} // namespace Util
} // namespace Bt
