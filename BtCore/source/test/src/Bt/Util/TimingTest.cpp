//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::TimingTest
//  
//*************************************************************************************************

#include <thread>
#include <pthread.h>
#include <gtest/gtest.h>

#include "Bt/Util/Timing.hpp"

namespace Bt {
namespace Util {


TEST(TimingTest, milliseconds) {
   uint32_t delay = 123;

   uint32_t start = Bt::Util::milliseconds();
   Bt::Util::delayInMilliseconds(delay);
   uint32_t stop = Bt::Util::milliseconds();

   uint32_t duration = stop - start;

   ASSERT_LE(delay, duration);
}



TEST(TimingTest, microseconds) {

   uint32_t delay = 876;
   uint32_t start = Bt::Util::microseconds();
   Bt::Util::delayInMicroseconds(delay);
   uint32_t stop = Bt::Util::microseconds();

   uint32_t duration = stop - start;

   ASSERT_LE(delay, duration);
}


} // namespace Util
} // namespace Bt
