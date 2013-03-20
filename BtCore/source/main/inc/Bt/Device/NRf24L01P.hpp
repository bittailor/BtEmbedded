//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::NRf24L01P
//  
//*************************************************************************************************

#ifndef INC__Bt_Device_NRf24L01P__hpp
#define INC__Bt_Device_NRf24L01P__hpp

#include "Bt/Mcu/I_Spi.hpp"
#include "Bt/Mcu/I_Pin.hpp"

namespace Bt {
namespace Device {

class NRf24L01P 
{
   public:
      NRf24L01P(Mcu::I_Spi& pSpi, Mcu::I_Pin& pChipEnable);
      ~NRf24L01P();



   
   private:
   	  // Constructor to prohibit copy construction.
      NRf24L01P(const NRf24L01P&);

      // Operator= to prohibit copy assignment
      NRf24L01P& operator=(const NRf24L01P&);

      Mcu::I_Spi* mSpi;
      Mcu::I_Pin* mChipEnable;

};

} // namespace Device
} // namespace Bt

#endif // INC__Bt_Device_NRf24L01P__hpp
