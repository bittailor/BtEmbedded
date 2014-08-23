//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Concurrency::ExecutionContext
//  
//*************************************************************************************************

#include "Bt/Concurrency/ExecutionContext.hpp"

#include <future>
#include <iostream>

#include <Bt/Log/Logging.hpp>

namespace Bt {
namespace Concurrency {


//-------------------------------------------------------------------------------------------------

ExecutionContext::ExecutionContext() : mRunning(true), mThread(&ExecutionContext::run,this) {

}

//-------------------------------------------------------------------------------------------------

ExecutionContext::~ExecutionContext() {
   mQueue.push([this](){mRunning = false;});
   mThread.join();
}

//-------------------------------------------------------------------------------------------------

void ExecutionContext::invoke(std::function<void()> iFunction) {
   mQueue.push(std::move(iFunction));
}

//-------------------------------------------------------------------------------------------------

void ExecutionContext::run() {
   while(mRunning) {
      workcycle();
   }
}

//-------------------------------------------------------------------------------------------------

void ExecutionContext::workcycle() {
   std::function<void ()> function;
   mQueue.pop(function);
   try {
      function();
   } catch(const std::exception& exception) {
      BT_LOG(ERROR) << "ExecutionContext caught a std exception (" << typeid(exception).name() << ") : "
                << exception.what() << std::endl;
   } catch (...) {
      BT_LOG(ERROR) << "ExecutionContext caught a non-std exception!" <<  std::endl;
   }
}

//-------------------------------------------------------------------------------------------------

} // namespace Concurrency
} // namespace Bt
