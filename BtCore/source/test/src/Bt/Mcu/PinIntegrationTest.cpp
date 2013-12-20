//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::PinIntegrationTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Mcu/Pin.hpp"

#include <bcm2835.h>

namespace Bt {
namespace Mcu {


/**
 * This test need to have two pins connected by a wire !
 *
 * Raspberry Pi:
 *    22 <=> 23
 *
 */
class PinIntegrationTest : public ::testing::Test {
   
   protected:
      
      PinIntegrationTest()
         : mOutputPin(22,I_Pin::MODE_OUTPUT)
         , mInputPin(23,I_Pin::MODE_INPUT) {

      }

      virtual void SetUp() {
      }

      virtual void TearDown() {
         
      }

      Pin mOutputPin;
      Pin mInputPin;
};

TEST_F(PinIntegrationTest, low) {
   mOutputPin.write(false);
   ASSERT_FALSE(mInputPin.read());
}

TEST_F(PinIntegrationTest, high) {
   mOutputPin.write(true);
   ASSERT_TRUE(mInputPin.read());
}

TEST_F(PinIntegrationTest, toggle) {
   mOutputPin.write(true);
   ASSERT_TRUE(mInputPin.read());
   mOutputPin.write(false);
   ASSERT_FALSE(mInputPin.read());
   mOutputPin.write(true);
   ASSERT_TRUE(mInputPin.read());
   mOutputPin.write(false);
   ASSERT_FALSE(mInputPin.read());
}

TEST_F(PinIntegrationTest, mode) {
   mOutputPin.mode(I_Pin::MODE_INPUT);
   mInputPin.mode(I_Pin::MODE_OUTPUT);
   mInputPin.write(true);
   ASSERT_TRUE(mOutputPin.read());
   mInputPin.write(false);
   ASSERT_FALSE(mOutputPin.read());
   mInputPin.write(true);
   ASSERT_TRUE(mOutputPin.read());
   mInputPin.write(false);
   ASSERT_FALSE(mOutputPin.read());
}

} // namespace Mcu
} // namespace Bt
