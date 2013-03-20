//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::Spi
//  
//*************************************************************************************************

#ifndef INC__Bt_Mcu_Spi__hpp
#define INC__Bt_Mcu_Spi__hpp

#include "Bt/Mcu/ISpi.hpp"
#include "Bt/Mcu/SpiPlatform.hpp"


namespace Bt {
namespace Mcu {

class Spi : public ISpi , private SpiPlatform
{
   public:

      Spi(BitOrder pBitOrder, Mode pSpiMode, Speed pSpeed);
      virtual ~Spi();

      virtual void chipSelect(bool pSelect);
      virtual uint8_t transfer(uint8_t pData);
   
   private:
   	  // Constructor to prohibit copy construction.
      Spi(const Spi&);

      // Operator= to prohibit copy assignment
      Spi& operator=(const Spi&);
};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_Spi__hpp