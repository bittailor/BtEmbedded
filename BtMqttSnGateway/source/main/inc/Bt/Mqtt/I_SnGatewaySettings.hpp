//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::I_SnGatewaySettings
//  
//*************************************************************************************************

#ifndef INC__Bt_Mqtt_I_SnGatewaySettings__hpp
#define INC__Bt_Mqtt_I_SnGatewaySettings__hpp

#include <Bt/Mcu/I_Spi.hpp>
#include <Bt/Log/Logging.hpp>

namespace Bt {
namespace Mqtt {

class I_SnGatewaySettings {
   public:

      struct Logging {
         Log::LoggingLevel level;
      };

      struct Broker {
         std::string url;
         boost::optional<std::string> user;
         boost::optional<std::string> password;
      };

      struct Rf24 {
         Mcu::I_Spi::ChipSelect chipSelect;
         int chipEnablePin;
         int irqPin;
         int channel;
      };

      struct Socket {
            int nodeId;
      };


      virtual ~I_SnGatewaySettings() {}
      
      virtual const Logging& logging() const = 0;
      virtual const Broker& broker() const = 0;
      virtual const Rf24& rf24() const = 0;
      virtual const Socket& socket() const = 0;

};

} // namespace Mqtt
} // namespace Bt

#endif // INC__Bt_Mqtt_I_SnGatewaySettings__hpp
