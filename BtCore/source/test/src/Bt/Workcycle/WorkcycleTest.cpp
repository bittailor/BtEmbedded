//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Workcycle::WorkcycleTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Workcycle/Workcycle.hpp"
#include "Bt/Workcycle/RunnableGMock.hpp"

namespace Bt {
namespace Workcycle {

class WorkcycleTest : public ::testing::Test {
   
   protected:
      
      virtual void SetUp() {
      }

      virtual void TearDown() {
         
      }

      Workcycle mWorkcycle;

};


TEST_F(WorkcycleTest, oneWorkcycle) {

   RunnableGMock runnable1;
   RunnableGMock runnable2;
   RunnableGMock runnable3;

   EXPECT_CALL(runnable1,workcycle()).Times(3);
   EXPECT_CALL(runnable2,workcycle()).Times(1);
   EXPECT_CALL(runnable3,workcycle()).Times(2);

   mWorkcycle.add(runnable1);

   mWorkcycle.oneWorkcycle();

   mWorkcycle.add(runnable2);
   mWorkcycle.add(runnable3);

   mWorkcycle.oneWorkcycle();

   mWorkcycle.remove(runnable2);

   mWorkcycle.oneWorkcycle();

   mWorkcycle.remove(runnable1);
   mWorkcycle.remove(runnable3);

}


} // namespace Workcycle
} // namespace Bt
