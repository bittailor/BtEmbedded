//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Concurrency::ExecutionContextTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include <future>
#include <stdexcept>

#include "Bt/Concurrency/ExecutionContext.hpp"

namespace Bt {
namespace Concurrency {

TEST(ExecutionContextTest, startStop) {
   auto future = std::async(std::launch::async, [](){
      ExecutionContext mExecutionContext;
      return true;
   });

   std::future_status status = future.wait_for(std::chrono::seconds(5));
   ASSERT_EQ(std::future_status::ready, status);
}

TEST(ExecutionContextTest, checkExecutionOnWorkerThread) {
   ExecutionContext mExecutionContext;

   auto future = mExecutionContext.invokeTask([](){
      return std::this_thread::get_id();
   });

   ASSERT_NE(std::this_thread::get_id(), future.get());
}

TEST(ExecutionContextTest, checkExecution) {
   ExecutionContext mExecutionContext;

   auto future = mExecutionContext.invokeTask([](){
      return std::this_thread::get_id();
   });

   ASSERT_NE(std::this_thread::get_id(), future.get());
}

TEST(ExecutionContextTest, checkExecutionBothOnSameWorkerTread) {
   ExecutionContext mExecutionContext;

   auto future1 = mExecutionContext.invokeTask([](){
      return std::this_thread::get_id();
   });

   auto future2 = mExecutionContext.invokeTask([](){
	   return std::this_thread::get_id();
   });

   ASSERT_EQ(future1.get(), future2.get());
}

TEST(ExecutionContextTest, checkException) {
   ExecutionContext mExecutionContext;

   auto future = mExecutionContext.invokeTask([](){
	   throw std::invalid_argument("argument");
	   return true;
   });

   ASSERT_THROW(future.get(), std::invalid_argument);
}


} // namespace Concurrency
} // namespace Bt
