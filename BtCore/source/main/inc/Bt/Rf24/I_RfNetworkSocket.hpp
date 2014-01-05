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

namespace Bt {
namespace Rf24 {

class I_RfNetworkSocket {
   public:

      class Packet;
      class I_Listener;

      virtual ~I_RfNetworkSocket() {}
      
      virtual bool startListening(I_Listener& pListener) = 0;
      virtual bool stopListening() = 0;

      virtual bool send(Packet& pPacket) = 0;
};

//-------------------------------------------------------------------------------------------------

class I_RfNetworkSocket::Packet {
   public:
      enum { HEADER_SIZE = 2 };
      enum { PAYLOAD_CAPACITY = I_Rf24Controller::Packet::BUFFER_CAPACITY - HEADER_SIZE };


      uint8_t source() {
         return mControllerPackage.buffer()[0];
      }

      uint8_t destination() {
         return mControllerPackage.buffer()[1];
      }

      void destination(uint8_t destination) {
         mControllerPackage.buffer()[1] = destination;
      }

      uint8_t* payload() {
         return mControllerPackage.buffer()+2;
      }

      size_t writePayload(uint8_t* pBuffer, size_t pSize) {
         if (pSize > PAYLOAD_CAPACITY ) {
            pSize = PAYLOAD_CAPACITY;
         }
         memcpy(payload(), pBuffer, pSize);
         mControllerPackage.size(pSize + HEADER_SIZE);
         return pSize;
      }

      friend class RfNetworkSocket;

   private:
      void source(uint8_t source) {
         mControllerPackage.buffer()[0] = source;
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
