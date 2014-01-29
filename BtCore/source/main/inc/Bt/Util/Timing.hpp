//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Timing
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_Timing__hpp
#define INC__Bt_Util_Timing__hpp

namespace Bt {
namespace Util {

void delayInMilliseconds(unsigned int milliseconds);
void delayInMicroseconds(unsigned int microseconds);

unsigned int milliseconds();
unsigned int microseconds();


} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_Timing__hpp
