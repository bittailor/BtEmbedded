//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::SnGateway
//  
//*************************************************************************************************

#ifndef INC__Bt_Mqtt_SnGateway__hpp
#define INC__Bt_Mqtt_SnGateway__hpp

#include <Bt/Rf24/RfPacketSocket.hpp>


namespace Bt {
namespace Mqtt {

class SnGateway 
{
   public:
      SnGateway();
      ~SnGateway();
   
      int run();

   private:
   	  // Constructor to prohibit copy construction.
      SnGateway(const SnGateway&);

      // Operator= to prohibit copy assignment
      SnGateway& operator=(const SnGateway&);

      //Rf24::RfPacketSocket mPacketSocket;

};

} // namespace Mqtt
} // namespace Bt

#endif // INC__Bt_Mqtt_SnGateway__hpp
