//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::I_RfController
//  
//*************************************************************************************************

#ifndef INC__Bt_Device_I_RfController__hpp
#define INC__Bt_Device_I_RfController__hpp

#include <stddef.h>

namespace Bt {
namespace Device {

class I_RfController {
   public:
      enum { MAX_PAYLOAD = 3*32};

      virtual ~I_RfController() {}
      
      virtual size_t payloadSize() = 0;
};

} // namespace Device
} // namespace Bt

#endif // INC__Bt_Device_I_RfController__hpp
