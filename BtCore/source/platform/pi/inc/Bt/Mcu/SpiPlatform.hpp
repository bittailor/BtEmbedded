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

#include <cstdint>

#include "Bt/Mcu/ISpi.hpp"

namespace Bt {
namespace Mcu {

class SpiPlatform 
{
   public:
      SpiPlatform(ISpi::BitOrder pBitOrder, ISpi::Mode pSpiMode, ISpi::Speed pSpeed);
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
