//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Workcycle::Workcycle
//  
//*************************************************************************************************

#ifndef INC__Bt_Workcycle_Workcycle__hpp
#define INC__Bt_Workcycle_Workcycle__hpp

#include "Bt/Workcycle/I_Workcycle.hpp"

namespace Bt {
namespace Workcycle {

class Workcycle : public I_Workcycle
{
   public:
      Workcycle();
      ~Workcycle();

      virtual void add(I_Runnable& iRunnable);
      virtual void remove(I_Runnable& iRunnable);

      void oneWorkcycle();
   
   private:
   	  // Constructor to prohibit copy construction.
      Workcycle(const Workcycle&);

      // Operator= to prohibit copy assignment
      Workcycle& operator=(const Workcycle&);


      typedef Util::IntrusiveList<I_Runnable> Runnables;

      Util::IntrusiveList<I_Runnable> mRunnables;
};

} // namespace Workcycle
} // namespace Bt

#endif // INC__Bt_Workcycle_Workcycle__hpp
