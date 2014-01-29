//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Workcycle::PeriodicWorkcycle
//  
//*************************************************************************************************

#ifndef INC__Bt_Workcycle_PeriodicWorkcycle__hpp
#define INC__Bt_Workcycle_PeriodicWorkcycle__hpp

#include "Bt/Workcycle/I_Runnable.hpp"
#include "Bt/Workcycle/Workcycle.hpp"
#include "Bt/Util/I_Time.hpp"

namespace Bt {
namespace Workcycle {

class PeriodicWorkcycle : public Workcycle, public I_Runnable
{
   public:
      PeriodicWorkcycle(unsigned long iPeriodInMicroSeconds,
                        Util::I_Time& iTime);
      ~PeriodicWorkcycle();

      virtual void workcycle();
   
   private:
   	  // Constructor to prohibit copy construction.
      PeriodicWorkcycle(const PeriodicWorkcycle&);

      // Operator= to prohibit copy assignment
      PeriodicWorkcycle& operator=(const PeriodicWorkcycle&);

      Util::I_Time* mTime;
      const unsigned long mPeriodInMicroSeconds;
      unsigned long mNextWorkcycle;



};

} // namespace Workcycle
} // namespace Bt

#endif // INC__Bt_Workcycle_PeriodicWorkcycle__hpp
