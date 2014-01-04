//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::I_RfNetworkSocket
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_I_RfNetworkSocket__hpp
#define INC__Bt_Net_I_RfNetworkSocket__hpp

#include <stdint.h>

namespace Bt {
namespace Net {

class I_RfNetworkSocket {
   public:

      class Packet;

      virtual ~I_RfNetworkSocket() {}
      


};


class I_RfNetworkSocket::Packet {
   public:
      enum { HEADER_SIZE = 2 };
      enum { PAYLOAD_CAPACITY = Device::I_RfController::Packet::BUFFER_CAPACITY - HEADER_SIZE };


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


      Device::I_RfController::Packet mControllerPackage;
};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_I_RfNetworkSocket__hpp
