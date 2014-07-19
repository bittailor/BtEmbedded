//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Concurrency::ExecutionContext
//  
//*************************************************************************************************

#ifndef INC__Bt_Concurrency_ExecutionContext__hpp
#define INC__Bt_Concurrency_ExecutionContext__hpp

#include <thread>

#include <Bt/Concurrency/I_ExecutionContext.hpp>
#include <Bt/Concurrency/BlockingQueue.hpp>

namespace Bt {
namespace Concurrency {

class ExecutionContext : public I_ExecutionContext
{
   public:
      ExecutionContext();
      ~ExecutionContext();


      virtual void invoke(std::function<void()> iFunction);

   private:
      void run();
      void workcycle();

   	  // Constructor to prohibit copy construction.
      ExecutionContext(const ExecutionContext&);

      // Operator= to prohibit copy assignment
      ExecutionContext& operator=(const ExecutionContext&);

      BlockingQueue<std::function<void()>> mQueue;
      bool mRunning;
      std::thread mThread;
};

} // namespace Concurrency
} // namespace Bt

#endif // INC__Bt_Concurrency_ExecutionContext__hpp
