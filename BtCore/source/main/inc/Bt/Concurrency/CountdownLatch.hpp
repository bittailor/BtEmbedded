//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Concurrency::CountdownLatch
//  
//*************************************************************************************************

#ifndef INC__Bt_Concurrency_CountdownLatch__hpp
#define INC__Bt_Concurrency_CountdownLatch__hpp

#include <mutex>
#include <condition_variable>

namespace Bt {
namespace Concurrency {

class CountdownLatch 
{
   public:
      explicit CountdownLatch(unsigned int iCount);
      ~CountdownLatch();

      void countDown();
      void wait();

      template< class Rep, class Period>
      bool waitFor(const std::chrono::duration<Rep, Period>& iTimeout) {
         std::unique_lock<std::mutex> lock(mMutex);
         return mConditionVariable.wait_for(lock, iTimeout, [this]{return mCount == 0;});
      }

   private:
   	  // Constructor to prohibit copy construction.
      CountdownLatch(const CountdownLatch&);

      // Operator= to prohibit copy assignment
      CountdownLatch& operator=(const CountdownLatch&);

      unsigned int mCount;
      std::mutex mMutex;
      std::condition_variable mConditionVariable;

};

} // namespace Concurrency
} // namespace Bt

#endif // INC__Bt_Concurrency_CountdownLatch__hpp
