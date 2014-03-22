//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Concurrency::BlockingQueueTest
//  
//*************************************************************************************************

#include <chrono>
#include <gtest/gtest.h>

#include "Bt/Concurrency/BlockingQueue.hpp"


namespace Bt {
namespace Concurrency {

class BlockingQueueTest : public ::testing::Test {
   
   protected:
      
      virtual void SetUp() {
      }

      virtual void TearDown() {
         
      }

      BlockingQueue<int> mBlockingQueue;

};


TEST_F(BlockingQueueTest, emptyOnEmptyReturnsTrue) {
   EXPECT_TRUE(mBlockingQueue.empty());
}

TEST_F(BlockingQueueTest, emptyOnNonEmptyReturnsFalse) {
   mBlockingQueue.push(1977);
   EXPECT_FALSE(mBlockingQueue.empty());
}

TEST_F(BlockingQueueTest, tryPopOnEmptyReturnFalse) {
   int value;
   EXPECT_FALSE(mBlockingQueue.tryPop(value));
}

TEST_F(BlockingQueueTest, tryPopOnNonEmptyReturnTrue) {
   mBlockingQueue.push(1977);
   int value;
   EXPECT_TRUE(mBlockingQueue.tryPop(value));
}

TEST_F(BlockingQueueTest, tryPopOnNonEmptySetsValue) {
   mBlockingQueue.push(1977);
   int value = 0;
   mBlockingQueue.tryPop(value);
   EXPECT_EQ(1977, value);
}

TEST_F(BlockingQueueTest, popOnNonEmptySetsValue) {
   mBlockingQueue.push(1977);
   int value = 0;
   mBlockingQueue.pop(value);
   EXPECT_EQ(1977, value);
}

TEST_F(BlockingQueueTest, pushPopOrder) {
   mBlockingQueue.push(1977);
   mBlockingQueue.push(2000);
   mBlockingQueue.push(2014);
   int value = 0;
   mBlockingQueue.pop(value);
   EXPECT_EQ(1977, value);
   mBlockingQueue.pop(value);
   EXPECT_EQ(2000, value);
   mBlockingQueue.pop(value);
   EXPECT_EQ(2014, value);
}

TEST_F(BlockingQueueTest, tryPopWithTimeoutOnEmptyReturnsFalse) {
   int value = 0;
   EXPECT_FALSE(mBlockingQueue.tryPop(value, std::chrono::milliseconds(1)));
}

TEST_F(BlockingQueueTest, tryPopWithTimeoutOnEmptyWaits) {
   int value = 0;
   auto start = std::chrono::system_clock::now();
   mBlockingQueue.tryPop(value, std::chrono::milliseconds(10));
   std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
   EXPECT_GE(duration,std::chrono::milliseconds(10));

}

TEST_F(BlockingQueueTest, tryPopWithTimeoutOnNonEmptyDoesNotWait) {
   mBlockingQueue.push(1977);
   int value = 0;
   auto start = std::chrono::system_clock::now();
   mBlockingQueue.tryPop(value, std::chrono::seconds(10));
   std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start);
   EXPECT_LE(duration,std::chrono::milliseconds(10));

}


} // namespace Concurrency
} // namespace Bt
