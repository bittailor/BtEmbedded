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

#include <algorithm>

#include "Bt/Util/Logging.hpp"

namespace Bt {
namespace Rf24 {


//-------------------------------------------------------------------------------------------------

RfPacketSocket::RfPacketSocket(I_RfNetworkSocket& pNetworkSocket) : mNetworkSocket(pNetworkSocket) {

}

//-------------------------------------------------------------------------------------------------

RfPacketSocket::~RfPacketSocket() {

}

//-------------------------------------------------------------------------------------------------

size_t RfPacketSocket::payloadCapacity() const {
   return I_RfNetworkSocket::Packet::PAYLOAD_CAPACITY;
}

//-------------------------------------------------------------------------------------------------

bool RfPacketSocket::hasPendingPacket() const {
   return !mReceivedQueue.empty();
}

//-------------------------------------------------------------------------------------------------

size_t RfPacketSocket::pendingPacketSize() const {
   const std::vector<uint8_t>* packet = mReceivedQueue.peek();
   if (packet == nullptr) {
      return -1;
   }
   return packet->size();
}

//-------------------------------------------------------------------------------------------------

int32_t RfPacketSocket::send(uint8_t* iPayload, size_t iSize, uint8_t iGatewayNodeId) {
   size_t sendSize = std::min(iSize,payloadCapacity());
   std::promise<bool> promise;
   auto future = promise.get_future();
   mSendQueue.push(std::pair<std::promise<bool>,std::vector<uint8_t>>(std::move(promise),std::vector<uint8_t>(iPayload,iPayload+iSize)));
   if(future.get()) {
      return sendSize;
   }
   return -1;
}

//-------------------------------------------------------------------------------------------------

int32_t RfPacketSocket::receive(uint8_t* oPayload, size_t iMaxSize, uint8_t* oGatewayNodeId) {
   std::vector<uint8_t> packet;
   mReceivedQueue.pop(packet);
   size_t readSize = std::min(iMaxSize,packet.size());
   memcpy(oPayload, packet.data(), packet.size());
   return readSize;
}

//-------------------------------------------------------------------------------------------------

void RfPacketSocket::workcycle() {

   if(mNetworkSocket.available()) {
      I_RfNetworkSocket::Packet packet;
      mNetworkSocket.receive(packet);
      const uint8_t* buffer = static_cast<const uint8_t*>(packet.payload());
      mReceivedQueue.push(std::vector<uint8_t>(buffer,buffer + packet.size()));
   }

   std::pair<std::promise<bool>,std::vector<uint8_t>> sendPacket;
   if(mSendQueue.tryPop(sendPacket)) {
      I_RfNetworkSocket::Packet packet;
      packet.writePayload(sendPacket.second.data(),sendPacket.second.size());
      bool sendResult = mNetworkSocket.send(packet);
      sendPacket.first.set_value(sendResult);
   }
}

//-------------------------------------------------------------------------------------------------

} // namespace Rf24
} // namespace Bt
