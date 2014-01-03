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

#include <chrono>

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
      std::chrono::milliseconds mDuration;
      std::chrono::high_resolution_clock::time_point mStart;

};

} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_TimeoutPlatform__hpp
