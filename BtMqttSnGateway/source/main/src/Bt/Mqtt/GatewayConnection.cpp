//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::GatewayConnection
//  
//*************************************************************************************************

#include "Bt/Mqtt/GatewayConnection.hpp"

#include <iostream>

#include <boost/log/trivial.hpp>

namespace Bt {
namespace Mqtt {


//-------------------------------------------------------------------------------------------------

GatewayConnection::GatewayConnection(uint8_t iRfNodeId,
                                     std::shared_ptr<Rf24::I_RfPacketSocket> iSocket,
                                     std::shared_ptr<I_MqttFactory> iFactory,
                                     std::function<bool(int)> iDispose)
: mRunning(true), mRfNodeId(iRfNodeId), mSocket(iSocket)
, mFactory(iFactory), mDispose(iDispose)
, mMsgIdCounter(1), mThread(&GatewayConnection::workcycle,this)
, mTimeTillNextKeepAlive(0){
   BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "GatewayConnection() ";
}

//-------------------------------------------------------------------------------------------------

GatewayConnection::~GatewayConnection() {
   mThread.detach();
   BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "~GatewayConnection() " ;
}

//-------------------------------------------------------------------------------------------------

int GatewayConnection::id() {
   return mRfNodeId;
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::handle(std::shared_ptr<Bt::Net::MqttSn::I_Message> iMessage) {
   mQueue.push([=](){iMessage->accept(*this);});
}

//-------------------------------------------------------------------------------------------------

bool GatewayConnection::messageArrived(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage) {
   std::string topicName = iTopicName;
   mQueue.push([=](){this->messageArrivedInternal(topicName,iMessage);});
   return true;
}
//-------------------------------------------------------------------------------------------------

void GatewayConnection::messageArrivedInternal(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage) {
   BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "PublishFromBroker: " << iTopicName << " " << iMessage->data ;

   uint16_t topicId = mTopicStorage.getTopicId(iTopicName);

   if(topicId == TopicStorage::NO_TOPIC_ID) {
      topicId = mTopicStorage.getOrCreateTopicId(iTopicName);
      uint16_t msgId = mMsgIdCounter++;
      Bt::Net::MqttSn::Register registerMessage(topicId, msgId, iTopicName);
      send(registerMessage);
   }

   uint16_t msgId = mMsgIdCounter++;

   Bt::Net::MqttSn::Publish publish(iMessage->qos, iMessage->retained, topicId, msgId, iMessage->data);
   send(publish);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Connect& iMessage) {
   BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "ConnectFromClient: " << iMessage.clientId ;

   mTimeTillNextKeepAlive = std::chrono::seconds(iMessage.duration + (iMessage.duration / 2));

   if(mBrokerClient) {
      BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "Reconnect without disconnect => flush topic storage" ;
      mBrokerClient->disconnect(1000);
      mTopicStorage.clear();
   }

   mBrokerClient = mFactory->createClient(*this, iMessage.clientId);

   bool result = mBrokerClient->connect(mFactory->createDefaultOptions());

   Bt::Net::MqttSn::ReturnCode returnCode = Bt::Net::MqttSn::ReturnCode::ACCEPTED;

   if(!result){
      returnCode = Bt::Net::MqttSn::ReturnCode::REJECTED_NOT_SUPPORTED;
   }

   Bt::Net::MqttSn::Connack connack(returnCode);
   send(connack);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Connack& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Register& iMessage) {
   BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "RegisterFromClient: " << iMessage.topicName ;

   uint16_t topicId = mTopicStorage.getOrCreateTopicId(iMessage.topicName);
   Bt::Net::MqttSn::Regack regack(topicId, iMessage.msgId , Bt::Net::MqttSn::ReturnCode::ACCEPTED);
   send(regack);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Regack& iMessage) {
   BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "RegackFromClient: " << iMessage.topicId ;
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Publish& iMessage) {
   BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "PublishFromClient: " << iMessage.topicId << " "  << iMessage.data ;
   if(!mBrokerClient) {
      BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "BrokerClient is null !" ;
      return;
   }


   std::string topicName = mTopicStorage.getTopicName(iMessage.topicId);
   if (topicName.empty()) {
      BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "PublishFromClient: could not find topic name for id " << iMessage.topicId ;
   }

   if(!mBrokerClient->publish(topicName, iMessage.data, iMessage.flags.bits.qos, iMessage.flags.bits.retain).get()) {
      BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "PublishFromClient : publish to broker failed ";
   }
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Disconnect& iMessage) {
   disconnect(true);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::disconnect(bool iSendDisconnectToClient) {
   BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "DisconnectFromClient : " ;
   if(!mBrokerClient) {
      BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "BrokerClient is null !" ;
      return;
   }

   if(!mBrokerClient->disconnect(1000)) {
      BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "DisconnectFromClient : disconnect to broker failed" ;
   }

   mBrokerClient.reset();

   if (iSendDisconnectToClient) {
      Bt::Net::MqttSn::Disconnect disconnect;
      send(disconnect);
   }

   mRunning = false;
   mDispose(id());
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Subscribe& iMessage) {
   BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "SubscribeFromClient: " << iMessage.topicName ;
   if(!mBrokerClient) {
      BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "BrokerClient is null !" ;
      return;
   }

   uint16_t topicId = 0x0000;
   if(!containsWildcardCharacters(iMessage.topicName)) {
      topicId = mTopicStorage.getOrCreateTopicId(iMessage.topicName);
   }

   Bt::Net::MqttSn::ReturnCode returnCode = Bt::Net::MqttSn::ReturnCode::ACCEPTED;
   if (!mBrokerClient->subscribe(iMessage.topicName,iMessage.flags.bits.qos)) {
      returnCode = Bt::Net::MqttSn::ReturnCode::REJECTED_NOT_SUPPORTED;
   }

   Bt::Net::MqttSn::Suback suback(iMessage.flags.bits.qos, topicId, iMessage.msgId, returnCode);
   send(suback);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Suback& iMessage) {
   BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "SubackFromClient: " << iMessage.topicId ;
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Pingreq& iMessage) {
   BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "PingreqFromClient: " ;
   Bt::Net::MqttSn::Pingresp response;
   send(response);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Pingresp& iMessage) {
   BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "PingrespFromClient: " ;
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::send(Bt::Net::MqttSn::I_Message& iMessage) {
   std::vector<uint8_t> buffer;
   Bt::Net::MqttSn::PacketWriter<std::vector> writer(buffer);
   iMessage.write(writer);
   mSocket->send(buffer.data(),buffer.size(), mRfNodeId);
}

//-------------------------------------------------------------------------------------------------

bool GatewayConnection::containsWildcardCharacters(const std::string& iTopicName) {
   return iTopicName.find_first_of("*+") != std::string::npos;
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::workcycle() {
   while (mRunning) {
      std::function<void()> action;
      bool validAction = true;
      if(mTimeTillNextKeepAlive == std::chrono::seconds(0)) {
         mQueue.pop(action);
      } else {
         validAction = mQueue.tryPop(action, mTimeTillNextKeepAlive);
      }

      if (validAction) {
         action();
      } else {
         BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "missing keep alive message => disconnect()" ;
         disconnect(false);
      }
   }
   BOOST_LOG_TRIVIAL(debug) << "GWC[" << static_cast<int>(mRfNodeId) << "]" << "end of workcycle" ;

}

//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Bt
