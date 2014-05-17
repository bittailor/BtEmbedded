//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Concurrency::Latch
//  
//*************************************************************************************************

#ifndef INC__Bt_Concurrency_Latch__hpp
#define INC__Bt_Concurrency_Latch__hpp

namespace Bt {
namespace Concurrency {

class Latch 
{
   public:
      Latch();
      ~Latch();
   
   private:
   	  // Constructor to prohibit copy construction.
      Latch(const Latch&);

      // Operator= to prohibit copy assignment
      Latch& operator=(const Latch&);
};

} // namespace Concurrency
} // namespace Bt

#endif // INC__Bt_Concurrency_Latch__hpp