//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::Mqtt::I_MqttClient
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_Mqtt_I_MqttClient__hpp
#define INC__Bt_Net_Mqtt_I_MqttClient__hpp

#include <future>
#include <memory>
#include <string>

namespace Bt {
namespace Net {
namespace Mqtt {

class I_MqttClient {
   public:
      struct ConnectOptions {
         std::string username;
         std::string password;
      };

      struct Message {
            int qos;
            bool retained;
            std::string data;
      };

      class I_Listener {
         public:
            virtual ~I_Listener() {}
            virtual void connectionLost(const std::string& iCause) = 0;
            virtual bool messageArrived(const std::string& iTopicName, std::shared_ptr<Message> iMessage) = 0;

      };

      virtual ~I_MqttClient() {}

      virtual bool connect(const ConnectOptions& options) = 0;
      virtual bool disconnect(int timeout) = 0;
      virtual std::future<bool> publish(const std::string& iTopic, const std::string& iPayload, int iQos, bool iRetained) = 0;
      virtual bool subscribe(const std::string& iTopic, int iQos) = 0;

};

} // namespace Mqtt
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Mqtt_I_MqttClient__hpp
