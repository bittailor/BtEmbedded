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

#include "Bt/Mcu/I_Spi.hpp"
#include "Bt/Mcu/SpiPlatform.hpp"
#include "Bt/Mcu/I_Pin.hpp"


namespace Bt {
namespace Mcu {

class Spi : public I_Spi , private SpiPlatform
{
   public:


      Spi(BitOrder pBitOrder, Mode pSpiMode, Speed pSpeed, ChipSelect pChipSelect);
      virtual ~Spi();

      virtual uint8_t transfer(uint8_t pData);
      virtual void transfer(uint8_t* pTransmitData, uint8_t* pReceiveData, size_t pSize);
   
   private:
   	  // Constructor to prohibit copy construction.
      Spi(const Spi&);

      // Operator= to prohibit copy assignment
      Spi& operator=(const Spi&);
};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_Spi__hpp
