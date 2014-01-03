//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::SpiPlatform
//  
//*************************************************************************************************

#ifndef INC__Bt_Mcu_SpiPlatform__hpp
#define INC__Bt_Mcu_SpiPlatform__hpp

#include "Bt/Mcu/I_Spi.hpp"

namespace Bt {
namespace Mcu {

class SpiPlatform 
{
   public:
      SpiPlatform(I_Spi::BitOrder pBitOrder, I_Spi::Mode pSpiMode, I_Spi::Speed pSpeed);
      ~SpiPlatform();
   
      uint8_t transfer(uint8_t data);

   private:
   	  // Constructor to prohibit copy construction.
      SpiPlatform(const SpiPlatform&);

      // Operator= to prohibit copy assignment
      SpiPlatform& operator=(const SpiPlatform&);
};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_SpiPlatform__hpp
