//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RfNetworkSocket
//  
//*************************************************************************************************

#include <string.h>

#include "Bt/Net/RfNetworkSocket.hpp"

namespace Bt {
namespace Net {

class RfNetworkSocket::NetworkPacket {
   public:
      NetworkPacket(uint8_t* pBuffer) : mBuffer(pBuffer), mPayloadSize(0) {

      }

      uint8_t source() {
         return mBuffer[0];
      }

      void source(uint8_t source) {
         mBuffer[0] = source;
      }

      uint8_t destination() {
         return mBuffer[1];
      }

      void destination(uint8_t destination) {
         mBuffer[1] = destination;
      }

      void writePayload(uint8_t* pBuffer, size_t pSize) {
         memcpy(payload(), pBuffer, pSize);
         mPayloadSize = pSize;
      }

      uint8_t* payload() {
         return mBuffer+2;
      }

      uint8_t* buffer() {
         return mBuffer;
      }

      uint8_t bufferSize() {
         return mPayloadSize + RfNetworkSocket::HEADER_SIZE;
      }

      void bufferSize(size_t pSize) {
         mPayloadSize = pSize - RfNetworkSocket::HEADER_SIZE;
      }

   private:
      uint8_t* mBuffer;
      size_t mPayloadSize;

};

//-------------------------------------------------------------------------------------------------

RfNetworkSocket::RfNetworkSocket(RfNodeId pNodeId, Device::I_RfController& pController)
: mNodeId(pNodeId), mController(&pController)  {

}

//-------------------------------------------------------------------------------------------------

void RfNetworkSocket::workcycle() {
   while (mController->isDataAvailable()) {
      uint8_t buffer[FRAME_BUFFER_SIZE];
      NetworkPacket packet(buffer);
      packet.bufferSize(mController->read(buffer,FRAME_BUFFER_SIZE));
   }
}

//-------------------------------------------------------------------------------------------------

void RfNetworkSocket::writeDatagram(RfNodeId pDestination, uint8_t* pBuffer, size_t pSize) {
   uint8_t buffer[FRAME_BUFFER_SIZE];
   NetworkPacket packet(buffer);
   packet.source(mNodeId.id());
   packet.destination(pDestination.id());
   packet.writePayload(pBuffer, pSize);
   write(packet);
}

//-------------------------------------------------------------------------------------------------

size_t RfNetworkSocket::write(NetworkPacket& packet) {
   Device::I_RfController::Pipe pipe = mRouting.calculateRoutingPipe(mNodeId, packet.destination());
   return mController->write(pipe, packet.buffer(), packet.bufferSize());
}

//-------------------------------------------------------------------------------------------------

} // namespace Net
} // namespace Bt
