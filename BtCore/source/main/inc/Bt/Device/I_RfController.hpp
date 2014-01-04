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

#include "Bt/Device/I_Rf24Device.hpp"

namespace Bt {
namespace Device {

class I_RfController {
   public:

      using Pipe = I_Rf24Device::Pipe;

      enum { MAX_PAYLOAD_SIZE = I_Rf24Device::MAX_PAYLOAD_SIZE };

      virtual ~I_RfController() {}

      virtual size_t write(Pipe pPipe, uint8_t* pData, size_t pSize) = 0;

      virtual void startListening() = 0;
      virtual void stopListening() = 0;
      virtual bool isDataAvailable() = 0;
      virtual size_t read(uint8_t* pBuffer, size_t pSize) = 0;
      virtual size_t read(uint8_t* pBuffer, size_t pSize, Pipe& pPipe) = 0;
};

} // namespace Device
} // namespace Bt

#endif // INC__Bt_Device_I_RfController__hpp
