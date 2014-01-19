//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::CoreInitializerPlatform
//  
//*************************************************************************************************

#ifndef INC__Bt_CoreInitializerPlatform__hpp
#define INC__Bt_CoreInitializerPlatform__hpp

namespace Bt {

class CoreInitializerPlatform 
{
   public:
      CoreInitializerPlatform();
      ~CoreInitializerPlatform();
   
   private:
   	  // Constructor to prohibit copy construction.
      CoreInitializerPlatform(const CoreInitializerPlatform&);

      // Operator= to prohibit copy assignment
      CoreInitializerPlatform& operator=(const CoreInitializerPlatform&);
};

} // namespace Bt

#endif // INC__Bt_CoreInitializerPlatform__hpp