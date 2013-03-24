//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::SpiTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Mcu/Spi.hpp"
#include "Bt/Mcu/PinGMock.hpp"

namespace Bt {
namespace Mcu {

class SpiTest : public ::testing::Test {
   
   protected:
      
      SpiTest(): mSpi(Spi::BIT_ORDER_MSBFIRST, Spi::MODE_0, Spi::SPEED_4_MHZ, mChipSelect) {
      }

      virtual void SetUp() {
      }

      virtual void TearDown() {
         
      }

      PinGMock mChipSelect;
      Spi mSpi;

};

TEST_F(SpiTest, CheckTransfer) {
   EXPECT_EQ(0,0);
}

} // namespace Mcu
} // namespace Bt
