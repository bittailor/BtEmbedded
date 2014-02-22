//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::Mqtt::I_MqttSnClient
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_Mqtt_I_MqttSnClient__hpp
#define INC__Bt_Net_Mqtt_I_MqttSnClient__hpp

namespace Bt {
namespace Net {
namespace Mqtt {

class I_MqttSnClient {
   public:
      enum { MAX_LENGTH_CLIENT_ID = 23 };

      virtual ~I_MqttSnClient() {}
      
      virtual void function() = 0;
};

} // namespace Mqtt
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Mqtt_I_MqttSnClient__hpp
