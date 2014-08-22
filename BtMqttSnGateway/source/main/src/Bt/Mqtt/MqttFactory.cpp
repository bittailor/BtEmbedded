//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::MqttFactory
//  
//*************************************************************************************************

#include "Bt/Mqtt/MqttFactory.hpp"

#include <Bt/Net/Mqtt/MqttClient.hpp>

namespace Bt {
namespace Mqtt {


//-------------------------------------------------------------------------------------------------

MqttFactory::MqttFactory(const std::string& iAddress,
                         const boost::optional<std::string>& iUser,
                         const boost::optional<std::string>& iPassword)
: mAddress(iAddress), mUser(iUser), mPassword(iPassword)  {

}

//-------------------------------------------------------------------------------------------------

MqttFactory::~MqttFactory() {

}

//-------------------------------------------------------------------------------------------------

std::shared_ptr<Bt::Net::Mqtt::I_MqttClient> MqttFactory::createClient(Bt::Net::Mqtt::I_MqttClient::I_Listener& iListener, std::string iClientId) {
   return std::make_shared<Bt::Net::Mqtt::MqttClient>(iListener, mAddress, iClientId);
}

//-------------------------------------------------------------------------------------------------

Bt::Net::Mqtt::I_MqttClient::ConnectOptions MqttFactory::createDefaultOptions() {
   if (!mUser) {
      return Bt::Net::Mqtt::I_MqttClient::ConnectOptions();
   }
   return Bt::Net::Mqtt::I_MqttClient::ConnectOptions{*mUser, *mPassword};
}

//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Bt
