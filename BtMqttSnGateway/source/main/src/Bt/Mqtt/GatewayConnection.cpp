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

namespace Bt {
namespace Mqtt {


//-------------------------------------------------------------------------------------------------

GatewayConnection::GatewayConnection(uint8_t iRfNodeId,
                                     std::shared_ptr<Rf24::I_RfPacketSocket> iSocket,
                                     std::shared_ptr<I_MqttFactory> iFactory,
                                     std::function<bool(int)> iDispose)
: mRfNodeId(iRfNodeId), mSocket(iSocket), mFactory(iFactory), mDispose(iDispose), mMsgIdCounter(1) {
   std::cout << "GatewayConnection() "<< static_cast<int>(mRfNodeId) << std::endl;
}

//-------------------------------------------------------------------------------------------------

GatewayConnection::~GatewayConnection() {
   std::cout << "~GatewayConnection() "<< static_cast<int>(mRfNodeId) << std::endl;
}

//-------------------------------------------------------------------------------------------------

int GatewayConnection::id() {
   return mRfNodeId;
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::handle(std::shared_ptr<Bt::Net::MqttSn::I_Message> iMessage) {
   iMessage->accept(*this);
}

//-------------------------------------------------------------------------------------------------

bool GatewayConnection::messageArrived(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage) {
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

   return true;
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Connect& iMessage) {
   std::cout << "Connect : " << iMessage.clientId << std::endl;

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
   std::cout << "Register : " << iMessage.topicName << std::endl;

   uint16_t topicId = mTopicStorage.getOrCreateTopicId(iMessage.topicName);
   Bt::Net::MqttSn::Regack regack(topicId, iMessage.msgId , Bt::Net::MqttSn::ReturnCode::ACCEPTED);
   send(regack);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Regack& iMessage) {
   std::cout << "Regack : " << iMessage.topicId << std::endl;
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Publish& iMessage) {
   std::cout << "Publish : " << iMessage.data << std::endl;

   std::string topicName = mTopicStorage.getTopicName(iMessage.topicId);
   if (topicName.empty()) {
      std::cout << "Publish : could not find topic name for id " << iMessage.topicId << std::endl;
   }

   if(!mBrokerClient->publish(topicName, iMessage.data, iMessage.flags.bits.qos, iMessage.flags.bits.retain).get()) {
      std::cout << "Publish : failed "<< std::endl;
   }
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Disconnect& iMessage) {
   std::cout << "Disconnect : " << std::endl;
   if(!mBrokerClient->disconnect(1000)) {
      std::cout << "Disconnect : disconnect failed" << std::endl;
   }

   mBrokerClient.reset();

   Bt::Net::MqttSn::Disconnect disconnect;
   send(disconnect);
   mDispose(id());
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Subscribe& iMessage) {
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
   std::cout << "Suback : " << iMessage.topicId << std::endl;
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

} // namespace Mqtt
} // namespace Bt
