//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfNodeTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Rf24/RfNode.hpp"

namespace Bt {
namespace Rf24 {

//-------------------------------------------------------------------------------------------------

::testing::AssertionResult checkLevel(const RfNode& pNodeId, uint8_t level) {
  if (pNodeId.level() == level)
    return ::testing::AssertionSuccess();
  else
    return ::testing::AssertionFailure() << "level() of " << (int)pNodeId.id() << " returned " <<  (int)pNodeId.level() << " and not " << (int)level ;
}


TEST(RfNodeTest, verifyLevel0) {
   checkLevel(0,0);
}

TEST(RfNodeTest, verifyLevel1) {
   for(uint8_t i = 1 ; i <= 5 ; i++ ) {
      checkLevel(i,1);
   }
}

TEST(RfNodeTest, verifyLevel2) {
   for(uint8_t i = 6 ; i <= 30 ; i++ ) {
      checkLevel(i,2);
   }
}

TEST(RfNodeTest, verifyLevel3) {
   for(uint8_t i = 31 ; i <= 155 ; i++ ) {
      checkLevel(i,3);
   }
}

TEST(RfNodeTest, verifyLevel4) {
   for(uint16_t i = 156 ; i <= 255 ; i++ ) {
      checkLevel(i,4);
   }
}

//-------------------------------------------------------------------------------------------------

} // namespace Rf24
} // namespace Bt
