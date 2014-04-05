//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::Mqtt::MqttClient
//  
//*************************************************************************************************

#include "Bt/Net/Mqtt/MqttClient.hpp"

#include <iostream>
#include <memory>
#include <vector>

namespace Bt {
namespace Net {
namespace Mqtt {


void MqttClient::connectionLostCallback(void *iContext, char *iCause) {
   if(iContext != nullptr) {
      MqttClient* client = static_cast<MqttClient*>(iContext);
      client->connectionLost(iCause);
   }
}

int MqttClient::messageArrivedCallback(void *iContext, char *iTopicName, int iTopicLength, MQTTClient_message *iMessage) {
   if(iContext != nullptr) {
      MqttClient* client = static_cast<MqttClient*>(iContext);
      return client->messageArrived(iTopicName, iTopicLength, iMessage);
   }
   return 0;
}

void MqttClient::deliveryCompleteCallback(void *iContext, MQTTClient_deliveryToken iDeliveryToken) {
   if(iContext != nullptr) {
      MqttClient* client = static_cast<MqttClient*>(iContext);
      client->deliveryComplete(iDeliveryToken);

   }
}



//-------------------------------------------------------------------------------------------------

MqttClient::MqttClient(I_Listener& iListener, std::string iAddress, std::string iClientId)
: mListener(iListener), mAddress(std::move(iAddress)), mClientId(std::move(iClientId)) {

   int result = MQTTClient_create(&mClient, const_cast<char*>(mAddress.c_str()), const_cast<char*>(mClientId.c_str()), MQTTCLIENT_PERSISTENCE_NONE, nullptr);
   if (result != MQTTCLIENT_SUCCESS)
   {
      std::cout << "MQTTClient_create: failed with " << result << std::endl;
   }

   result = MQTTClient_setCallbacks(mClient, this, &connectionLostCallback, &messageArrivedCallback, &deliveryCompleteCallback);
   if (result != MQTTCLIENT_SUCCESS)
   {
      std::cout << "MQTTClient_setCallbacks: failed with " << result << std::endl;
   }

}

//-------------------------------------------------------------------------------------------------

MqttClient::~MqttClient() {
   MQTTClient_destroy(&mClient);
}

//-------------------------------------------------------------------------------------------------

bool MqttClient::connect(const ConnectOptions& options) {
   MQTTClient_connectOptions connectOptions = MQTTClient_connectOptions_initializer;
   connectOptions.cleansession = 1;
   connectOptions.connectTimeout = 100;
   if(!options.username.empty()) {
      connectOptions.username = const_cast<char*>(options.username.c_str());
      connectOptions.password = const_cast<char*>(options.password.c_str());
   }
   int connectResult = MQTTClient_connect(mClient, &connectOptions);
   if (connectResult != MQTTCLIENT_SUCCESS)
   {
      std::cout << "MqttClient::connect: failed with " << connectResult << std::endl;
      return false;
   }
   return true;
}

//-------------------------------------------------------------------------------------------------

bool MqttClient::disconnect(int timeout) {
   return MQTTClient_disconnect(mClient, timeout) == MQTTCLIENT_SUCCESS;
}

//-------------------------------------------------------------------------------------------------

std::future<bool> MqttClient::publish(const std::string& iTopic, const std::string& iPayload, int iQos, bool iRetained) {
   MQTTClient_message message = MQTTClient_message_initializer;
   message.payload = const_cast<char *>(iPayload.data());
   message.payloadlen = iPayload.size();
   message.qos = iQos;
   message.retained = iRetained;

   MQTTClient_deliveryToken deliveryToken;

   std::promise<bool> promise;
   std::future<bool> future = promise.get_future();

   int result = MQTTClient_publishMessage(mClient, const_cast<char*>(iTopic.c_str()), &message, &deliveryToken);
   if (result != MQTTCLIENT_SUCCESS)
   {
      std::cout << "MqttClient::publish: failed with " << result << std::endl;
      promise.set_value(false);
      return future;
   }

   if (iQos > 0) {
      std::lock_guard<std::mutex> lock(mMutex);
      mTokens[deliveryToken] = std::move(promise);
   } else {
      promise.set_value(true);
   }

   return future;
}

//-------------------------------------------------------------------------------------------------

int MqttClient::subscribe(const std::string& iTopic, int iQos) {
   return MQTTClient_subscribe(mClient, const_cast<char*>(iTopic.c_str()), iQos);
}

//-------------------------------------------------------------------------------------------------

void MqttClient::connectionLost(char* iCause) {
   std::cout << "MqttClient::connectionLost: " << iCause << std::endl;
}

//-------------------------------------------------------------------------------------------------

int MqttClient::messageArrived(char *iTopicName, int iTopicLength, MQTTClient_message *iMessage) {
   std::string topicName(iTopicName);
   if (iTopicLength > 0) {
      topicName = std::string(iTopicName,iTopicLength);
   }

   auto message = std::shared_ptr<Message>( new Message {
      std::string(static_cast<char*>(iMessage->payload), iMessage->payloadlen),
      iMessage->retained > 0
   });

   mListener.messageArrived(topicName, message);

   MQTTClient_freeMessage(&iMessage);
   MQTTClient_free(iTopicName);
   return 1;
}

//-------------------------------------------------------------------------------------------------

void MqttClient::deliveryComplete(MQTTClient_deliveryToken iDeliveryToken) {
   std::cout << "MqttClient::deliveryComplete: token = " << iDeliveryToken << std::endl;
   std::lock_guard<std::mutex> lock(mMutex);
   auto iterator = mTokens.find(iDeliveryToken);
   if (iterator == mTokens.end()) {
      std::cout << "MqttClient::deliveryComplete: could not find token = " << iDeliveryToken << std::endl;
      return;
   }

   iterator->second.set_value(true);
   mTokens.erase(iterator);
}

//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Net
} // namespace Bt
