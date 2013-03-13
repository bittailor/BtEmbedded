//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::ISpi
//  
//*************************************************************************************************

#ifndef INC__Bt_Mcu_ISpi__hpp
#define INC__Bt_Mcu_ISpi__hpp

#include <cstdint>

namespace Bt {
namespace Mcu {

class ISpi {
   public:
      virtual ~ISpi() {}
      
      virtual uint8_t transfer(uint8_t data) = 0;
};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_ISpi__hpp
