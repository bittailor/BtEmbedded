//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::I_MqttFactory
//  
//*************************************************************************************************

#ifndef INC__Bt_Mqtt_I_MqttFactory__hpp
#define INC__Bt_Mqtt_I_MqttFactory__hpp

#include <Bt/Net/Mqtt/I_MqttClient.hpp>

namespace Bt {
namespace Mqtt {

class I_MqttFactory {
   public:
      virtual ~I_MqttFactory() {}
      
      virtual std::shared_ptr<Bt::Net::Mqtt::I_MqttClient> createClient(Bt::Net::Mqtt::I_MqttClient::I_Listener& iListener, std::string iClientId) = 0;
      virtual Bt::Net::Mqtt::I_MqttClient::ConnectOptions createDefaultOptions() = 0;

};

} // namespace Mqtt
} // namespace Bt

#endif // INC__Bt_Mqtt_I_MqttFactory__hpp
