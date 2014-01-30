//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Timing
//  
//*************************************************************************************************

#include "Bt/Util/Timing.hpp"

#include <chrono>
#include <thread>

namespace Bt {
namespace Util {

namespace {
std::chrono::high_resolution_clock::time_point sEpoch = std::chrono::high_resolution_clock::now();
} // namespace

//-------------------------------------------------------------------------------------------------

void delayInMilliseconds(uint32_t milliseconds) {
   std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

//-------------------------------------------------------------------------------------------------

void delayInMicroseconds(uint32_t microseconds) {
   std::this_thread::sleep_for(std::chrono::microseconds(microseconds));
}

//-------------------------------------------------------------------------------------------------

uint32_t milliseconds() {
   return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - sEpoch).count();
}

//-------------------------------------------------------------------------------------------------

uint32_t microseconds() {
   return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - sEpoch).count();
}

//-------------------------------------------------------------------------------------------------

} // namespace Util
} // namespace Bt
