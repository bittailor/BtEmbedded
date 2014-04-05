//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::MqttFactory
//  
//*************************************************************************************************

#ifndef INC__Bt_Mqtt_MqttFactory__hpp
#define INC__Bt_Mqtt_MqttFactory__hpp

#include "Bt/Mqtt/I_MqttFactory.hpp"

namespace Bt {
namespace Mqtt {

class MqttFactory : public I_MqttFactory
{
   public:
      MqttFactory(const std::string& iAddress, const std::string& iUser, const std::string& iPassword);
      ~MqttFactory();
   
      virtual std::shared_ptr<Bt::Net::Mqtt::I_MqttClient> createClient(Bt::Net::Mqtt::I_MqttClient::I_Listener& iListener, std::string iClientId);
      virtual Bt::Net::Mqtt::I_MqttClient::ConnectOptions createDefaultOptions();

   private:
   	  // Constructor to prohibit copy construction.
      MqttFactory(const MqttFactory&);

      // Operator= to prohibit copy assignment
      MqttFactory& operator=(const MqttFactory&);

      std::string mAddress;
      std::string mUser;
      std::string mPassword;
};

} // namespace Mqtt
} // namespace Bt

#endif // INC__Bt_Mqtt_MqttFactory__hpp
