//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::PinGMock
//  
//*************************************************************************************************

#ifndef INC__Bt_Mcu_PinGMock__hpp
#define INC__Bt_Mcu_PinGMock__hpp

#include <gmock/gmock.h>

#include "Bt/Mcu/I_Pin.hpp"

namespace Bt {
namespace Mcu {

class PinGMock : public I_Pin {
   public:
      
      MOCK_METHOD1(mode, void(Mode));
      MOCK_METHOD1(write, void(bool));
      MOCK_METHOD0(read, bool());
};

} // namespace Mcu
} // namespace Bt

#endif // INC__Bt_Mcu_PinGMock__hpp
