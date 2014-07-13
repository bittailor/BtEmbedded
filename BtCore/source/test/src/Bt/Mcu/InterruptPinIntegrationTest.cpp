//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::InterruptPinIntegrationTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include <future>

#include "Bt/Mcu/InterruptPin.hpp"
#include "Bt/Mcu/Pin.hpp"
#include "Bt/Util/Timeout.hpp"

namespace Bt {
namespace Mcu {


/**
 * This test need to have two pins connected by a wire !
 *
 * Raspberry Pi:
 *    22 <=> 23
 *
 */
class InterruptPinIntegrationTest : public ::testing::Test {
   
   protected:

      

      virtual void SetUp() {
      }

      virtual void TearDown() {
         
      }
};


TEST_F(InterruptPinIntegrationTest, failingInterrupt) {
   Pin outputPin(22,I_Pin::MODE_OUTPUT);
   InterruptPin interruptPin(23, I_InterruptPin::Edge::FALLING);
   outputPin.write(true);


   std::promise<std::chrono::time_point<std::chrono::high_resolution_clock>> promise;
   auto future = promise.get_future();
   interruptPin.enable([&promise] {
      promise.set_value(std::chrono::high_resolution_clock::now());
   });

   {
      auto state = future.wait_for(std::chrono::milliseconds(100));
      ASSERT_EQ(std::future_status::timeout, state);
   }

   auto start = std::chrono::high_resolution_clock::now();
   outputPin.write(false);
   auto state = future.wait_for(std::chrono::seconds(1));

   ASSERT_EQ(std::future_status::ready, state);
   std::cout << "wait for interrupt took " << std::chrono::duration_cast<std::chrono::microseconds >(future.get() - start).count() << " microseconds" << std::endl;

   interruptPin.disable();
}

TEST_F(InterruptPinIntegrationTest, multipleFailingInterrupt) {
   Pin outputPin(22,I_Pin::MODE_OUTPUT);
   InterruptPin interruptPin(23, I_InterruptPin::Edge::FALLING);
   outputPin.write(true);


   std::atomic<int> counter(0);

   interruptPin.enable([&counter] {
      counter++;
   });

   int loop = 10;

   for (int i = 0 ; i < loop ; i++) {
      outputPin.write(false);
      Util::Timeout timeout(1000);
      while(counter.load() <= i){
         ASSERT_FALSE(timeout.check());
         std::this_thread::yield();
      }
      outputPin.write(true);
      outputPin.write(false);
   }

   ASSERT_EQ(loop, counter.load());
}

TEST_F(InterruptPinIntegrationTest, enableDisableWithoutInterruot) {
   Pin outputPin(22,I_Pin::MODE_OUTPUT);
   InterruptPin interruptPin(23, I_InterruptPin::Edge::FALLING);
   outputPin.write(true);

   std::atomic<int> counter(0);

   interruptPin.enable([&counter] {
      counter++;
   });

   std::this_thread::sleep_for(std::chrono::seconds(2));

   interruptPin.disable();
   ASSERT_EQ(0, counter.load());
}


} // namespace Mcu
} // namespace Bt
