//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::MqttSn::Messages
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_MqttSn_Messages__hpp
#define INC__Bt_Net_MqttSn_Messages__hpp

#include <ios>
#include <cstdint>
#include <deque>
#include <string>
#include <Bt/Util/Endianness.hpp>
#include <vector>

namespace Bt {
namespace Net {
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

std::ostream& operator<<(std::ostream& iStream, MsgType iMsgType);


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


class I_PacketReader {
   public:
      virtual ~I_PacketReader(){}

      virtual std::streamsize readIn(int8_t& oValue) = 0;
      virtual std::streamsize readIn(uint8_t& oValue) = 0;
      virtual std::streamsize readIn(uint16_t& oValue) = 0;
      virtual std::streamsize readIn(std::string& oValue) = 0;
      virtual std::streamsize size() = 0;

      template<typename T>
      T read() {
         T value;
         readIn(value);
         return value;
      }

};


template<template <typename T, typename... A> class Container, typename... Rest>
class PacketReader : public I_PacketReader {
   public:

      PacketReader(Container<uint8_t>& container)
      : mContainer(container), mReadPosition(0) {

      }

      virtual std::streamsize readIn(int8_t& oValue) {
         return readBytes(reinterpret_cast<uint8_t*>(&oValue), sizeof(int8_t));
      }

      virtual std::streamsize readIn(uint8_t& oValue) {
         return readBytes(&oValue, sizeof(int8_t));
      }

      virtual std::streamsize readIn(uint16_t& oValue) {
         std::streamsize result = readBytes(reinterpret_cast<uint8_t*>(&oValue), sizeof(uint16_t));
         oValue = Util::Endianness::networkToHost<uint16_t>(oValue);
         return result;
      }

      virtual std::streamsize readIn(std::string& oValue) {
         oValue.clear();
         std::streamsize result = size();
         if (result == 0) {
            return -1; // EOF
         }
         oValue.append(mContainer.begin() + mReadPosition, mContainer.end());
         mReadPosition = mContainer.size();
         return result;
      }

      virtual std::streamsize size() {
         return static_cast<std::streamsize>(mContainer.size() - mReadPosition);
      }

      std::streamsize readBytes(uint8_t* s, std::streamsize n) {
         std::streamsize amt = size();
         std::streamsize result = (std::min)(n, amt);
         if (result != 0) {
            std::copy( mContainer.begin() + mReadPosition,
                       mContainer.begin() + mReadPosition + result,
                       s );
            mReadPosition += result;
            return result;
         } else {
            return -1; // EOF
         }
      }

      Container<uint8_t, Rest...>& container() { return mContainer; }

   private:

      Container<uint8_t, Rest...>&  mContainer;
      typename Container<uint8_t, Rest...>::size_type   mReadPosition;
};

class I_PacketWriter {
   public:
      virtual ~I_PacketWriter(){}

      virtual std::streamsize write(int8_t iValue) = 0;
      virtual std::streamsize write(uint8_t iValue) = 0;
      virtual std::streamsize write(uint16_t iValue) = 0;
      virtual std::streamsize write(const std::string& iValue) = 0;
      virtual std::streamsize size() = 0;

};


template<template <typename T, typename... A> class Container, typename... Rest>
class PacketWriter : public I_PacketWriter {
   public:

      PacketWriter(Container<uint8_t>& container)
      : mContainer(container) {

      }

      virtual std::streamsize write(int8_t iValue) {
         return writeBytes(reinterpret_cast<uint8_t*>(&iValue), sizeof(int8_t));
      }

      virtual std::streamsize write(uint8_t iValue) {
         return writeBytes(&iValue, sizeof(int8_t));
      }

      virtual std::streamsize write(uint16_t iValue) {
         iValue = Util::Endianness::hostToNetwork<uint16_t>(iValue);
         return writeBytes(reinterpret_cast<uint8_t*>(&iValue), sizeof(uint16_t));
      }

      virtual std::streamsize write(const std::string& iValue) {
         return writeBytes(reinterpret_cast<const uint8_t*>(iValue.c_str()), iValue.size());
      }

      virtual std::streamsize size() {
         return static_cast<std::streamsize>(mContainer.size());
      }

      std::streamsize writeBytes(const uint8_t* s, std::streamsize n)
      {
         mContainer.insert(mContainer.end(), s, s + n);
         return n;
      }


