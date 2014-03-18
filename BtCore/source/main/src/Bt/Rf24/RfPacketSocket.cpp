//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfPacketSocket
//  
//*************************************************************************************************

#include "Bt/Rf24/RfPacketSocket.hpp"
#include "Bt/Util/Logging.hpp"

namespace Bt {
namespace Rf24 {


//-------------------------------------------------------------------------------------------------

RfPacketSocket::RfPacketSocket(I_RfNetworkSocket& pNetworkSocket) : mNetworkSocket(&pNetworkSocket) {

}

//-------------------------------------------------------------------------------------------------

RfPacketSocket::~RfPacketSocket() {

}

//-------------------------------------------------------------------------------------------------

bool RfPacketSocket::send(uint8_t* iPayload, size_t iSize,  uint8_t iNodeId){
   if(iSize > I_RfNetworkSocket::Packet::PAYLOAD_CAPACITY) {
      BT_UTIL_LOG_ERROR("size to large " << iSize << ">" << I_RfNetworkSocket::Packet::PAYLOAD_CAPACITY );
      return false;
   }

   I_RfNetworkSocket::Packet packet;
   packet.writePayload(iPayload,iSize);
   return mNetworkSocket->send(packet);
}

//-------------------------------------------------------------------------------------------------

int32_t RfPacketSocket::receive(uint8_t* oPayload, size_t iMaxSize, uint8_t* oNodeId){
   I_RfNetworkSocket::Packet packet;
   if (!mNetworkSocket->receive(packet)){
      return false;
   }

   if (oNodeId != nullptr) {
      *oNodeId = packet.source();
   }

   if(packet.size() > iMaxSize) {
      BT_UTIL_LOG_ERROR("max size to small " << packet.size() << ">" << iMaxSize);
      return -1;
   }

   return packet.readPayload(oPayload, iMaxSize);
}

//-------------------------------------------------------------------------------------------------

bool RfPacketSocket::available(){
   return mNetworkSocket->available();
}

//-------------------------------------------------------------------------------------------------

} // namespace Rf24
} // namespace Bt
