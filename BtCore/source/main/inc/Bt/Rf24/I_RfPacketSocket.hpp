//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::I_RfPacketSocket
//  
//*************************************************************************************************

#ifndef INC__Bt_Rf24_I_RfPacketSocket__hpp
#define INC__Bt_Rf24_I_RfPacketSocket__hpp

#include <stddef.h>
#include <stdint.h>

#include "Bt/Rf24/I_RfNetworkSocket.hpp"

namespace Bt {
namespace Rf24 {

class I_RfPacketSocket {
   public:

      virtual ~I_RfPacketSocket() {}
      
      virtual size_t payloadCapacity() const = 0;

      virtual bool hasPendingPacket() const = 0;
      virtual size_t pendingPacketSize() const = 0;

      virtual int32_t send(uint8_t* iPayload, size_t iSize, uint8_t iGatewayNodeId) = 0;
      virtual int32_t receive(uint8_t* oPayload, size_t iMaxSize, uint8_t* oGatewayNodeId) = 0;


};

//-------------------------------------------------------------------------------------------------

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_I_RfPacketSocket__hpp
