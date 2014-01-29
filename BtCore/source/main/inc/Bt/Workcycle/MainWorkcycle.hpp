//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Workcycle::MainWorkcycle
//  
//*************************************************************************************************

#ifndef INC__Bt_Workcycle_MainWorkcycle__hpp
#define INC__Bt_Workcycle_MainWorkcycle__hpp

#include "Bt/Workcycle/Workcycle.hpp"

namespace Bt {
namespace Workcycle {

class MainWorkcycle : public Workcycle
{
   public:
      MainWorkcycle();
      ~MainWorkcycle();
   
      void run();

   private:
   	  // Constructor to prohibit copy construction.
      MainWorkcycle(const MainWorkcycle&);

      // Operator= to prohibit copy assignment
      MainWorkcycle& operator=(const MainWorkcycle&);
};

} // namespace Workcycle
} // namespace Bt

#endif // INC__Bt_Workcycle_MainWorkcycle__hpp
