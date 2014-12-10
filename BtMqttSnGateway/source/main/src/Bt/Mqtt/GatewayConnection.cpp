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

#include <boost/lexical_cast.hpp>

#include <Bt/Log/Logging.hpp>
#include <Bt/Util/Demangling.hpp>

namespace Bt {
namespace Mqtt {

#define BT_LOG_GWC(LEVEL) BT_LOG(LEVEL) << "GWC[" << static_cast<int>(mRfNodeId) << "]"
#define BT_LOG_GWC_STATE(LEVEL) BT_LOG(LEVEL) << "GWC[" << static_cast<int>(mGatewayConnection.mRfNodeId) << "]"

//-------------------------------------------------------------------------------------------------

GatewayConnection::GatewayConnection(uint8_t iRfNodeId,
                                     std::shared_ptr<Rf24::I_RfPacketSocket> iSocket,
                                     std::shared_ptr<I_MqttFactory> iFactory,
                                     std::function<void(int)> iDispose)
: mRunning(true), mRfNodeId(iRfNodeId), mSocket(iSocket)
, mFactory(iFactory), mDispose(iDispose)
, mMsgIdCounter(1), mThread(&GatewayConnection::workcycle,this)
, mKeepAliveTimeout(0)
, mAsleepTimeout(0)
, mDisconnectedState(*this), mActiveState(*this), mAsleepState(*this), mCurrentState(&mDisconnectedState){
   BT_LOG_GWC(INFO) << "GatewayConnection()";
}

//-------------------------------------------------------------------------------------------------

GatewayConnection::~GatewayConnection() {
   BT_LOG_GWC(DEBUG) << " send stop workcycle.";
   mQueue.push([=]() {
      this->mRunning = false;
      BT_LOG_GWC(DEBUG) << " workcycle stopped.";
   });
   BT_LOG_GWC(DEBUG) << " join workcycle thread.";
   mThread.join();
   BT_LOG_GWC(INFO) << "~GatewayConnection()" ;
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

void GatewayConnection::connectionLost(const std::string& iCause) {
   std::string cause = iCause;
   mQueue.push([=](){this->connectionLostInternal(cause);});
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::connectionLostInternal(const std::string& iCause) {
   BT_LOG_GWC(WARNING) << "ConnectionLostFromBroker: " << iCause ;
   dispose(true);
}

//-------------------------------------------------------------------------------------------------

bool GatewayConnection::messageArrived(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage) {
   std::string topicName = iTopicName;
   mQueue.push([=](){this->messageArrivedInternal(topicName,iMessage);});
   return true;
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::messageArrivedInternal(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage) {
   BT_LOG_GWC(DEBUG) << "PublishFromBroker: " << iTopicName << " " << iMessage->data ;
   mCurrentState->handleMessageArrived(iTopicName, iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Connect& iMessage) {
   BT_LOG_GWC(DEBUG) << "CONNECT from client - " << iMessage.clientId;
   mCurrentState->handleConnect(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Connack& iMessage) {
   BT_LOG_GWC(WARNING) << "drop CONNACK message since since this should not be sent by a client";
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Register& iMessage) {
   BT_LOG_GWC(DEBUG) << "REGISTER from client: -" << iMessage.topicName ;
   mCurrentState->handleRegister(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Regack& iMessage) {
   BT_LOG_GWC(DEBUG) << "REGACK from client - " << iMessage.topicId;
   mCurrentState->handleRegack(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Publish& iMessage) {
   BT_LOG_GWC(DEBUG) << "PUBLISH from client - " << iMessage.topicId << " "  << iMessage.data ;
   mCurrentState->handlePublish(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Disconnect& iMessage) {
   BT_LOG_GWC(DEBUG) << "DISCONNECT from client - " << (iMessage.withDuration ? boost::lexical_cast<std::string>(iMessage.duration) : "");
   mCurrentState->handleDisconnect(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Subscribe& iMessage) {
   BT_LOG_GWC(DEBUG) << "SUBSCRIBE from client - " << iMessage.topicName;
   mCurrentState->handleSubscribe(iMessage);
}


//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Suback& iMessage) {
   BT_LOG_GWC(WARNING) << "drop SUBACK message since since this should not be sent by a client";
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Pingreq& iMessage) {
   BT_LOG_GWC(DEBUG) << "PINGREQ from client -" << (iMessage.withClientId ? iMessage.clientId : "") ;
   mCurrentState->handlePingreq(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Pingresp& iMessage) {
   BT_LOG_GWC(DEBUG) << "PINGRESP from client -" ;
   mCurrentState->handlePingresp(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::connectToBroker(Bt::Net::MqttSn::Connect& iMessage) {

   if(!iMessage.flags.bits.cleanSession) {
      BT_LOG_GWC(WARNING) << "CONNECT with CleanSession=false not supported!";
      changeState(mDisconnectedState);
      send(Bt::Net::MqttSn::Connack(Bt::Net::MqttSn::ReturnCode::REJECTED_NOT_SUPPORTED));
      return;
   }

   mKeepAliveTimeout = std::chrono::seconds(iMessage.duration + (iMessage.duration / 2));

   try {
      mBrokerClient = mFactory->createClient(*this, iMessage.clientId);
      bool result = mBrokerClient->connect(mFactory->createDefaultOptions());
      if(!result){
         changeState(mDisconnectedState);
         Bt::Net::MqttSn::Connack connack(Bt::Net::MqttSn::ReturnCode::REJECTED_NOT_SUPPORTED);
         send(connack);
         return;
      }
   } catch (std::exception& exception) {
      BT_LOG_GWC(WARNING) << "create MqttClient failed : " << exception.what() ;
      changeState(mDisconnectedState);
      Bt::Net::MqttSn::Connack connack(Bt::Net::MqttSn::ReturnCode::REJECTED_NOT_SUPPORTED);
      send(connack);
      return;
   }

   changeState(mActiveState);

   Bt::Net::MqttSn::Connack connack(Bt::Net::MqttSn::ReturnCode::ACCEPTED);
   send(connack);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::reconnectToBroker(Bt::Net::MqttSn::Connect& iMessage) {
   BT_LOG_GWC(DEBUG) << "Reconnect without disconnect => flush topic storage" ;
   mBrokerClient->disconnect(1000);
   mTopicStorage.clear();
   connectToBroker(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::connectFromSleep(Bt::Net::MqttSn::Connect& iMessage) {
   changeState(mActiveState);
   sendBufferedMessages();
   send(Bt::Net::MqttSn::Connack(Bt::Net::MqttSn::ReturnCode::ACCEPTED));
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::awake() {
   sendBufferedMessages();
   send(Bt::Net::MqttSn::Pingresp());
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::forwardMessageToClient(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage) {
   uint16_t topicId = mTopicStorage.getTopicId(iTopicName);

   if(topicId == TopicStorage::NO_TOPIC_ID) {
      topicId = mTopicStorage.getOrCreateTopicId(iTopicName);
      uint16_t msgId = mMsgIdCounter++;
      send(Bt::Net::MqttSn::Register(topicId, msgId, iTopicName));
   }

   uint16_t msgId = mMsgIdCounter++;

   Bt::Net::MqttSn::Publish publish(iMessage->qos, iMessage->retained, topicId, msgId, iMessage->data);
   send(publish);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::storeMessage(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage) {
   mBufferedMessages.push_back(BufferedMessage{iTopicName,iMessage});
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::sendPingresp() {
   send(Bt::Net::MqttSn::Pingresp());
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::sendBufferedMessages() {
   std::vector<BufferedMessage> messages = mBufferedMessages;
   mBufferedMessages.clear();
   for(BufferedMessage message : messages) {
      forwardMessageToClient(message.first, message.second);
   }
}


//-------------------------------------------------------------------------------------------------

void GatewayConnection::send(const Bt::Net::MqttSn::I_Message& iMessage) {
   std::vector<uint8_t> buffer;
   Bt::Net::MqttSn::PacketWriter<std::vector> writer(buffer);
   iMessage.write(writer);
   if(mSocket->send(buffer.data(),buffer.size(), mRfNodeId) == -1) {
      BT_LOG_GWC(ERROR) << "send of " << Util::demangle(typeid(iMessage)) << " failed";
   }
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::disconnect(bool iSendDisconnectToClient) {
   if(mBrokerClient) {
      BT_LOG_GWC(WARNING) << "send disconnect to broker" ;
      if(!mBrokerClient->disconnect(1000)) {
         BT_LOG_GWC(WARNING) << "disconnect from broker failed" ;
      }
   }
   dispose(iSendDisconnectToClient);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::dispose(bool iSendDisconnectToClient) {
   BT_LOG_GWC(WARNING) << "dispose " ;
   changeState(mDisconnectedState);
   mBrokerClient.reset();

   if (iSendDisconnectToClient) {
      send(Bt::Net::MqttSn::Disconnect());
   }

   mDispose(id());
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
      std::chrono::seconds timeout = mCurrentState->timeout();
      BT_LOG_GWC(DEBUG) << "wait for a message with a timeout of " << timeout.count() << "s" ;
      if(timeout == std::chrono::seconds(0)) {
         mQueue.pop(action);
      } else {
         validAction = mQueue.tryPop(action, mKeepAliveTimeout);
      }

      if (validAction) {
         action();
      } else {
         BT_LOG_GWC(WARNING) << "missing keep alive message => disconnect()" ;
         disconnect(false);
      }
   }
   BT_LOG_GWC(DEBUG) << "end of workcycle" ;

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::changeState(State& iState) {
   BT_LOG_GWC(DEBUG) << "state change " << Util::demangle(typeid(*mCurrentState)) << " => " << Util::demangle(typeid(iState));
   mCurrentState = &iState;
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::registerTopic(Bt::Net::MqttSn::Register& iMessage) {
   uint16_t topicId = mTopicStorage.getOrCreateTopicId(iMessage.topicName);
   send(Bt::Net::MqttSn::Regack(topicId, iMessage.msgId , Bt::Net::MqttSn::ReturnCode::ACCEPTED));
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::publishToBroker(Bt::Net::MqttSn::Publish& iMessage) {
   std::string topicName = mTopicStorage.getTopicName(iMessage.topicId);
   if (topicName.empty()) {
      BT_LOG_GWC(WARNING) << "could not find topic name for id " << iMessage.topicId ;
      return;
   }

   if(!mBrokerClient->publish(topicName, iMessage.data, iMessage.flags.bits.qos, iMessage.flags.bits.retain).get()) {
      BT_LOG_GWC(WARNING) << "publish to broker failed";
   }
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::handleSleep(uint16_t iDuration) {
   mAsleepTimeout = std::chrono::seconds(iDuration);
   changeState(mAsleepState);
   send(Bt::Net::MqttSn::Disconnect());
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::subscribe(Bt::Net::MqttSn::Subscribe& iMessage) {

   uint16_t topicId = 0x0000;
   if(!containsWildcardCharacters(iMessage.topicName)) {
      topicId = mTopicStorage.getOrCreateTopicId(iMessage.topicName);
   }

   Bt::Net::MqttSn::ReturnCode returnCode = Bt::Net::MqttSn::ReturnCode::ACCEPTED;
   if (!mBrokerClient->subscribe(iMessage.topicName,iMessage.flags.bits.qos)) {
      returnCode = Bt::Net::MqttSn::ReturnCode::REJECTED_NOT_SUPPORTED;
   }

   send(Bt::Net::MqttSn::Suback(iMessage.flags.bits.qos, topicId, iMessage.msgId, returnCode));
}

//-------------------------------------------------------------------------------------------------
// GatewayConnection::Disconnected
//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handleConnect(Bt::Net::MqttSn::Connect& iMessage) {
   mGatewayConnection.connectToBroker(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handleRegister(Bt::Net::MqttSn::Register& iMessage) {
   BT_LOG_GWC_STATE(WARNING) << "drop REGISTER message from client since in Disconnected state";
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handleRegack(Bt::Net::MqttSn::Regack& iMessage) {
   BT_LOG_GWC_STATE(WARNING) << "drop REGACK message from client since in Disconnected state";
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handlePublish(Bt::Net::MqttSn::Publish& iMessage) {
   if (iMessage.flags.bits.qos == 4 ) {
      BT_LOG_GWC_STATE(WARNING) << "PUBLISH with QoS -1 not supported yet => drop PUBLISH message";
      return;
   }
   BT_LOG_GWC_STATE(WARNING) << "drop PUBLISH message from client since in Disconnected state";
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handleDisconnect(Bt::Net::MqttSn::Disconnect& iMessage) {
   mGatewayConnection.dispose(true);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handleSubscribe(Bt::Net::MqttSn::Subscribe& iMessage) {
   BT_LOG_GWC_STATE(WARNING) << "drop SUBSCRIBE message from client since in Disconnected state";
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handlePingreq(Bt::Net::MqttSn::Pingreq& iMessage) {
   // do not send a PINGRESP since we are Disconnected
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handlePingresp(Bt::Net::MqttSn::Pingresp& iMessage) {
   // currently nothing to do
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handleMessageArrived(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage) {
   BT_LOG_GWC_STATE(WARNING) << "drop publish message from broker since in Disconnected state";
}

//-------------------------------------------------------------------------------------------------

std::chrono::seconds GatewayConnection::Disconnected::timeout() {
   return std::chrono::seconds(0);
}


//-------------------------------------------------------------------------------------------------
// GatewayConnection::Active
//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handleConnect(Bt::Net::MqttSn::Connect& iMessage) {
   mGatewayConnection.reconnectToBroker(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handleRegister(Bt::Net::MqttSn::Register& iMessage) {
   mGatewayConnection.registerTopic(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handleRegack(Bt::Net::MqttSn::Regack& iMessage) {
   // currently nothing to do
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handlePublish(Bt::Net::MqttSn::Publish& iMessage) {
   mGatewayConnection.publishToBroker(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handleDisconnect(Bt::Net::MqttSn::Disconnect& iMessage) {
   if (iMessage.withDuration) {
      mGatewayConnection.handleSleep(iMessage.duration);
   } else {
      mGatewayConnection.disconnect(true);
   }
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handleSubscribe(Bt::Net::MqttSn::Subscribe& iMessage) {
   mGatewayConnection.subscribe(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handlePingreq(Bt::Net::MqttSn::Pingreq& iMessage) {
   mGatewayConnection.sendPingresp();
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handlePingresp(Bt::Net::MqttSn::Pingresp& iMessage) {
   // currently nothing to do
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handleMessageArrived(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage) {
   mGatewayConnection.forwardMessageToClient(iTopicName, iMessage);
}

//-------------------------------------------------------------------------------------------------

std::chrono::seconds GatewayConnection::Active::timeout() {
   return mGatewayConnection.mKeepAliveTimeout;
}

//-------------------------------------------------------------------------------------------------
// Asleep
//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handleConnect(Bt::Net::MqttSn::Connect& iMessage) {
   if(iMessage.flags.bits.cleanSession) {
      BT_LOG_GWC_STATE(WARNING) << "connect from Asleep with clean session set => reconnect";
      mGatewayConnection.reconnectToBroker(iMessage);
   } else {
      mGatewayConnection.connectFromSleep(iMessage);
   }

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handleRegister(Bt::Net::MqttSn::Register& iMessage) {
   mGatewayConnection.registerTopic(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handleRegack(Bt::Net::MqttSn::Regack& iMessage) {
   // currently nothing to do
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handlePublish(Bt::Net::MqttSn::Publish& iMessage) {
   // we are in asleep, but lets do it anyway
   mGatewayConnection.publishToBroker(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handleDisconnect(Bt::Net::MqttSn::Disconnect& iMessage) {
   // we are in asleep, but lets do it anyway
   mGatewayConnection.disconnect(true);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handleSubscribe(Bt::Net::MqttSn::Subscribe& iMessage) {
   // we are in asleep, but lets do it anyway
   mGatewayConnection.subscribe(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handlePingreq(Bt::Net::MqttSn::Pingreq& iMessage) {
   if(iMessage.withClientId) {
      mGatewayConnection.awake();
   } else {
      mGatewayConnection.sendPingresp();
   }
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handlePingresp(Bt::Net::MqttSn::Pingresp& iMessage) {
   // currently nothing to do
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handleMessageArrived(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage) {
   mGatewayConnection.storeMessage(iTopicName, iMessage);
}

//-------------------------------------------------------------------------------------------------

std::chrono::seconds GatewayConnection::Asleep::timeout() {
   return mGatewayConnection.mAsleepTimeout;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Bt
