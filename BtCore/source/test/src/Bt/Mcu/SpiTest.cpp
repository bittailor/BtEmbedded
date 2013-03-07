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

namespace Bt {
namespace Mcu {

class SpiTest : public ::testing::Test {
   
   protected:
      
      virtual void SetUp() {
      }

      virtual void TearDown() {
         
      }

      Spi mSpi;

};

TEST_F(SpiTest, CheckTransfer) {
   EXPECT_EQ(0,0);
}


} // namespace Mcu
} // namespace Bt
