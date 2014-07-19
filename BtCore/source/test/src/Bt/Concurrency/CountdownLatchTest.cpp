//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Concurrency::CountdownLatchTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include <future>

#include "Bt/Concurrency/CountdownLatch.hpp"

namespace  Bt {
namespace Concurrency {

class CountdownLatchTest : public ::testing::Test {
};

bool WaitLatch(CountdownLatch& iLatch) {
   iLatch.wait();
   return true;
}

bool WaitForLatch(CountdownLatch& iLatch, std::chrono::milliseconds iTimeout) {
   return iLatch.waitFor(iTimeout);
}

TEST_F(CountdownLatchTest, TwoThreadsWaitLatchOfOne) {
   CountdownLatch latch(1);
   auto futureOne = std::async(std::launch::async, WaitLatch, std::ref(latch));
   auto futureTwo = std::async(std::launch::async, WaitLatch, std::ref(latch));

   latch.countDown();

   ASSERT_EQ(std::future_status::ready, futureOne.wait_for(std::chrono::seconds(1)));
   ASSERT_EQ(std::future_status::ready, futureTwo.wait_for(std::chrono::seconds(1)));

}

TEST_F(CountdownLatchTest, TwoThreadsWaitLatchAlreadyZero) {
   CountdownLatch latch(1);
   latch.countDown();

   auto futureOne = std::async(std::launch::async, WaitLatch, std::ref(latch));
   auto futureTwo = std::async(std::launch::async, WaitLatch, std::ref(latch));

   ASSERT_EQ(std::future_status::ready, futureOne.wait_for(std::chrono::seconds(1)));
   ASSERT_EQ(std::future_status::ready, futureTwo.wait_for(std::chrono::seconds(1)));

}

TEST_F(CountdownLatchTest, OneThreadsWaitLatchOfTwo) {
   CountdownLatch latch(2);
   auto futureOne = std::async(std::launch::async, WaitLatch, std::ref(latch));

   ASSERT_EQ(std::future_status::timeout, futureOne.wait_for(std::chrono::milliseconds(100)));

   latch.countDown();

   ASSERT_EQ(std::future_status::timeout, futureOne.wait_for(std::chrono::milliseconds(100)));

   latch.countDown();

   ASSERT_EQ(std::future_status::ready, futureOne.wait_for(std::chrono::seconds(1)));
}

TEST_F(CountdownLatchTest, OneThreadsWaitForLatchOfTwo) {
   CountdownLatch latch(2);

   auto futureFirst = std::async(std::launch::async, WaitForLatch, std::ref(latch), std::chrono::milliseconds(100));
   ASSERT_FALSE(futureFirst.get());

   auto futureSecond = std::async(std::launch::async, WaitForLatch, std::ref(latch), std::chrono::milliseconds(100));
   latch.countDown();
   ASSERT_FALSE(futureSecond.get());

   auto futureThird = std::async(std::launch::async, WaitForLatch, std::ref(latch), std::chrono::milliseconds(100));
   latch.countDown();
   ASSERT_TRUE(futureThird.get());
}


} // namespace Concurrency
} // namespace Bt
