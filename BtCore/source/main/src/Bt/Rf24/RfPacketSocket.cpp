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

RfPacketSocket::RfPacketSocket(I_RfNetworkSocket& iNetworkSocket, Concurrency::I_ExecutionContext& iExecutionContext)
: mNetworkSocket(iNetworkSocket), mExecutionContext(iExecutionContext) {
   mNetworkSocket.startListening(std::bind(&RfPacketSocket::onPacketReceived,this,std::placeholders::_1));
}

//-------------------------------------------------------------------------------------------------

RfPacketSocket::~RfPacketSocket() {
   mNetworkSocket.stopListening();
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
   const I_RfNetworkSocket::Packet* packet = mReceivedQueue.peek();
   if (packet == nullptr) {
      return -1;
   }
   return packet->size();
}

//-------------------------------------------------------------------------------------------------

int32_t RfPacketSocket::send(const uint8_t* iPayload, size_t iSize, uint8_t iNodeId) {
   size_t sendSize = std::min(iSize,payloadCapacity());

   I_RfNetworkSocket::Packet packet;
   packet.writePayload(iPayload,sendSize);
   packet.destination(iNodeId);

   std::future<bool> sendTask = mExecutionContext.invokeTask([&packet,this](){
      BT_LOG(DEBUG) << "send a packet of size " << packet.size() ;
      return mNetworkSocket.send(packet);
   });

   if(sendTask.get()) {
      return sendSize;
   }

   return -1;
}

//-------------------------------------------------------------------------------------------------

int32_t RfPacketSocket::receive(uint8_t* oPayload, size_t iMaxSize, uint8_t* oNodeId) {
   I_RfNetworkSocket::Packet packet;
   mReceivedQueue.pop(packet);
   size_t readSize = std::min(iMaxSize,packet.size());
   memcpy(oPayload, packet.payload(), readSize);
   if(oNodeId != nullptr) {
      *oNodeId = packet.source();
   }
   return readSize;
}

//-------------------------------------------------------------------------------------------------

void RfPacketSocket::onPacketReceived(I_RfNetworkSocket::Packet& iPacket) {
   mReceivedQueue.push(iPacket);
}

//-------------------------------------------------------------------------------------------------

} // namespace Rf24
} // namespace Bt
