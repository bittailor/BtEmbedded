//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Concurrency::BlockingQueue
//  
//*************************************************************************************************

#ifndef INC__Bt_Concurrency_BlockingQueue__hpp
#define INC__Bt_Concurrency_BlockingQueue__hpp

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace Bt {
namespace Concurrency {

template <typename T>
class BlockingQueue
{
   public:

      bool empty() const {
         std::lock_guard<std::mutex> lock(mMutex);
         return mQueue.empty();
      }

      void push(const T& iValue)
      {
         std::lock_guard<std::mutex> lock(mMutex);
         mQueue.push(iValue);
         mConditionVariable.notify_one();
      }

      void pop(T& oValue)
      {
         std::unique_lock<std::mutex> lock(mMutex);
         mConditionVariable.wait(lock, [this]{return !mQueue.empty();});
         oValue = std::move(mQueue.front());
         mQueue.pop();
      }

      template< class Rep, class Period>
      bool tryPop(T& oValue, const std::chrono::duration<Rep, Period>& iTimeout)
      {
         std::unique_lock<std::mutex> lock(mMutex);
         if(!mConditionVariable.wait_for(lock, iTimeout, [this]{return !mQueue.empty();})){
            return false;
         }
         oValue = std::move(mQueue.front());
         mQueue.pop();
         return true;
      }

      bool tryPop(T& oValue)
      {
         std::lock_guard<std::mutex> lock(mMutex);
         if (mQueue.empty()) {
            return false;
         }
         oValue = std::move(mQueue.front());
         mQueue.pop();
         return true;
      }

   private:
      mutable std::mutex mMutex;
      std::queue<T> mQueue;
      std::condition_variable mConditionVariable;
};


} // namespace Concurrency
} // namespace Bt

#endif // INC__Bt_Concurrency_BlockingQueue__hpp
