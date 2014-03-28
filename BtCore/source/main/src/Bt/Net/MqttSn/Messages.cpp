//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::MqttSn::Messages
//  
//*************************************************************************************************

#include <type_traits>
#include <iostream>

#include "Bt/Net/MqttSn/Messages.hpp"

namespace Bt {
namespace Net {
namespace MqttSn {

std::ostream& operator<<(std::ostream& iStream, MsgType iMsgType) {
   switch (iMsgType) {
      case MsgType::ADVERTISE       : iStream << "ADVERTISE"; break;
      case MsgType::SEARCHGW        : iStream << "SEARCHGW"; break;
      case MsgType::GWINFO          : iStream << "GWINFO"; break;
      case MsgType::CONNECT         : iStream << "CONNECT"; break;
      case MsgType::CONNACK         : iStream << "CONNACK"; break;
      case MsgType::WILLTOPICREQ    : iStream << "WILLTOPICREQ"; break;
      case MsgType::WILLTOPIC       : iStream << "WILLTOPIC"; break;
      case MsgType::WILLMSGREQ      : iStream << "WILLMSGREQ"; break;
      case MsgType::WILLMSG         : iStream << "WILLMSG"; break;
      case MsgType::REGISTER        : iStream << "REGISTER"; break;
      case MsgType::REGACK          : iStream << "REGACK"; break;
      case MsgType::PUBLISH         : iStream << "PUBLISH"; break;
      case MsgType::PUBACK          : iStream << "PUBACK"; break;
      case MsgType::PUBCOMP         : iStream << "PUBCOMP"; break;
      case MsgType::PUBREC          : iStream << "PUBREC"; break;
      case MsgType::PUBREL          : iStream << "PUBREL"; break;
      case MsgType::SUBSCRIBE       : iStream << "SUBSCRIBE"; break;
      case MsgType::SUBACK          : iStream << "SUBACK"; break;
      case MsgType::UNSUBSCRIBE     : iStream << "UNSUBSCRIBE"; break;
      case MsgType::UNSUBACK        : iStream << "UNSUBACK"; break;
      case MsgType::PINGREQ         : iStream << "PINGREQ"; break;
      case MsgType::PINGRESP        : iStream << "PINGRESP"; break;
      case MsgType::DISCONNECT      : iStream << "DISCONNECT"; break;
      case MsgType::WILLTOPICUPD    : iStream << "WILLTOPICUPD"; break;
      case MsgType::WILLTOPICRESP   : iStream << "WILLTOPICRESP"; break;
      case MsgType::WILLMSGUPD      : iStream << "WILLMSGUPD"; break;
      case MsgType::WILLMSGRESP     : iStream << "WILLMSGRESP"; break;
   }

   return iStream;
}


//static_assert(std::is_standard_layout<Header>::value,"Header must be standard_layout");
//static_assert(std::is_standard_layout<Connect>::value,"Connack must be standard_layout");
//static_assert(std::is_standard_layout<Connack>::value,"Connack must be standard_layout");

} // namespace MqttSn
} // namespace Net
} // namespace Bt
