//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Workcycle::PeriodicWorkcycleTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Workcycle/PeriodicWorkcycle.hpp"
#include "Bt/Util/TimeGMock.hpp"
#include "Bt/Workcycle/RunnableGMock.hpp"

using namespace testing;

namespace Bt {
namespace Workcycle {

class PeriodicWorkcycleTest : public ::testing::Test {
   
   protected:
      
      PeriodicWorkcycleTest(): mPeriodicWorkcycle(1000,mTime) {

      }

      virtual void SetUp() {
      }

      virtual void TearDown() {
         
      }

      Util::TimeGMock mTime;
      PeriodicWorkcycle mPeriodicWorkcycle;

};


TEST_F(PeriodicWorkcycleTest, PeriodicWorkcycle) {
   StrictMock<RunnableGMock> runnable;

   EXPECT_CALL(runnable,workcycle()).Times(3);

   EXPECT_CALL(mTime,microseconds())
      .WillOnce(Return(77466))
      .WillOnce(Return(77999))
      .WillOnce(Return(78010))
      .WillOnce(Return(78999))
      .WillOnce(Return(79000))
      ;

   mPeriodicWorkcycle.add(runnable);
   mPeriodicWorkcycle.workcycle();
   mPeriodicWorkcycle.workcycle();
   mPeriodicWorkcycle.workcycle();
   mPeriodicWorkcycle.workcycle();
   mPeriodicWorkcycle.workcycle();




}


} // namespace Workcycle
} // namespace Bt
