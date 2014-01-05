//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RfNetworkRoutingAlgorithmTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Net/RfNetworkRoutingAlgorithm.hpp"

namespace Bt {
namespace Net {

class RfNetworkRoutingAlgorithmTest : public ::testing::Test {
   
   protected:
      
      virtual void SetUp() {
      }

      virtual void TearDown() {
         
      }

      RfNetworkRoutingAlgorithm mRfNetworkRoutingAlgorithm;

};

TEST_F(RfNetworkRoutingAlgorithmTest, routingFromTopNode) {
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_1, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(0, 1));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_2, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(0, 2));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_3, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(0, 3));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_4, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(0, 4));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_5, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(0, 5));
}

TEST_F(RfNetworkRoutingAlgorithmTest, routingToParentWhenDestinationAtLowerLevel) {
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 0));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 1));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 2));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 3));
}

TEST_F(RfNetworkRoutingAlgorithmTest, routingToParentWhenDestinationAtSameLevel) {
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 6));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 7));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 10));

   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 11));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 13));

   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 16));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 20));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 30));
}

TEST_F(RfNetworkRoutingAlgorithmTest, routingToParentWhenHigherLevelButNotChild) {
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 31));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 32));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 35));

   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 66));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 70));

   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_0, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 176));
}

TEST_F(RfNetworkRoutingAlgorithmTest, routingToChildOneLevel) {
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_1, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 61));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_2, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 62));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_3, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 63));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_4, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 64));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_5, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(12, 65));
}

TEST_F(RfNetworkRoutingAlgorithmTest, routingToChildMultipleLevel) {
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_1, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 181));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_1, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 182));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_1, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 183));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_1, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 184));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_1, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 185));

   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_2, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 186));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_2, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 187));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_2, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 188));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_2, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 189));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_2, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 190));

   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_3, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 191));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_3, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 192));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_3, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 193));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_3, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 194));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_3, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 195));

   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_4, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 196));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_4, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 197));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_4, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 198));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_4, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 199));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_4, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 200));

   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_5, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 201));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_5, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 202));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_5, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 203));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_5, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 204));
   EXPECT_EQ(Rf24::I_RfController::Pipe::PIPE_5, mRfNetworkRoutingAlgorithm.calculateRoutingPipe(7, 205));
}


} // namespace Net
} // namespace Bt
