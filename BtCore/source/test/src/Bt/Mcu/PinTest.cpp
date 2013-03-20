//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::PinTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Mcu/Pin.hpp"

#include <bcm2835.h>

namespace Bt {
namespace Mcu {


/**
 * This test need to have the to pins connected by a wire !
 */
class PinTest : public ::testing::Test {
   
   protected:
      
      PinTest()
         : mOutputPin(22,IPin::MODE_OUTPUT)
         , mInputPin(23,IPin::MODE_INPUT) {

      }

      virtual void SetUp() {
      }

      virtual void TearDown() {
         
      }

      Pin mOutputPin;
      Pin mInputPin;
};

TEST_F(PinTest, low) {
   mOutputPin.write(false);
   ASSERT_FALSE(mInputPin.read());
}

TEST_F(PinTest, high) {
   mOutputPin.write(true);
   ASSERT_TRUE(mInputPin.read());
}

TEST_F(PinTest, toggle) {
   mOutputPin.write(true);
   ASSERT_TRUE(mInputPin.read());
   mOutputPin.write(false);
   ASSERT_FALSE(mInputPin.read());
   mOutputPin.write(true);
   ASSERT_TRUE(mInputPin.read());
   mOutputPin.write(false);
   ASSERT_FALSE(mInputPin.read());
}

TEST_F(PinTest, mode) {
   mOutputPin.mode(IPin::MODE_INPUT);
   mInputPin.mode(IPin::MODE_OUTPUT);
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
