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
};

} // namespace Mqtt
} // namespace Bt

#endif // INC__Bt_Mqtt_SnGateway__hpp
