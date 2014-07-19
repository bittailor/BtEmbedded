//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Concurrency::I_ExecutionContext
//  
//*************************************************************************************************

#ifndef INC__Bt_Concurrency_I_ExecutionContext__hpp
#define INC__Bt_Concurrency_I_ExecutionContext__hpp

#include <functional>
#include <future>
#include <type_traits>

namespace Bt {
namespace Concurrency {

class I_ExecutionContext {
   public:
      virtual ~I_ExecutionContext() {}
      
      virtual void invoke(std::function<void()> iFunction) = 0;

      template<typename Fn, typename... Args>
      std::future<typename std::result_of<Fn(Args...)>::type> invokeTask(Fn&& fn, Args&&... args) {
    	  typedef typename std::result_of<Fn(Args...)>::type R;
    	  std::shared_ptr<std::promise<R>> promise = std::make_shared<std::promise<R>>();
    	  std::future<R> future = promise->get_future();
    	  std::function<R()> function(std::bind(std::forward<Fn>(fn),std::forward<Args>(args)...));
    	  std::function<void()> voidFunction = std::bind(&I_ExecutionContext::invokeTaskInternal<R>, this, function, promise);
    	  invoke(voidFunction);
    	  return future;
      }

   private:

      template<typename R>
      void invokeTaskInternal(std::function<R()> iFunction, std::shared_ptr<std::promise<R>> iPromise) {
    	  try {
    		  iPromise->set_value(iFunction());
    	  } catch(...) {
			  iPromise->set_exception(std::current_exception());
		  }
      }
};

} // namespace Concurrency
} // namespace Bt

#endif // INC__Bt_Concurrency_I_ExecutionContext__hpp
