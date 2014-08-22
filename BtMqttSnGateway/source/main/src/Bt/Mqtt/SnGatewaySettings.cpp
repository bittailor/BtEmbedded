//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::SnGatewaySettings
//  
//*************************************************************************************************

#include "Bt/Mqtt/SnGatewaySettings.hpp"

#include <iostream>

#include <Bt/Mcu/I_Spi.hpp>

namespace Bt {
namespace Mqtt {


//-------------------------------------------------------------------------------------------------

SnGatewaySettings::SnGatewaySettings(const std::string& iFilename) {
   boost::property_tree::read_xml(iFilename, mProperties);

   mBroker.url = get<std::string>("Settings.Broker.Url");
   mBroker.user = getOptional<std::string>("Settings.Broker.User");
   mBroker.password = getOptional<std::string>("Settings.Broker.Password");

   int chipSelect = get<int>("Settings.RfPacketSocket.ChipSelect",0);
   switch(chipSelect) {
      case 0 : mRf24.chipSelect = Mcu::I_Spi::ChipSelect::CHIP_SELECT_0; break;
      case 1 : mRf24.chipSelect = Mcu::I_Spi::ChipSelect::CHIP_SELECT_1; break;
      default : throw boost::property_tree::ptree_error("The Settings.RfPacketSocket.ChipSelect can only be 0 or 1");
   }
   mRf24.chipEnablePin = get<int>("Settings.RfPacketSocket.ChipEnablePin",24);
   mRf24.irqPin = get<int>("Settings.RfPacketSocket.IrqPin",25);

   mSocket.nodeId = get<int>("Settings.RfPacketSocket.NodeId",0);

}
//Mcu::I_Spi::ChipSelect
//-------------------------------------------------------------------------------------------------

SnGatewaySettings::~SnGatewaySettings() {

}

//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Bt
