//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Timeout
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_Timeout__hpp
#define INC__Bt_Util_Timeout__hpp

#include "Bt/Util/TimeoutPlatform.hpp"

namespace Bt {
namespace Util {

class Timeout : private TimeoutPlatform
{


   public:
      Timeout(unsigned int pMilliseconds);
      ~Timeout();

      operator bool() const; // TODO (BT) implement the safe bool idiom !
   
   private:

};

} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_Timeout__hpp
