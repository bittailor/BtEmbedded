//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Concurrency::BockingQueue
//  
//*************************************************************************************************

#ifndef INC__Bt_Concurrency_BockingQueue__hpp
#define INC__Bt_Concurrency_BockingQueue__hpp

namespace Bt {
namespace Concurrency {

class BockingQueue 
{
   public:
      BockingQueue();
      ~BockingQueue();
   
   private:
   	  // Constructor to prohibit copy construction.
      BockingQueue(const BockingQueue&);

      // Operator= to prohibit copy assignment
      BockingQueue& operator=(const BockingQueue&);
};

} // namespace Concurrency
} // namespace Bt

#endif // INC__Bt_Concurrency_BockingQueue__hpp