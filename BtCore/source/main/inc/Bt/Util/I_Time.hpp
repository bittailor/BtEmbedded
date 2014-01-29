//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::I_Time
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_I_Time__hpp
#define INC__Bt_Util_I_Time__hpp

namespace Bt {
namespace Util {

class I_Time {
   public:
      virtual ~I_Time() {}
      
      virtual unsigned long microseconds() = 0;
};

} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_I_Time__hpp
