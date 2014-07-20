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
#include <iostream>

#include <Bt/Log/Logging.hpp>


namespace Bt {
namespace Rf24 {


//-------------------------------------------------------------------------------------------------

RfPacketSocket::RfPacketSocket(I_RfNetworkSocket& iNetworkSocket) : mNetworkSocket(iNetworkSocket) {

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
   const ReceiveMessage* packet = mReceivedQueue.peek();
   if (packet == nullptr) {
      return -1;
   }
   return (*packet)->size();
}

//-------------------------------------------------------------------------------------------------

int32_t RfPacketSocket::send(const uint8_t* iPayload, size_t iSize, uint8_t iNodeId) {
   size_t sendSize = std::min(iSize,payloadCapacity());
   std::promise<bool> promise;
   auto future = promise.get_future();
   std::shared_ptr<I_RfNetworkSocket::Packet> packet(new I_RfNetworkSocket::Packet());
   packet->writePayload(iPayload,sendSize);
   packet->destination(iNodeId);

   mSendQueue.push(SendMessage(std::move(promise),packet));
   if(future.get()) {
      return sendSize;
   }
   return -1;
}

//-------------------------------------------------------------------------------------------------

int32_t RfPacketSocket::receive(uint8_t* oPayload, size_t iMaxSize, uint8_t* oNodeId) {
   ReceiveMessage packet;
   mReceivedQueue.pop(packet);
   size_t readSize = std::min(iMaxSize,packet->size());
   memcpy(oPayload, packet->payload(), readSize);
   if(oNodeId != nullptr) {
      *oNodeId = packet->source();
   }
   return readSize;
}

//-------------------------------------------------------------------------------------------------

void RfPacketSocket::workcycle() {

   if(mNetworkSocket.available()) {
      ReceiveMessage packet(new I_RfNetworkSocket::Packet);
      mNetworkSocket.receive(*packet);
      mReceivedQueue.push(packet);
   }

   SendMessage sendPacket;
   if(mSendQueue.tryPop(sendPacket)) {
      BT_LOG(DEBUG) << "send a packet of size " << sendPacket.second->size() ;
      bool sendResult = mNetworkSocket.send(*sendPacket.second);
      sendPacket.first.set_value(sendResult);
   }
}

//-------------------------------------------------------------------------------------------------

} // namespace Rf24
} // namespace Bt
