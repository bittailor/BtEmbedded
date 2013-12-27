//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Delay
//  
//*************************************************************************************************

#include "Bt/Util/Delay.hpp"

#include <chrono>
#include <thread>

namespace Bt {
namespace Util {


//-------------------------------------------------------------------------------------------------

void delayInMilliseconds(unsigned int milliseconds) {
   std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

//-------------------------------------------------------------------------------------------------

void delayInMicroseconds(unsigned int microseconds) {
   std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}

//-------------------------------------------------------------------------------------------------


} // namespace Util
} // namespace Bt