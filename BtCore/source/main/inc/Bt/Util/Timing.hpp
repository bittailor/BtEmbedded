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

#include <stdint.h>

namespace Bt {
namespace Util {

void delayInMilliseconds(uint32_t milliseconds);
void delayInMicroseconds(uint32_t microseconds);

uint32_t milliseconds();
uint32_t microseconds();


} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_Timing__hpp
