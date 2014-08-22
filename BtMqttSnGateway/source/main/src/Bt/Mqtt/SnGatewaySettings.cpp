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

   mLogging.level = getEnum("Settings.Logging.Level", Log::LoggingLevel::WARNING, std::map<std::string,Log::LoggingLevel>{
      {"Error", Log::LoggingLevel::ERROR},
      {"Warning", Log::LoggingLevel::WARNING},
      {"Info", Log::LoggingLevel::INFO},
      {"Debug", Log::LoggingLevel::DEBUG}
   });

   mBroker.url = get<std::string>("Settings.Broker.Url");
   mBroker.user = getOptional<std::string>("Settings.Broker.User");
   mBroker.password = getOptional<std::string>("Settings.Broker.Password");
   mRf24.chipSelect = getEnum("Settings.Broker.ChipSelect", Mcu::I_Spi::ChipSelect::CHIP_SELECT_0, std::map<int,Mcu::I_Spi::ChipSelect>{
      {0, Mcu::I_Spi::ChipSelect::CHIP_SELECT_0},
      {1, Mcu::I_Spi::ChipSelect::CHIP_SELECT_1}
   });

   mRf24.chipEnablePin = get("Settings.RfPacketSocket.ChipEnablePin",25);
   mRf24.irqPin = get("Settings.RfPacketSocket.IrqPin",24);

   mSocket.nodeId = get("Settings.RfPacketSocket.NodeId",0);

}

//-------------------------------------------------------------------------------------------------

SnGatewaySettings::~SnGatewaySettings() {

}

//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Bt
