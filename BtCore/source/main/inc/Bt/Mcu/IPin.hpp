//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::IPin
//  
//*************************************************************************************************

#ifndef INC__Bt_Mcu_IPin__hpp
#define INC__Bt_Mcu_IPin__hpp

namespace Bt {
namespace Mcu {

class IPin {
   public:
      enum Mode {
         MODE_INPUT,
         MODE_OUTPUT
      };

      virtual ~IPin() {}
      
      virtual void mode(Mode pMode) = 0;

      virtual void write(bool pHigh) = 0;

      virtual bool read() = 0;


};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_IPin__hpp
