//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::InterruptPinPlatform
//  
//*************************************************************************************************

#ifndef INC__Bt_Mcu_InterruptPinPlatform__hpp
#define INC__Bt_Mcu_InterruptPinPlatform__hpp

#include <cstdint>
#include <string>
#include <atomic>
#include <thread>
#include <functional>


#include <Bt/Mcu/I_InterruptPin.hpp>

namespace Bt {
namespace Mcu {

class InterruptPinPlatform 
{
   public:
      InterruptPinPlatform(uint8_t iPinId, I_InterruptPin::Edge iEdge);
      ~InterruptPinPlatform();
   
      void enable(std::function<void()> iInterruptHandler);
      void disable();

   private:
   	  // Constructor to prohibit copy construction.
      InterruptPinPlatform(const InterruptPinPlatform&);

      // Operator= to prohibit copy assignment
      InterruptPinPlatform& operator=(const InterruptPinPlatform&);

      void pollCycle(std::function<void()> iInterruptHandler, int iPinFileHandle, int iPipeFileHandle);
      bool waitForInterrupt(int iPinFileHandle, int iPipeFileHandle);

      const uint8_t mPinId;
      I_InterruptPin::Edge mEdge;
      std::string mPinPath;
      std::thread mPollThread;
      int mPinFileHandle;
      int mDisablePipe[2];
      std::atomic<bool> mEnabled;
};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_InterruptPinPlatform__hpp
