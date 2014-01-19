//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::CoreInitializer
//  
//*************************************************************************************************

#ifndef INC__Bt_CoreInitializer__hpp
#define INC__Bt_CoreInitializer__hpp

#include "Bt/CoreInitializerPlatform.hpp"

namespace Bt {

class CoreInitializer : private CoreInitializerPlatform
{
   public:
      CoreInitializer();
      ~CoreInitializer();
   
   private:
   	  // Constructor to prohibit copy construction.
      CoreInitializer(const CoreInitializer&);

      // Operator= to prohibit copy assignment
      CoreInitializer& operator=(const CoreInitializer&);
};

} // namespace Bt

#endif // INC__Bt_CoreInitializer__hpp
