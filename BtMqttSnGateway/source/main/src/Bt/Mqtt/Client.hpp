//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::Client
//  
//*************************************************************************************************

#ifndef INC__Bt_Mqtt_Client__hpp
#define INC__Bt_Mqtt_Client__hpp

#include <memory>
#include <Bt/Rf24/I_RfPacketSocket.hpp>
#include <Bt/Net/Mqtt/MqttSn/Messages.hpp>

namespace Bt {
namespace Mqtt {

class Client : private Bt::Net::Mqtt::MqttSn::I_MessageVisitor
{
   public:
      Client(uint8_t iRfNodeId, std::shared_ptr<Rf24::I_RfPacketSocket> iSocket);
      ~Client();

      void handle(std::shared_ptr<Bt::Net::Mqtt::MqttSn::I_Message> iMessage);
   
   private:
   	  // Constructor to prohibit copy construction.
      Client(const Client&);

      // Operator= to prohibit copy assignment
      Client& operator=(const Client&);

      virtual void visit(Bt::Net::Mqtt::MqttSn::Connect& iMessage);
      virtual void visit(Bt::Net::Mqtt::MqttSn::Connack& iMessage);
      virtual void visit(Bt::Net::Mqtt::MqttSn::Register& iMessage);
      virtual void visit(Bt::Net::Mqtt::MqttSn::Regack& iMessage);
      virtual void visit(Bt::Net::Mqtt::MqttSn::Publish& iMessage);

      void send(Bt::Net::Mqtt::MqttSn::I_Message& iMessage);

      uint8_t mRfNodeId;
      std::shared_ptr<Rf24::I_RfPacketSocket> mSocket;
};

} // namespace Mqtt
} // namespace Bt

#endif // INC__Bt_Mqtt_Client__hpp
