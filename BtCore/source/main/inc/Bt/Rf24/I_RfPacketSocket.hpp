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

      class Packet;
      class I_Listener;

      virtual ~I_RfPacketSocket() {}
      
      virtual bool send(Packet& pPacket) = 0;
};

//-------------------------------------------------------------------------------------------------

class I_RfPacketSocket::Packet {
   public:
      enum { HEADER_SIZE = 1 };
      enum { MAX_TRANSMISSION_UNIT = I_RfNetworkSocket::Packet::PAYLOAD_CAPACITY - HEADER_SIZE };
      enum { PAYLOAD_CAPACITY = MAX_TRANSMISSION_UNIT * 3 };

      Packet() : mSource(), mDestination(), mBuffer(), mSize() {

      }

      uint8_t source() {
         return mSource;
      }

      uint8_t destination() {
         return mDestination;
      }

      void destination(uint8_t pDestination) {
         mDestination = pDestination;
      }

      void* payload() {
         return mBuffer;
      }

      size_t size() {
         return mSize;
      }

      void size(size_t pSize) {
         mSize = pSize;
      }

   private:
      uint8_t mSource;
      uint8_t mDestination;
      uint8_t mBuffer[PAYLOAD_CAPACITY];
      size_t mSize;
};

//-------------------------------------------------------------------------------------------------

class I_RfPacketSocket::I_Listener {
   public:
      virtual ~I_Listener() {}

      virtual void packetReceived(Packet& pPacket) = 0;

};

//-------------------------------------------------------------------------------------------------




} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_I_RfPacketSocket__hpp
