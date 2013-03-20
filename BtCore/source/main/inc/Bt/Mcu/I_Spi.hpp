//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::I_Spi
//  
//*************************************************************************************************

#ifndef INC__Bt_Mcu_I_Spi__hpp
#define INC__Bt_Mcu_I_Spi__hpp

#include <cstdint>

namespace Bt {
namespace Mcu {

class I_Spi {
   public:

      enum BitOrder {BIT_ORDER_LSBFIRST , BIT_ORDER_MSBFIRST};
      enum Mode {MODE_0,MODE_1,MODE_2,MODE_3};
      enum Speed {SPEED_4_MHZ};

      virtual ~I_Spi() {}
      
      virtual void chipSelect(bool pSelect) = 0;
      virtual uint8_t transfer(uint8_t pData) = 0;
};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_I_Spi__hpp
