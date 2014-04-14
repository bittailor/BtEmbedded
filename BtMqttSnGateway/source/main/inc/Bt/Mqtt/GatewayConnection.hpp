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
#include <thread>
#include <functional>

#include <Bt/Rf24/I_RfPacketSocket.hpp>
#include <Bt/Net/MqttSn/Messages.hpp>
#include <Bt/Net/Mqtt/MqttClient.hpp>

#include "Bt/Concurrency/BlockingQueue.hpp"
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
      virtual void messageArrivedInternal(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage);

      virtual void visit(Bt::Net::MqttSn::Connect& iMessage);
      virtual void visit(Bt::Net::MqttSn::Connack& iMessage);
      virtual void visit(Bt::Net::MqttSn::Register& iMessage);
      virtual void visit(Bt::Net::MqttSn::Regack& iMessage);
      virtual void visit(Bt::Net::MqttSn::Publish& iMessage);
      virtual void visit(Bt::Net::MqttSn::Disconnect& iMessage);
      virtual void visit(Bt::Net::MqttSn::Subscribe& iMessage);
      virtual void visit(Bt::Net::MqttSn::Suback& iMessage);
      virtual void visit(Bt::Net::MqttSn::Pingreq& iMessage);
      virtual void visit(Bt::Net::MqttSn::Pingresp& iMessage);

      void send(Bt::Net::MqttSn::I_Message& iMessage);

      bool containsWildcardCharacters(const std::string& iTopicName);

      void workcycle();

      bool mRunning;
      uint8_t mRfNodeId;
      Bt::Concurrency::BlockingQueue<std::function<void()>> mQueue;
      std::shared_ptr<Rf24::I_RfPacketSocket> mSocket;
      std::shared_ptr<I_MqttFactory> mFactory;
      std::function<bool(int)> mDispose;
      std::shared_ptr<Net::Mqtt::I_MqttClient> mBrokerClient;
      TopicStorage mTopicStorage;
      uint16_t mMsgIdCounter;
      std::thread mThread;
};

} // namespace Mqtt
} // namespace Bt

#endif // INC__Bt_Mqtt_GatewayConnection__hpp
