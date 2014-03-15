//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::Mqtt::MqttSnClient
//  
//*************************************************************************************************

#include "Bt/Net/Mqtt/MqttSnClient.hpp"

#include <string.h>
#include <Bt/Util/Endianness.hpp>
#include <Bt/Util/Logging.hpp>

namespace Bt {
namespace Net {
namespace Mqtt {

namespace {

enum ProtocolId {
      PROTOCOL_ID_1_2 = 0x01
};

enum class MsgType : uint8_t {
      ADVERTISE = 0x00,
      SEARCHGW,
      GWINFO,
      CONNECT = 0x04,
      CONNACK,
      WILLTOPICREQ,
      WILLTOPIC,
      WILLMSGREQ,
      WILLMSG,
      REGISTER,
      REGACK,
      PUBLISH,
      PUBACK,
      PUBCOMP,
      PUBREC,
      PUBREL,
      SUBSCRIBE = 0x12,
      SUBACK,
      UNSUBSCRIBE,
      UNSUBACK,
      PINGREQ,
      PINGRESP,
      DISCONNECT,
      WILLTOPICUPD = 0x1a,
      WILLTOPICRESP,
      WILLMSGUPD,
      WILLMSGRESP
};

union Flags {
      uint8_t byte;
      struct
      {
            bool dup             : 1;
            uint8_t qos          : 2;
            bool retain          : 1;
            bool will            : 1;
            bool cleanSession    : 1;
            uint8_t TopicIdType  : 2;
      } bits;
};

struct Header {
      uint8_t length;
      MsgType msgType;
};

struct Connect {
      Header header;
      Flags flags;
      uint8_t protocolId;
      uint16_t duration;
      char clientId[0];

      void initialize() {
         header.length = sizeof(Connect);
         header.msgType = MsgType::CONNECT;
         protocolId = PROTOCOL_ID_1_2;
         duration = 0xFFFF;
      }

      void setClientId(const char* pClientId) {
         strncpy(clientId, pClientId, I_MqttSnClient::MAX_LENGTH_CLIENT_ID);
         header.length = sizeof(Connect) + strlen(pClientId) - 1;
      }

      void setDuration(uint16_t pDuration) {
         duration = Util::Endianness::hostToNetwork(pDuration);
      }

      uint16_t getDuration() {
         return Util::Endianness::networkToHost(duration);
      }

};



/*
class Message {
   public:
      Message(void* pBuffer) : mBuffer((uint8_t*)pBuffer) {
      }

      void length(uint8_t pLength) {
         mBuffer[0] = pLength;
      }

      uint8_t lenght() {
         return mBuffer[0];
      }

      void msgType(MsgType pMsgType) {
         mBuffer[1] = static_cast<uint8_t>(pMsgType);
      }

      MsgType msgType() {
         return static_cast<MsgType>(mBuffer[1]);
      }
   protected:
      void* bufferAt(size_t nIndex) {
         return &mBuffer[nIndex];
      }

      template<typename T>
      T* bufferAt(size_t nIndex) {
         return static_cast<T*>(bufferAt(nIndex));
      }

      uint8_t* mBuffer;

};

class Connect : public Message {

   public:
      Connect(void* pBuffer) : Message(pBuffer) {
         msgType(MsgType::CONNECT);
         mBuffer[3] = PROTOCOL_ID_1_2;
      }

      void flags(uint8_t pFlags) {
         mBuffer[2] = pFlags;
      }

      uint8_t flags() {
         return mBuffer[2];
      }

      void duration(uint16_t pDuration) {
         *bufferAt<uint16_t>(4) = Util::Endianness::hostToNetwork(pDuration);
      }

      uint16_t duration() {
         return Util::Endianness::networkToHost(*bufferAt<uint16_t>(4));
      }

      void clientId(const char* pClientId) {
         strncpy(bufferAt<char>(6), pClientId, I_MqttSnClient::MAX_LENGTH_CLIENT_ID);
      }

      const char* clientId() {
         return bufferAt<char>(6);
      }

};
*/

} // namespace




//-------------------------------------------------------------------------------------------------

MqttSnClient::MqttSnClient(Rf24::I_RfPacketSocket& pSocket, uint8_t pGatewayNodeId, const char* pClientId)
: mSocket(&pSocket), mGatewayNodeId(pGatewayNodeId) {
   strncpy(mClientId, pClientId, MAX_LENGTH_CLIENT_ID);

}

//-------------------------------------------------------------------------------------------------

MqttSnClient::~MqttSnClient() {

}

//-------------------------------------------------------------------------------------------------

bool MqttSnClient::connect() {
   Rf24::I_RfPacketSocket::Packet connectPacket;
   Connect* connect = static_cast<Connect*>(connectPacket.payload());
   connect->initialize();
   connect->setClientId(mClientId);

   connectPacket.size(connect->header.length);
   if (!mSocket->send(connectPacket))
   {
      BT_UTIL_LOG_ERROR("send failed");
      return false;
   }





   return true;
}

//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Net
} // namespace Bt
