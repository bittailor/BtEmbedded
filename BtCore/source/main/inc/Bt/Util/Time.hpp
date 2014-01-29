//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Time
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_Time__hpp
#define INC__Bt_Util_Time__hpp

#include "Bt/Util/I_Time.hpp"

namespace Bt {
namespace Util {

class Time : public I_Time
{
   public:
      Time();
      ~Time();

      virtual unsigned long microseconds();
   
   private:
   	  // Constructor to prohibit copy construction.
      Time(const Time&);

      // Operator= to prohibit copy assignment
      Time& operator=(const Time&);
};

} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_Time__hpp
