//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::I_RfNetworkSocket
//  
//*************************************************************************************************

#ifndef INC__Bt_Rf24_I_RfNetworkSocket__hpp
#define INC__Bt_Rf24_I_RfNetworkSocket__hpp

#include <stdint.h>
#include <string.h>

#include "Bt/Rf24/I_Rf24Controller.hpp"

namespace Bt {
namespace Rf24 {

class I_RfNetworkSocket {
   public:

      class Packet;
      class I_Listener;
      virtual ~I_RfNetworkSocket() {}
      
      virtual bool send(Packet& iPacket) = 0;
      virtual bool receive(Packet& oPacket) = 0;
      virtual bool available() = 0;

      virtual bool setListener(I_Listener& iListener) = 0;
      virtual bool resetListener() = 0;


};

//-------------------------------------------------------------------------------------------------

class I_RfNetworkSocket::Packet {
   public:
      enum { HEADER_SIZE = 3 };
      enum { PAYLOAD_CAPACITY = I_Rf24Controller::Packet::BUFFER_CAPACITY - HEADER_SIZE };

      Packet() {
         mControllerPackage.size(HEADER_SIZE);
      }

      uint8_t source() {
         return mControllerPackage.buffer()[0];
      }

      uint8_t destination() {
         return mControllerPackage.buffer()[1];
      }

      uint8_t id() {
         return mControllerPackage.buffer()[2];
      }

      void destination(uint8_t destination) {
         mControllerPackage.buffer()[1] = destination;
      }

      const void* payload() {
         return payloadBuffer();
      }

      size_t size() {
         return mControllerPackage.size() - HEADER_SIZE;
      }

      size_t writePayload(const void* iBuffer, size_t iSize) {
         if (iSize > PAYLOAD_CAPACITY ) {
            iSize = PAYLOAD_CAPACITY;
         }
         memcpy(payloadBuffer(), iBuffer, iSize);
         mControllerPackage.size(iSize + HEADER_SIZE);
         return iSize;
      }

      size_t readPayload(void* oBuffer, size_t iMaxSize) {
         size_t readSize = size();
         if (readSize > iMaxSize) {
            readSize = iMaxSize;
         }
         memcpy(oBuffer, payload(), readSize);
         return readSize;
      }

      void copy(I_RfNetworkSocket::Packet& iPacket) const {
         mControllerPackage.copy(iPacket.mControllerPackage);
      }

      friend class RfNetworkSocket;

   private:
      void* payloadBuffer() {
         return mControllerPackage.buffer() + HEADER_SIZE;
      }

      void source(uint8_t pSource) {
         mControllerPackage.buffer()[0] = pSource;
      }

      void id(uint8_t pId) {
         mControllerPackage.buffer()[2] = pId;
      }



      I_Rf24Controller::Packet mControllerPackage;
};

//-------------------------------------------------------------------------------------------------

class I_RfNetworkSocket::I_Listener {
   public:
      virtual ~I_Listener() {}

      virtual void packetReceived(Packet& pPacket) = 0;

};

//-------------------------------------------------------------------------------------------------



} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_I_RfNetworkSocket__hpp
