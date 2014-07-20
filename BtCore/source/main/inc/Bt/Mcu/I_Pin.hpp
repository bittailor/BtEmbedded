//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::I_Pin
//  
//*************************************************************************************************

#ifndef INC__Bt_Mcu_I_Pin__hpp
#define INC__Bt_Mcu_I_Pin__hpp

namespace Bt {
namespace Mcu {

class I_Pin {
   public:

      enum Mode {
         MODE_INPUT,
         MODE_OUTPUT
      };

      virtual ~I_Pin() {}
      
      virtual void mode(Mode iMode) = 0;

      virtual void write(bool iHigh) = 0;

      virtual bool read() = 0;


};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_I_Pin__hpp
