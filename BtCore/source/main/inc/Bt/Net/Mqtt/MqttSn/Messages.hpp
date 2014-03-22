//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::Mqtt::MqttSn::Messages
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_Mqtt_MqttSn_Messages__hpp
#define INC__Bt_Net_Mqtt_MqttSn_Messages__hpp

#include <stdint.h>
#include <string>
#include <Bt/Util/Endianness.hpp>

namespace Bt {
namespace Net {
namespace Mqtt {
namespace MqttSn {

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

class Header {
   public:
      Header(MsgType iMsgType, uint8_t iLength)
      : mLength(iLength), mMsgType(iMsgType) {

      }

      uint8_t length() { return mLength; }
      MsgType msgType() { return mMsgType; }

   private:
      uint8_t mLength;
      MsgType mMsgType;
};

class Connect {
   public:
      Connect(const std::string& iClientId)
      : mHeader(MsgType::CONNECT, sizeof(Connect) + iClientId.length()) {
         mFlags.bits.dup = false;
         mFlags.bits.qos = 0;
         mFlags.bits.retain = false;
         mFlags.bits.will = false; //TODO (BT) implement support for will topic!
         mFlags.bits.cleanSession = true;
         mFlags.bits.topicIdType = 0;
         mProtocolId = PROTOCOL_ID_1_2;
         mDuration = 0xFFFF; //TODO (BT) implement support duration of Keep Alive timer
      }

      uint16_t getDuration() {
         return Util::Endianness::networkToHost(mDuration);
      }

   private:
      Header mHeader;
      Flags mFlags;
      uint8_t mProtocolId;
      uint16_t mDuration;
      char mClientId[0];
};

class Connack {
   public:
      Connack(ReturnCode iReturnCode)
      : mHeader(MsgType::CONNACK,sizeof(Connack)), mReturnCode(iReturnCode) {

      }

      ReturnCode returnCode() { return mReturnCode; }

   private:
      Header      mHeader;
      ReturnCode  mReturnCode;
};




} // namespace MqttSn
} // namespace Mqtt
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Mqtt_MqttSn_Messages__hpp
