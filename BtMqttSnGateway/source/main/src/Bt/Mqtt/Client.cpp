//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::Client
//  
//*************************************************************************************************

#include "Bt/Mqtt/Client.hpp"

#include <iostream>

namespace Bt {
namespace Mqtt {


//-------------------------------------------------------------------------------------------------

Client::Client(uint8_t iRfNodeId, std::shared_ptr<Rf24::I_RfPacketSocket> iSocket)
: mRfNodeId(iRfNodeId), mSocket(iSocket) {

}

//-------------------------------------------------------------------------------------------------

Client::~Client() {

}

//-------------------------------------------------------------------------------------------------

void Client::handle(std::shared_ptr<Bt::Net::Mqtt::MqttSn::I_Message> iMessage) {
   iMessage->accept(*this);
}

//-------------------------------------------------------------------------------------------------

void Client::visit(Bt::Net::Mqtt::MqttSn::Connect& iMessage) {
   std::cout << "Connect : " << iMessage.clientId << std::endl;
   Bt::Net::Mqtt::MqttSn::Connack connack(Bt::Net::Mqtt::MqttSn::ReturnCode::ACCEPTED);
   send(connack);
}

//-------------------------------------------------------------------------------------------------

void Client::visit(Bt::Net::Mqtt::MqttSn::Connack& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void Client::visit(Bt::Net::Mqtt::MqttSn::Register& iMessage) {
   std::cout << "Register : " << iMessage.topicName << std::endl;
   Bt::Net::Mqtt::MqttSn::Regack regack(77, iMessage.msgId , Bt::Net::Mqtt::MqttSn::ReturnCode::ACCEPTED);
   send(regack);
}

//-------------------------------------------------------------------------------------------------

void Client::visit(Bt::Net::Mqtt::MqttSn::Regack& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void Client::visit(Bt::Net::Mqtt::MqttSn::Publish& iMessage) {
   std::cout << "Publish : " << iMessage.data << std::endl;
}

//-------------------------------------------------------------------------------------------------

void Client::send(Bt::Net::Mqtt::MqttSn::I_Message& iMessage) {
   std::vector<uint8_t> buffer;
   Bt::Net::Mqtt::MqttSn::PacketWriter<std::vector> writer(buffer);
   iMessage.write(writer);
   mSocket->send(buffer.data(),buffer.size(), mRfNodeId);
}

//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Bt
