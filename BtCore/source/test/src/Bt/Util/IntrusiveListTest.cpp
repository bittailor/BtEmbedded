//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::IntrusiveListTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Util/IntrusiveList.hpp"

namespace Bt {
namespace Util {


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

class TestElement : public IntrusiveList<TestElement>::Element
{

};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------


class IntrusiveListTest : public ::testing::Test {
   
   protected:
      
      virtual void SetUp() {
      }

      virtual void TearDown() {
         
      }

      IntrusiveList<TestElement> mIntrusiveList;

};

//-------------------------------------------------------------------------------------------------

TEST_F(IntrusiveListTest, pushBack) {

   TestElement e1;
   TestElement e2;
   TestElement e3;


   mIntrusiveList.pushBack(e1);
   mIntrusiveList.pushBack(e2);
   mIntrusiveList.pushBack(e3);

   IntrusiveList<TestElement>::Iterator iterator = mIntrusiveList.begin();

   EXPECT_EQ(&e1,&(*iterator++));
   EXPECT_EQ(&e2,&(*iterator++));
   EXPECT_EQ(&e3,&(*iterator++));

}

//-------------------------------------------------------------------------------------------------

TEST_F(IntrusiveListTest, iteratorBeginEnd) {

   TestElement e1;
   TestElement e2;
   TestElement e3;

   mIntrusiveList.pushBack(e1);
   mIntrusiveList.pushBack(e2);
   mIntrusiveList.pushBack(e3);

   IntrusiveList<TestElement>::Iterator iterator = mIntrusiveList.begin();
   ++iterator;
   ++iterator;
   ++iterator;
   EXPECT_TRUE(iterator == mIntrusiveList.end());
}

//-------------------------------------------------------------------------------------------------

TEST_F(IntrusiveListTest, remove) {

   TestElement e1;
   TestElement e2;
   TestElement e3;
   TestElement e4;

   mIntrusiveList.pushBack(e1);
   mIntrusiveList.pushBack(e2);
   mIntrusiveList.pushBack(e3);
   mIntrusiveList.pushBack(e4);


   mIntrusiveList.remove(e2);

   {
      IntrusiveList<TestElement>::Iterator iterator = mIntrusiveList.begin();
      EXPECT_EQ(&e1,&(*iterator++));
      EXPECT_EQ(&e3,&(*iterator++));
      EXPECT_EQ(&e4,&(*iterator++));
   }

   mIntrusiveList.pushBack(e2);

   {
      IntrusiveList<TestElement>::Iterator iterator = mIntrusiveList.begin();
      EXPECT_EQ(&e1,&(*iterator++));
      EXPECT_EQ(&e3,&(*iterator++));
      EXPECT_EQ(&e4,&(*iterator++));
      EXPECT_EQ(&e2,&(*iterator++));
   }

   mIntrusiveList.remove(e1);

   {
      IntrusiveList<TestElement>::Iterator iterator = mIntrusiveList.begin();
      EXPECT_EQ(&e3,&(*iterator++));
      EXPECT_EQ(&e4,&(*iterator++));
      EXPECT_EQ(&e2,&(*iterator++));
   }

   mIntrusiveList.remove(e2);

   {
      IntrusiveList<TestElement>::Iterator iterator = mIntrusiveList.begin();
      EXPECT_EQ(&e3,&(*iterator++));
      EXPECT_EQ(&e4,&(*iterator++));
   }

   mIntrusiveList.remove(e3);
   mIntrusiveList.remove(e4);

   EXPECT_TRUE(mIntrusiveList.begin() == mIntrusiveList.end());

}

//-------------------------------------------------------------------------------------------------


} // namespace Util
} // namespace Bt
