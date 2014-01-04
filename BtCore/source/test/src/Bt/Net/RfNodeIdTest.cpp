//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RfNodeIdTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Net/RfNodeId.hpp"

namespace Bt {
namespace Net {

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

::testing::AssertionResult checkLevel(const RfNodeId& pNodeId, uint8_t level) {
  if (pNodeId.level() == level)
    return ::testing::AssertionSuccess();
  else
    return ::testing::AssertionFailure() << "level() of " << (int)pNodeId.id() << " returned " <<  (int)pNodeId.level() << " and not " << (int)level ;
}


TEST(RfNodeIdTest, verifyLevel0) {
   checkLevel(0,0);
}

TEST(RfNodeIdTest, verifyLevel1) {
   for(uint8_t i = 1 ; i <= 5 ; i++ ) {
      checkLevel(i,1);
   }
}

TEST(RfNodeIdTest, verifyLevel2) {
   for(uint8_t i = 6 ; i <= 30 ; i++ ) {
      checkLevel(i,2);
   }
}

TEST(RfNodeIdTest, verifyLevel3) {
   for(uint8_t i = 31 ; i <= 155 ; i++ ) {
      checkLevel(i,3);
   }
}

TEST(RfNodeIdTest, verifyLevel4) {
   for(uint16_t i = 156 ; i <= 255 ; i++ ) {
      checkLevel(i,4);
   }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

::testing::AssertionResult isDirectChild(const RfNodeId& pParent, const RfNodeId& pChild) {
  if (pParent.isDirectChild(pChild))
    return ::testing::AssertionSuccess() << (int)pChild.id() << " is direct child of " <<  (int) pParent.id();
  else
    return ::testing::AssertionFailure() << (int)pChild.id() << " is no direct child of " <<  (int) pParent.id();
}

TEST(RfNodeIdTest, isDirectChildOfNode0ToSelf) {
   RfNodeId nodeId(0);

   EXPECT_FALSE(isDirectChild(nodeId,0));
}

TEST(RfNodeIdTest, isDirectChildOfNode0ToDirectChild) {
   RfNodeId nodeId(0);

   for(uint16_t i = 1 ; i <= 255 ; i++ ) {
      EXPECT_TRUE(isDirectChild(nodeId,i));
   }
}

TEST(RfNodeIdTest, isDirectChildOfNode2ToDirectChild) {
   RfNodeId nodeId(2);

   for(uint8_t i = 11 ; i <= 15 ; i++ ) {
      EXPECT_TRUE(isDirectChild(nodeId,i));
   }

   for(uint8_t i = 56 ; i <= 80 ; i++ ) {
         EXPECT_TRUE(isDirectChild(nodeId,i));
   }

}

TEST(RfNodeIdTest, isDirectChildOfNode2ToNonDirectChild) {
   RfNodeId nodeId(2);

   for(uint8_t i = 0 ; i <= 10 ; i++ ) {
      EXPECT_FALSE(isDirectChild(nodeId,i));
   }

   for(uint8_t i = 16 ; i <= 55 ; i++ ) {
      EXPECT_FALSE(isDirectChild(nodeId,i));
   }

   for(uint16_t i = 81 ; i <= 255 ; i++ ) {
      EXPECT_FALSE(isDirectChild(nodeId,i));
   }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------


} // namespace Net
} // namespace Bt
