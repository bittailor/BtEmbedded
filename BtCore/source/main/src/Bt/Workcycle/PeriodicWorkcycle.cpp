//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Workcycle::PeriodicWorkcycle
//  
//*************************************************************************************************

#include "Bt/Workcycle/PeriodicWorkcycle.hpp"

namespace Bt {
namespace Workcycle {


//-------------------------------------------------------------------------------------------------

PeriodicWorkcycle::PeriodicWorkcycle(unsigned long iPeriodInMicroSeconds, Util::I_Time& iTime)
: mTime(&iTime), mPeriodInMicroSeconds(iPeriodInMicroSeconds), mNextWorkcycle(0){
}

//-------------------------------------------------------------------------------------------------

PeriodicWorkcycle::~PeriodicWorkcycle() {

}

//-------------------------------------------------------------------------------------------------

void PeriodicWorkcycle::workcycle() {
   unsigned long now = mTime->microseconds();
   if (now >= mNextWorkcycle) {
      // mNextWorkcycle = now + mPeriodInMicroSeconds;
      while(now >= mNextWorkcycle) {
         mNextWorkcycle += mPeriodInMicroSeconds;
      }
      oneWorkcycle();
   }
}

//-------------------------------------------------------------------------------------------------

} // namespace Workcycle
} // namespace Bt
