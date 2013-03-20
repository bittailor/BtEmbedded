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

      enum BitOrder {BIT_ORDER_LSBFIRST , BIT_ORDER_MSBFIRST};
      enum Mode {MODE_0,MODE_1,MODE_2,MODE_3};
      enum Speed {SPEED_4_MHZ};

      virtual ~ISpi() {}
      
      virtual void chipSelect(bool pSelect) = 0;
      virtual uint8_t transfer(uint8_t pData) = 0;
};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_ISpi__hpp
