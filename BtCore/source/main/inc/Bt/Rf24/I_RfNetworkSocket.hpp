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
      virtual ~I_RfNetworkSocket() {}
      
      virtual bool send(Packet& iPacket) = 0;

      virtual void startListening(std::function<void(Packet& iPacket)> iCallback) = 0;
      virtual void stopListening() = 0;


};

//-------------------------------------------------------------------------------------------------

class I_RfNetworkSocket::Packet {
   public:
      enum { HEADER_SIZE = 3 };
      enum { PAYLOAD_CAPACITY = I_Rf24DeviceController::Packet::CAPACITY - HEADER_SIZE };

      Packet() {
         mControllerPackage.size(HEADER_SIZE);
      }

      explicit Packet(I_Rf24DeviceController::Packet& iControllerPackage) {
         mControllerPackage = iControllerPackage;
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

      size_t size() const {
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

      void source(uint8_t iSource) {
         mControllerPackage.buffer()[0] = iSource;
      }

      void id(uint8_t iId) {
         mControllerPackage.buffer()[2] = iId;
      }

      I_Rf24DeviceController::Packet mControllerPackage;
};

//-------------------------------------------------------------------------------------------------

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_I_RfNetworkSocket__hpp
