//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::NRf24L01PTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Device/NRf24L01P.hpp"

namespace Bt {
namespace Device {

class NRf24L01PTest : public ::testing::Test {
   
   protected:
      
      virtual void SetUp() {
      }

      virtual void TearDown() {
         
      }

      NRf24L01P mNRf24L01P;

};


TEST_F(NRf24L01PTest, testcase) {
   EXPECT_EQ(1,1);
}


} // namespace Device
} // namespace Bt
