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
#include <cstddef>


namespace Bt {
namespace Mcu {

class I_Spi {
   public:

      enum BitOrder {BIT_ORDER_LSBFIRST , BIT_ORDER_MSBFIRST};
      enum Mode {MODE_0,MODE_1,MODE_2,MODE_3};
      enum Speed {SPEED_4_MHZ, SPEED_8_MHZ};
      enum ChipSelect {CHIP_SELECT_0, CHIP_SELECT_1, CHIP_SELECT_NONE};

      virtual ~I_Spi() {}
      
      virtual uint8_t transfer(uint8_t iData)=0;
      virtual void transfer(uint8_t* iTransmitData, uint8_t* oReceiveData, size_t iSize) = 0;
};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_I_Spi__hpp
