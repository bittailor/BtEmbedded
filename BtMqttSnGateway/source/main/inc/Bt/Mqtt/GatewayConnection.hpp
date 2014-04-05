//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::GatewayConnection
//  
//*************************************************************************************************

#ifndef INC__Bt_Mqtt_GatewayConnection__hpp
#define INC__Bt_Mqtt_GatewayConnection__hpp

#include <memory>
#include <functional>

#include <Bt/Rf24/I_RfPacketSocket.hpp>
#include <Bt/Net/MqttSn/Messages.hpp>
#include <Bt/Net/Mqtt/MqttClient.hpp>

#include "Bt/Mqtt/TopicStorage.hpp"
#include "Bt/Mqtt/I_MqttFactory.hpp"

using Bt::Net::Mqtt::MqttClient;

namespace Bt {
namespace Mqtt {

class GatewayConnection : private Bt::Net::MqttSn::I_MessageVisitor, private Bt::Net::Mqtt::I_MqttClient::I_Listener
{
   public:
      GatewayConnection(uint8_t iRfNodeId,
                        std::shared_ptr<Rf24::I_RfPacketSocket> iSocket,
                        std::shared_ptr<I_MqttFactory> iFactory,
                        std::function<bool(int)> iDispose);
      ~GatewayConnection();

      int id();

      void handle(std::shared_ptr<Bt::Net::MqttSn::I_Message> iMessage);
   
   private:
   	  // Constructor to prohibit copy construction.
      GatewayConnection(const GatewayConnection&);

      // Operator= to prohibit copy assignment
      GatewayConnection& operator=(const GatewayConnection&);

      virtual bool messageArrived(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage);

      virtual void visit(Bt::Net::MqttSn::Connect& iMessage);
      virtual void visit(Bt::Net::MqttSn::Connack& iMessage);
      virtual void visit(Bt::Net::MqttSn::Register& iMessage);
      virtual void visit(Bt::Net::MqttSn::Regack& iMessage);
      virtual void visit(Bt::Net::MqttSn::Publish& iMessage);
      virtual void visit(Bt::Net::MqttSn::Disconnect& iMessage);

      void send(Bt::Net::MqttSn::I_Message& iMessage);

      uint8_t mRfNodeId;
      std::shared_ptr<Rf24::I_RfPacketSocket> mSocket;
      std::shared_ptr<I_MqttFactory> mFactory;
      std::function<bool(int)> mDispose;
      std::shared_ptr<Net::Mqtt::I_MqttClient> mBrokerClient;
      TopicStorage mTopicStorage;
};

} // namespace Mqtt
} // namespace Bt

#endif // INC__Bt_Mqtt_GatewayConnection__hpp