      Container<uint8_t, Rest...>& container() { return mContainer; }

   private:

      Container<uint8_t, Rest...>&  mContainer;
};


//class PacketReader {
//   public:
//
//      PacketReader(std::vector<uint8_t>& iBuffer) : mBuffer(iBuffer), mPosition(0) {
//
//      }
//
//      size_t size() { return mBuffer.size() - mPosition; }
//
//
//      void readIn(int8_t& oValue) {
//         oValue = *(mBuffer.data() + mPosition);
//         mPosition++;
//      }
//
//      void readIn(uint8_t& oValue) {
//         oValue = *(mBuffer.data() + mPosition);
//         mPosition++;
//      }
//
//      void readIn(uint16_t& oValue) {
//         oValue = Util::Endianness::networkToHost<uint16_t>(*reinterpret_cast<uint16_t*>(mBuffer.data() + mPosition));
//         mPosition += 2;
//      }
//
//      void readIn(std::string& oValue) {
//         oValue = *reinterpret_cast<char*>(mBuffer.data() + mPosition);
//         mPosition += oValue.size() + 1;
//      }
//
//      template<typename T>
//      T read() {
//         T value;
//         readIn(value);
//         return value;
//      }
//
//
//   private:
//      std::vector<uint8_t>& mBuffer;
//      size_t mPosition;
//};
//
//class PacketWriter {
//   public:
//
//      PacketWriter() {
//
//      }
//
//      size_t size() { return mBuffer.size(); }
//
//
//      void write(int8_t iValue) {
//         mBuffer.push_back(iValue);
//      }
//
//      void write(uint8_t& iValue) {
//         mBuffer.push_back(iValue);
//      }
//
//      void readIn(uint16_t& oValue) {
//         oValue = Util::Endianness::networkToHost<uint16_t>(*reinterpret_cast<uint16_t*>(mBuffer.data() + mPosition));
//         mPosition += 2;
//      }
//
//      void readIn(std::string& oValue) {
//         oValue = *reinterpret_cast<char*>(mBuffer.data() + mPosition);
//         mPosition += oValue.size() + 1;
//      }
//
//      template<typename T>
//      T read() {
//         T value;
//         readIn(value);
//         return value;
//      }
//
//
//   private:
//      std::vector<uint8_t>& mBuffer;
//};


class I_MessageVisitor;

class I_Message {
   public:
      virtual ~I_Message(){}
      virtual void write(I_PacketWriter& iWriter) const = 0;
      virtual void accept(I_MessageVisitor& iVistor) = 0;
};

class Connect;
class Connack;
class Register;
class Regack;
class Publish;
class Disconnect;
class Subscribe;
class Suback;
class Pingreq;
class Pingresp;

class I_MessageVisitor {
   public:
      virtual ~I_MessageVisitor(){}
      virtual void visit(Connect& iMessage) = 0;
      virtual void visit(Connack& iMessage) = 0;
      virtual void visit(Register& iMessage) = 0;
      virtual void visit(Regack& iMessage) = 0;
      virtual void visit(Publish& iMessage) = 0;
      virtual void visit(Disconnect& iMessage) = 0;
      virtual void visit(Subscribe& iMessage) = 0;
      virtual void visit(Suback& iMessage) = 0;
      virtual void visit(Pingreq& iMessage) = 0;
      virtual void visit(Pingresp& iMessage) = 0;
};

class Connect : public I_Message {
   public:
      Flags flags;
      uint16_t duration;
      std::string clientId;

      Connect(I_PacketReader& reader) {
         flags.byte = reader.read<uint8_t>();
         reader.read<uint8_t>(); // ProtocolId
         duration = reader.read<uint16_t>();
         clientId = reader.read<std::string>();
      }

      Connect(std::string iClientId) {
         flags.bits.dup = false;
         flags.bits.qos = 0;
         flags.bits.retain = false;
         flags.bits.will = false; //TODO (BT) implement support for will topic!
         flags.bits.cleanSession = true;
         flags.bits.topicIdType = 0;
         duration = 0xFFFF; //TODO (BT) implement support duration of Keep Alive timer
         clientId = std::move(iClientId);
      }

