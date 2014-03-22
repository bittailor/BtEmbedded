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

enum class ReturnCode : uint8_t {
   ACCEPTED = 0x00,
   REJECTED_CONGESTION,
   REJECTED_INVALID_TOPIC_ID,
   REJECTED_NOT_SUPPORTED,
};

Log::I_Logger& operator<<(Log::I_Logger& iLogger, ReturnCode iReturnCode) {
   iLogger << static_cast<uint8_t>(iReturnCode);
   return iLogger;
}

union Flags {
      uint8_t byte;
      struct
      {
            bool dup             : 1;
            uint8_t qos          : 2;
            bool retain          : 1;
            bool will            : 1;
            bool cleanSession    : 1;
            uint8_t topicIdType  : 2;
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
         flags.bits.dup = false;
         flags.bits.qos = 0;
         flags.bits.retain = false;
         flags.bits.will = false; //TODO (BT) implement support for will topic!
         flags.bits.cleanSession = true;
         flags.bits.topicIdType = 0;
         protocolId = PROTOCOL_ID_1_2;
         duration = 0xFFFF; //TODO (BT) implement support duration of Keep Alive timer
      }

      void setClientId(const char* pClientId) {
         strncpy(clientId, pClientId, I_MqttSnClient::MAX_LENGTH_CLIENT_ID);
         header.length = sizeof(Connect) + strlen(pClientId);
      }

      void setDuration(uint16_t pDuration) {
         duration = Util::Endianness::hostToNetwork(pDuration);
      }

      uint16_t getDuration() {
         return Util::Endianness::networkToHost(duration);
      }

};

struct Connack {
      Header      header;
      ReturnCode  returnCode;
};

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
//   uint8_t buffer[Rf24::I_RfPacketSocket::PAYLOAD_CAPACITY] = {0};
//
//   Connect* connect = reinterpret_cast<Connect*>(buffer);
//   connect->initialize();
//   connect->setClientId(mClientId);
//   if (!mSocket->send(buffer, connect->header.length, mGatewayNodeId))
//   {
//      BT_UTIL_LOG_ERROR("send CONNECT failed");
//      return false;
//   }
//
//   uint8_t receiveNodeId;
//   int32_t size;
//
//   do {
//      size = mSocket->receive(buffer, Rf24::I_RfPacketSocket::PAYLOAD_CAPACITY, &receiveNodeId);
//   } while(receiveNodeId == mGatewayNodeId);
//
//   if (size < 0) {
//      BT_UTIL_LOG_ERROR("send CONNACK failed");
//      return false;
//   }
//
//   Connack* connack = reinterpret_cast<Connack*>(buffer);
//   if (size != connack->header.length) {
//      BT_UTIL_LOG_ERROR("CONNACK packet invalid size " << size << "!=" << connack->header.length);
//   }
//   if (connack->returnCode == ReturnCode::ACCEPTED) {
//      return true;
//   }
//
//   BT_UTIL_LOG_ERROR("connect failed with ReturnCode " << connack->returnCode );
//
//   return false;
   return false;
}

//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Net
} // namespace Bt
