//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::Mqtt::MqttSnClient
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_Mqtt_MqttSnClient__hpp
#define INC__Bt_Net_Mqtt_MqttSnClient__hpp

#include <stdint.h>

#include <Bt/Net/Mqtt/I_MqttSnClient.hpp>
#include <Bt/Rf24/I_RfPacketSocket.hpp>

namespace Bt {
namespace Net {
namespace Mqtt {

class MqttSnClient : public I_MqttSnClient
{
   public:
      MqttSnClient(Rf24::I_RfPacketSocket& pSocket, uint8_t pGatewayNodeId, const char* pClientId);
      ~MqttSnClient();

      bool connect();
   
   private:
   	  // Constructor to prohibit copy construction.
      MqttSnClient(const MqttSnClient&);

      // Operator= to prohibit copy assignment
      MqttSnClient& operator=(const MqttSnClient&);

      Rf24::I_RfPacketSocket* mSocket;
      uint8_t mGatewayNodeId;
      char mClientId[MAX_LENGTH_CLIENT_ID + 1];

};

} // namespace Mqtt
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_Mqtt_MqttSnClient__hpp