      virtual void write(I_PacketWriter& iWriter) const {
         uint8_t length = 6 + clientId.size();
         iWriter.write(length);
         iWriter.write(static_cast<uint8_t>(MsgType::CONNECT));
         iWriter.write(flags.byte);
         iWriter.write(static_cast<uint8_t>(PROTOCOL_ID_1_2));
         iWriter.write(duration);
         iWriter.write(clientId);
      }

      virtual void accept(I_MessageVisitor& iVistor) {
         iVistor.visit(*this);
      }

   private:

};

class Connack : public I_Message {
   public:
      ReturnCode  returnCode;

      Connack(ReturnCode iReturnCode) : returnCode(iReturnCode) {
      }

      Connack(I_PacketReader& reader) {
         returnCode = static_cast<ReturnCode>(reader.read<uint8_t>());
      }

      virtual void write(I_PacketWriter& iWriter) const {
         uint8_t length = 3;
         iWriter.write(length);
         iWriter.write(static_cast<uint8_t>(MsgType::CONNACK));
         iWriter.write(static_cast<uint8_t>(returnCode));
      }

      virtual void accept(I_MessageVisitor& iVistor) {
         iVistor.visit(*this);
      }

   private:
};

class Register : public I_Message {
   public:
      uint16_t topicId;
      uint16_t msgId;
      std::string topicName;

      Register(I_PacketReader& reader) {
         topicId = reader.read<uint16_t>();
         msgId = reader.read<uint16_t>();
         topicName = reader.read<std::string>();
      }

      Register(uint16_t iTopicId, uint16_t iMsgId, std::string iTopicName)
      : topicId(iTopicId), msgId(iMsgId), topicName(std::move(iTopicName))  {
      }

      virtual void write(I_PacketWriter& iWriter) const {
         uint8_t length = 6 + topicName.length();
         iWriter.write(length);
         iWriter.write(static_cast<uint8_t>(MsgType::REGISTER));
         iWriter.write(topicId);
         iWriter.write(msgId);
         iWriter.write(topicName);
      }

      virtual void accept(I_MessageVisitor& iVistor) {
         iVistor.visit(*this);
      }

   private:
};

class Regack : public I_Message {
   public:
      uint16_t topicId;
      uint16_t msgId;
      ReturnCode returnCode;

      Regack(uint16_t iTopicId, uint16_t iMsgId, ReturnCode iReturnCode)
      : topicId(iTopicId), msgId(iMsgId), returnCode(iReturnCode) {

      }

      Regack(I_PacketReader& reader) {
         topicId = reader.read<uint16_t>();
         msgId = reader.read<uint16_t>();
         returnCode = static_cast<ReturnCode>(reader.read<uint8_t>());
      }

      virtual void write(I_PacketWriter& iWriter) const {
         uint8_t length = 7 ;
         iWriter.write(length);
         iWriter.write(static_cast<uint8_t>(MsgType::REGACK));
         iWriter.write(topicId);
         iWriter.write(msgId);
         iWriter.write(static_cast<uint8_t>(returnCode));
      }

      virtual void accept(I_MessageVisitor& iVistor) {
         iVistor.visit(*this);
      }

   private:
};

class Publish : public I_Message {
   public:
      Flags flags;
      uint16_t topicId;
      uint16_t msgId;
      std::string data;

      Publish(I_PacketReader& reader) {
         flags.byte = reader.read<uint8_t>();
         topicId = reader.read<uint16_t>();
         msgId = reader.read<uint16_t>();
         data = reader.read<std::string>();
      }

      Publish(uint8_t iQos, bool iRetain, uint16_t iTopicId, uint16_t iMsgId, std::string iData)
      : topicId(iTopicId), msgId(iMsgId), data(std::move(iData)) {
         flags.bits.dup = false;
         flags.bits.qos = iQos;
         flags.bits.retain = iRetain;
         flags.bits.will = false; // not used by Publish
         flags.bits.cleanSession = false; // not used by Publish
         flags.bits.topicIdType = 0x00;
      }

      virtual void write(I_PacketWriter& iWriter) const {
         uint8_t length = 7 + data.size();
         iWriter.write(length);
         iWriter.write(static_cast<uint8_t>(MsgType::PUBLISH));
         iWriter.write(flags.byte);
         iWriter.write(topicId);
         iWriter.write(msgId);
         iWriter.write(data);
      }

