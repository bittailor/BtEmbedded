//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::GpioLibrary
//  
//*************************************************************************************************

#ifndef INC__Bt_Mcu_GpioLibrary__hpp
#define INC__Bt_Mcu_GpioLibrary__hpp

#include <atomic>

namespace Bt {
namespace Mcu {

class GpioLibrary
{
   public:
      static void ensureIsInitialized();


   private:
      // Only Statics!
      GpioLibrary();
      ~GpioLibrary();
      GpioLibrary(const GpioLibrary&);
      GpioLibrary& operator=(const GpioLibrary&);

      static std::atomic<bool> sIsInitialized;
};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_GpioLibrary__hpp
