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

namespace Bt {
namespace Mcu {

class Spi 
{
   public:
      Spi();
      ~Spi();
   
   private:
   	  // Constructor to prohibit copy construction.
      Spi(const Spi&);

      // Operator= to prohibit copy assignment
      Spi& operator=(const Spi&);
};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_Spi__hpp