      virtual void accept(I_MessageVisitor& iVistor) {
         iVistor.visit(*this);
      }

   private:
};

class Disconnect : public I_Message {
   public:
      bool withDuration;
      uint16_t duration;

      Disconnect() : withDuration(false), duration(0) {
      }

      Disconnect(I_PacketReader& iReader) {
         if (iReader.size() == 0) {
            withDuration = false;
            return;
         }
         withDuration = true;
         duration = iReader.read<uint16_t>();
      }

      virtual void write(I_PacketWriter& iWriter) const {
         uint8_t length = 2;

         if(withDuration) {
            length = 4;
         }

         iWriter.write(length);
         iWriter.write(static_cast<uint8_t>(MsgType::DISCONNECT));
         if(withDuration) {
            iWriter.write(duration);
         }
      }

      virtual void accept(I_MessageVisitor& iVistor) {
         iVistor.visit(*this);
      }

   private:
};

class Subscribe : public I_Message {
   public:
      Flags flags;
      uint16_t msgId;
      std::string topicName;

      Subscribe(I_PacketReader& reader) {
         flags.byte = reader.read<uint8_t>();
         msgId = reader.read<uint16_t>();
         topicName = reader.read<std::string>();
      }

      virtual void write(I_PacketWriter& iWriter) const {
         uint8_t length = 5 + topicName.size() ;
         iWriter.write(length);
         iWriter.write(static_cast<uint8_t>(MsgType::SUBSCRIBE));
         iWriter.write(flags.byte);
         iWriter.write(msgId);
         iWriter.write(topicName);
      }

      virtual void accept(I_MessageVisitor& iVistor) {
         iVistor.visit(*this);
      }

   private:
};

class Suback : public I_Message {
   public:
      Flags flags;
      uint16_t topicId;
      uint16_t msgId;
      ReturnCode returnCode;

      Suback(uint8_t iQos, uint16_t iTopicId, uint16_t iMsgId, ReturnCode iReturnCode) {
         flags.bits.dup = false;
         flags.bits.qos = iQos;
         flags.bits.retain = false; // not used by Suback
         flags.bits.will = false; // not used by Suback
         flags.bits.cleanSession = false; // not used by Suback
         flags.bits.topicIdType = 0; // not used by Suback
         topicId = iTopicId;
         msgId = iMsgId;
         returnCode = ReturnCode::ACCEPTED;
      }

      Suback(I_PacketReader& reader) {
         flags.byte = reader.read<uint8_t>();
         topicId = reader.read<uint16_t>();
         msgId = reader.read<uint16_t>();
         returnCode = static_cast<ReturnCode>(reader.read<uint8_t>());
      }

      virtual void write(I_PacketWriter& iWriter) const {
         uint8_t length = 8 ;
         iWriter.write(length);
         iWriter.write(static_cast<uint8_t>(MsgType::SUBACK));
         iWriter.write(flags.byte);
         iWriter.write(topicId);
         iWriter.write(msgId);
         iWriter.write(static_cast<uint8_t>(returnCode));
      }

      virtual void accept(I_MessageVisitor& iVistor) {
         iVistor.visit(*this);
      }

   private:
};

class Pingreq : public I_Message {
   public:
      bool withClientId;
      std::string clientId;

      Pingreq(I_PacketReader& iReader) {
         if (iReader.size() == 0) {
            withClientId = false;
            return;
         }
         withClientId = true;
         clientId = iReader.read<std::string>();
      }

      virtual void write(I_PacketWriter& iWriter) const {
         uint8_t length = 2 ;
         iWriter.write(length);
         iWriter.write(static_cast<uint8_t>(MsgType::PINGREQ));
      }

      virtual void accept(I_MessageVisitor& iVistor) {
         iVistor.visit(*this);
      }
};

class Pingresp : public I_Message {
   public:
      Pingresp() {
      }

      Pingresp(I_PacketReader& reader) {
      }

      virtual void write(I_PacketWriter& iWriter) const {
         uint8_t length = 2 ;
         iWriter.write(length);
         iWriter.write(static_cast<uint8_t>(MsgType::PINGRESP));
      }

      virtual void accept(I_MessageVisitor& iVistor) {
         iVistor.visit(*this);
      }
};









} // namespace MqttSn
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_MqttSn_Messages__hpp
