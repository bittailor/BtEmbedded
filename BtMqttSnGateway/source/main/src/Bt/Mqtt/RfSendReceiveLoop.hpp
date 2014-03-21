//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::RfSendReceiveLoop
//  
//*************************************************************************************************

#ifndef INC__Bt_Mqtt_RfSendReceiveLoop__hpp
#define INC__Bt_Mqtt_RfSendReceiveLoop__hpp

#include <atomic>
#include <thread>

namespace Bt {
namespace Mqtt {

class RfSendReceiveLoop 
{
   public:
      RfSendReceiveLoop();
      ~RfSendReceiveLoop();

      bool start();
   
   private:
   	// Constructor to prohibit copy construction.
      RfSendReceiveLoop(const RfSendReceiveLoop&);

      // Operator= to prohibit copy assignment
      RfSendReceiveLoop& operator=(const RfSendReceiveLoop&);

      void run();
      void workcycle();

      std::atomic<bool> mRunning;
      std::thread mThread;

};

} // namespace Mqtt
} // namespace Bt

#endif // INC__Bt_Mqtt_RfSendReceiveLoop__hpp
