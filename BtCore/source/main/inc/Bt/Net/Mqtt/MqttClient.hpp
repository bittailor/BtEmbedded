//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::Mqtt::MqttClient
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_Mqtt_MqttClient__hpp
#define INC__Bt_Net_Mqtt_MqttClient__hpp

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <future>

extern "C" {
#include "MQTTClient.h"
}

namespace Bt {
namespace Net {
namespace Mqtt {

class MqttClient 
{
   public:

      struct ConnectOptions {
         std::string username;
         std::string password;
      };

      struct Message {
         std::string payload;
         bool retained;
      };

      class I_Listener {
         public:
            virtual ~I_Listener() {}
            virtual bool messageArrived(const std::string& iTopicName, std::shared_ptr<Message> iMessage) = 0;
      };

      MqttClient(I_Listener& iListener, std::string iAddress, std::string iClientId);
      ~MqttClient();

      bool connect(const ConnectOptions& options);
      bool disconnect(int timeout);
      std::future<bool> publish(const std::string& iTopicName, const std::string& iPayload, int iQos, bool iRetained);
   
   private:
      static void connectionLostCallback(void *iContext, char *iCause);
      static int messageArrivedCallback(void *iContext, char *iTopicName, int iTopicLength, MQTTClient_message *iMessage);
      static void deliveryCompleteCallback(void *iContext, MQTTClient_deliveryToken iDeliveryToken);

   	  // Constructor to prohibit copy construction.
      MqttClient(const MqttClient&);

      // Operator= to prohibit copy assignment
      MqttClient& operator=(const MqttClient&);

      void connectionLost(char* iCause);
      int messageArrived(char* iTopicName, int iTopicLength, MQTTClient_message *iMessage);
      void deliveryComplete(MQTTClient_deliveryToken iDeliveryToken);

      MQTTClient mClient;
      I_Listener& mListener;
      std::string mAddress;
      std::string mClientId;
      std::map<MQTTClient_deliveryToken,std::promise<bool>> mTokens;


};

} // namespace Mqtt
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Mqtt_MqttClient__hpp
