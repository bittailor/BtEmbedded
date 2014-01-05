//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfPipesTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Bt/Rf24/RfPipe.hpp"

namespace Bt {
namespace Rf24 {

TEST(RfPipesTest, All) {
   ASSERT_THAT(RfPipes::ALL_PIPES, testing::ElementsAre(
      RfPipe::PIPE_0,
      RfPipe::PIPE_1,
      RfPipe::PIPE_2,
      RfPipe::PIPE_3,
      RfPipe::PIPE_4,
      RfPipe::PIPE_5
   ));
}

TEST(RfPipesTest, numberOfPipes) {
   EXPECT_EQ(6, RfPipes::NUMBER_OF_PIPES);
}


} // namespace Rf24
} // namespace Bt
