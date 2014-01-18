//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Delay
//  
//*************************************************************************************************

#include "Bt/Util/Delay.hpp"
#include <Arduino.h>

namespace Bt {
namespace Util {


//-------------------------------------------------------------------------------------------------

void delayInMilliseconds(unsigned int milliseconds) {
   delay(milliseconds);
}

//-------------------------------------------------------------------------------------------------

void delayInMicroseconds(unsigned int microseconds) {
   delayMicroseconds(microseconds);
}

//-------------------------------------------------------------------------------------------------

} // namespace Util
} // namespace Bt
