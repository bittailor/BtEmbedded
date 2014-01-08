//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::TimeoutPlatform
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_TimeoutPlatform__hpp
#define INC__Bt_Util_TimeoutPlatform__hpp

namespace Bt {
namespace Util {

class TimeoutPlatform 
{
   public:
      TimeoutPlatform(unsigned int pMilliseconds);
      ~TimeoutPlatform();
   
   protected:
      bool check() const;

   private:

};

} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_TimeoutPlatform__hpp
