//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::MqttSn::Message
//  
//*************************************************************************************************

#include "Bt/Net/MqttSn/Message.hpp"

#include <iostream>

#include <Bt/Log/Logging.hpp>

#include <Bt/Util/Range.hpp>
#include <Bt/Net/MqttSn/Messages.hpp>
#include <Bt/Util/Endianness.hpp>



namespace Bt {
namespace Net {
namespace MqttSn {

namespace {




//   template<typename T> T readIn(uint8_t* buffer);
//
//   template<> uint8_t  readIn<uint8_t>(uint8_t* buffer) {
//      return *buffer;
//   }
//
//   template<> uint16_t readIn<uint16_t>(uint8_t* buffer) {
//      return Util::Endianness::networkToHost<uint16_t>(*reinterpret_cast<uint16_t*>(buffer));
//   }

}


//-------------------------------------------------------------------------------------------------

MessageBuffer::MessageBuffer(size_t iCapacity) : mBuffer(iCapacity + 1, 0) {

}

//-------------------------------------------------------------------------------------------------

MessageBuffer::MessageBuffer(std::vector<uint8_t> iRawMessage) : mBuffer(std::move(iRawMessage)) {

}

//-------------------------------------------------------------------------------------------------

MessageBuffer::~MessageBuffer() {

}

//-------------------------------------------------------------------------------------------------

std::shared_ptr<I_Message> MessageBuffer::parse()  {
   PacketReader<std::vector> reader(mBuffer);
   size_t size = reader.read<uint8_t>();
   if(size == 0x01) {
      size = reader.read<uint16_t>();
   }

   mBuffer.resize(size);

   uint8_t rawMsgType = reader.read<uint8_t>();
   MsgType msgType = static_cast<MsgType>(rawMsgType);

   switch(msgType) {
//      case MsgType::ADVERTISE       : return nullptr;
//      case MsgType::SEARCHGW        : return nullptr;
//      case MsgType::GWINFO          : return nullptr;
      case MsgType::CONNECT         : return std::shared_ptr<I_Message>(new Connect(reader));
      case MsgType::CONNACK         : return std::shared_ptr<I_Message>(new Connack(reader));
//      case MsgType::WILLTOPICREQ    : iStream << "WILLTOPICREQ"; break;
//      case MsgType::WILLTOPIC       : iStream << "WILLTOPIC"; break;
//      case MsgType::WILLMSGREQ      : iStream << "WILLMSGREQ"; break;
//      case MsgType::WILLMSG         : iStream << "WILLMSG"; break;
      case MsgType::REGISTER        : return std::shared_ptr<I_Message>(new Register(reader));
      case MsgType::REGACK          : return std::shared_ptr<I_Message>(new Regack(reader));
      case MsgType::PUBLISH         : return std::shared_ptr<I_Message>(new Publish(reader));
//      case MsgType::PUBACK          : iStream << "PUBACK"; break;
//      case MsgType::PUBCOMP         : iStream << "PUBCOMP"; break;
//      case MsgType::PUBREC          : iStream << "PUBREC"; break;
//      case MsgType::PUBREL          : iStream << "PUBREL"; break;
      case MsgType::SUBSCRIBE       : return std::shared_ptr<I_Message>(new Subscribe(reader));
      case MsgType::SUBACK          : return std::shared_ptr<I_Message>(new Suback(reader));
//      case MsgType::UNSUBSCRIBE     : iStream << "UNSUBSCRIBE"; break;
//      case MsgType::UNSUBACK        : iStream << "UNSUBACK"; break;
      case MsgType::PINGREQ         : return std::shared_ptr<I_Message>(new Pingreq(reader));
      case MsgType::PINGRESP        : return std::shared_ptr<I_Message>(new Pingresp(reader));
      case MsgType::DISCONNECT      : return std::shared_ptr<I_Message>(new Disconnect(reader));
//      case MsgType::WILLTOPICUPD    : iStream << "WILLTOPICUPD"; break;
//      case MsgType::WILLTOPICRESP   : iStream << "WILLTOPICRESP"; break;
//      case MsgType::WILLMSGUPD      : iStream << "WILLMSGUPD"; break;
//      case MsgType::WILLMSGRESP     : iStream << "WILLMSGRESP"; break;
      default: break;
   }

   BT_LOG(WARNING) << msgType << "[" << static_cast<int>(rawMsgType) << "] not implemented or unknown" ;
   return nullptr;
}


//-------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& iStream, const MessageBuffer& iMessage) {
   size_t length = iMessage.buffer()[0];
   iStream << static_cast<MsgType>(iMessage.buffer()[1]) << "[" << length << "] " << std::endl << "   ";
   for(uint8_t byte : Util::range(iMessage.buffer(), iMessage.buffer() + length)) {
      iStream << std::hex << static_cast<uint32_t>(byte) << ",";
   }

   iStream << std::dec << std::endl << "   ";

   for(uint8_t byte : Util::range(iMessage.buffer(), iMessage.buffer() + length)) {
         iStream << static_cast<uint32_t>(byte) << ",";
   }

   iStream << std::dec << std::endl << "   ";

   for(uint8_t byte : Util::range(iMessage.buffer(), iMessage.buffer() + length)) {
      if(byte > 31) {
         iStream << static_cast<char>(byte);
      }
   }

   return iStream;
}

//-------------------------------------------------------------------------------------------------

} // namespace MqttSn
} // namespace Net
} // namespace Bt
