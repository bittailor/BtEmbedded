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
   BT_LOG_GWC(DEBUG) << "GatewayConnection() ";
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
   BT_LOG_GWC(DEBUG) << "~GatewayConnection() " ;
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

void GatewayConnection::forwardMessageToClient(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage) {
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

void GatewayConnection::storeMessage(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage) {
   mBufferedMessages.push_back(BufferedMessage{iTopicName,iMessage});
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::sendPingresp() {
   Bt::Net::MqttSn::Pingresp response;
   send(response);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Connect& iMessage) {
   BT_LOG_GWC(DEBUG) << "ConnectFromClient: " << iMessage.clientId;
   mCurrentState->handleConnect(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Connack& iMessage) {
   BT_LOG_GWC(WARNING) << "drop CONNACK message since since this should not be sent by a client";
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Register& iMessage) {
   BT_LOG_GWC(DEBUG) << "RegisterFromClient: " << iMessage.topicName ;
   mCurrentState->handleRegister(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Regack& iMessage) {
   BT_LOG_GWC(DEBUG) << "RegackFromClient: " << iMessage.topicId;
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Publish& iMessage) {
   BT_LOG_GWC(DEBUG) << "PublishFromClient: " << iMessage.topicId << " "  << iMessage.data ;
   if(!mBrokerClient) {
      BT_LOG_GWC(DEBUG) << "BrokerClient is null !" ;
      return;
   }


   std::string topicName = mTopicStorage.getTopicName(iMessage.topicId);
   if (topicName.empty()) {
      BT_LOG_GWC(DEBUG) << "PublishFromClient: could not find topic name for id " << iMessage.topicId ;
   }

   if(!mBrokerClient->publish(topicName, iMessage.data, iMessage.flags.bits.qos, iMessage.flags.bits.retain).get()) {
      BT_LOG_GWC(DEBUG) << "PublishFromClient : publish to broker failed ";
   }
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Disconnect& iMessage) {
   if (iMessage.withDuration) {
      handleSleep(iMessage.duration);
      return;
   }

   disconnect(true);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::handleSleep(uint16_t iDuration) {
   mAsleepTimeout = std::chrono::seconds(iDuration);
   changeState(mAsleepState);
   Bt::Net::MqttSn::Disconnect disconnect;
   send(disconnect);
}


//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Subscribe& iMessage) {
   BT_LOG_GWC(DEBUG) << "SubscribeFromClient: " << iMessage.topicName ;
   if(!mBrokerClient) {
      BT_LOG_GWC(DEBUG) << "BrokerClient is null !" ;
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
   BT_LOG_GWC(DEBUG) << "SubackFromClient: " << iMessage.topicId ;
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Pingreq& iMessage) {
   BT_LOG_GWC(DEBUG) << "PingreqFromClient: " ;
   mCurrentState->handlePingreq(iMessage);



   if(!mBrokerClient) {
      BT_LOG_GWC(DEBUG) << "BrokerClient is null !" ;
      return;
   }

   Bt::Net::MqttSn::Pingresp response;
   send(response);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::visit(Bt::Net::MqttSn::Pingresp& iMessage) {
   BT_LOG_GWC(DEBUG) << "PingrespFromClient: " ;
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::connectToBroker(Bt::Net::MqttSn::Connect& iMessage) {

   mKeepAliveTimeout = std::chrono::seconds(iMessage.duration + (iMessage.duration / 2));

   Bt::Net::MqttSn::ReturnCode returnCode = Bt::Net::MqttSn::ReturnCode::ACCEPTED;

   try {
      mBrokerClient = mFactory->createClient(*this, iMessage.clientId);
      bool result = mBrokerClient->connect(mFactory->createDefaultOptions());
      if(!result){
         returnCode = Bt::Net::MqttSn::ReturnCode::REJECTED_NOT_SUPPORTED;
      }
   } catch (std::exception& exception) {
      BT_LOG_GWC(WARNING) << "create MqttClient failed : " << exception.what() ;
      returnCode = Bt::Net::MqttSn::ReturnCode::REJECTED_NOT_SUPPORTED;
   }

   Bt::Net::MqttSn::Connack connack(returnCode);
   if (returnCode == Bt::Net::MqttSn::ReturnCode::ACCEPTED) {
      changeState(mActiveState);
   } else {
      changeState(mDisconnectedState);
   }

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
   Bt::Net::MqttSn::Connack connack(Bt::Net::MqttSn::ReturnCode::ACCEPTED);
   send(connack);
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

void GatewayConnection::send(Bt::Net::MqttSn::I_Message& iMessage) {
   std::vector<uint8_t> buffer;
   Bt::Net::MqttSn::PacketWriter<std::vector> writer(buffer);
   iMessage.write(writer);
   mSocket->send(buffer.data(),buffer.size(), mRfNodeId);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::disconnect(bool iSendDisconnectToClient) {
   BT_LOG_GWC(DEBUG) << "DisconnectFromClient : " ;
   if(!mBrokerClient) {
      BT_LOG_GWC(DEBUG) << "BrokerClient is null !" ;
      return;
   }

   if(!mBrokerClient->disconnect(1000)) {
      BT_LOG_GWC(DEBUG) << "DisconnectFromClient : disconnect to broker failed" ;
   }

   dispose(iSendDisconnectToClient);

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::dispose(bool iSendDisconnectToClient) {
   mBrokerClient.reset();

   if (iSendDisconnectToClient) {
      Bt::Net::MqttSn::Disconnect disconnect;
      send(disconnect);
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
      if(timeout == std::chrono::seconds(0)) {
         mQueue.pop(action);
      } else {
         validAction = mQueue.tryPop(action, mKeepAliveTimeout);
      }

      if (validAction) {
         action();
      } else {
         BT_LOG_GWC(DEBUG) << "missing keep alive message => disconnect()" ;
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
// GatewayConnection::Disconnected
//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handleConnect(Bt::Net::MqttSn::Connect& iMessage) {
   mGatewayConnection.connectToBroker(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handleRegister(Bt::Net::MqttSn::Register& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handleRegack(Bt::Net::MqttSn::Regack& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handlePublish(Bt::Net::MqttSn::Publish& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handleDisconnect(Bt::Net::MqttSn::Disconnect& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handleSubscribe(Bt::Net::MqttSn::Subscribe& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handleSuback(Bt::Net::MqttSn::Suback& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handlePingreq(Bt::Net::MqttSn::Pingreq& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handlePingresp(Bt::Net::MqttSn::Pingresp& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Disconnected::handleMessageArrived(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::I_MqttClient::Message> iMessage) {
   BT_LOG_GWC_STATE(WARNING) << "drop message from broker since in Disconnected state";
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
   uint16_t topicId = mGatewayConnection.mTopicStorage.getOrCreateTopicId(iMessage.topicName);
   Bt::Net::MqttSn::Regack regack(topicId, iMessage.msgId , Bt::Net::MqttSn::ReturnCode::ACCEPTED);
   mGatewayConnection.send(regack);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handleRegack(Bt::Net::MqttSn::Regack& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handlePublish(Bt::Net::MqttSn::Publish& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handleDisconnect(Bt::Net::MqttSn::Disconnect& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handleSubscribe(Bt::Net::MqttSn::Subscribe& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handleSuback(Bt::Net::MqttSn::Suback& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handlePingreq(Bt::Net::MqttSn::Pingreq& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Active::handlePingresp(Bt::Net::MqttSn::Pingresp& iMessage) {

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
   mGatewayConnection.connectFromSleep(iMessage);
}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handleRegister(Bt::Net::MqttSn::Register& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handleRegack(Bt::Net::MqttSn::Regack& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handlePublish(Bt::Net::MqttSn::Publish& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handleDisconnect(Bt::Net::MqttSn::Disconnect& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handleSubscribe(Bt::Net::MqttSn::Subscribe& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handleSuback(Bt::Net::MqttSn::Suback& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handlePingreq(Bt::Net::MqttSn::Pingreq& iMessage) {

}

//-------------------------------------------------------------------------------------------------

void GatewayConnection::Asleep::handlePingresp(Bt::Net::MqttSn::Pingresp& iMessage) {

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
