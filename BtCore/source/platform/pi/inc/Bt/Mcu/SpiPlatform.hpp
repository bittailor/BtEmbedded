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
#include <atomic>

#include "Bt/Mcu/I_Spi.hpp"
#include "Bt/Mcu/I_Pin.hpp"


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

      void initialize(I_Spi::BitOrder pBitOrder, I_Spi::Mode pSpiMode, I_Spi::Speed pSpeed);
      void dispose();

      static std::atomic<int> sInstanceCounter;

};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_SpiPlatform__hpp
