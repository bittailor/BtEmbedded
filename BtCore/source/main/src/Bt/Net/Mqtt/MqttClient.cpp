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
   return MQTTClient_disconnect(mClient, timeout) > 0;
}

//-------------------------------------------------------------------------------------------------

bool MqttClient::publish(const std::string& iTopicName, const Message& iMessage) {
   MQTTClient_message message = MQTTClient_message_initializer;
   message.payload = const_cast<char *>(iMessage.payload.data());
   message.payloadlen = iMessage.payload.size();
   message.qos = 0;
   message.retained = iMessage.retained;

   MQTTClient_deliveryToken deliveryToken;


   int result = MQTTClient_publishMessage(mClient, const_cast<char*>(iTopicName.c_str()), &message, &deliveryToken);
   if (result != MQTTCLIENT_SUCCESS)
   {
      std::cout << "MqttClient::publish: failed with " << result << std::endl;
      return false;
   }
   return true;
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
}

//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Net
} // namespace Bt
