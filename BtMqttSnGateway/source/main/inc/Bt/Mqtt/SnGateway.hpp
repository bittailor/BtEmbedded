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

#include <atomic>

#include <Bt/Concurrency/ExecutionContext.hpp>
#include <Bt/Rf24/RfPacketSocket.hpp>
#include <Bt/Mqtt/I_MqttFactory.hpp>
#include <Bt/Util/Repository.hpp>

#include <Bt/Mqtt/GatewayConnection.hpp>
#include <Bt/Mqtt/SnGatewaySettings.hpp>
#include <Bt/Mqtt/I_SnGatewaySettings.hpp>


namespace Bt {
namespace Mqtt {

class SnGateway
{
   public:
      SnGateway(const I_SnGatewaySettings& iSettings);
      ~SnGateway();
   
      int run();
      void stop();

   private:
   	  // Constructor to prohibit copy construction.
      SnGateway(const SnGateway&);

      // Operator= to prohibit copy assignment
      SnGateway& operator=(const SnGateway&);


      const I_SnGatewaySettings& mSettings;
      Bt::Concurrency::ExecutionContext mWorker;
      std::shared_ptr<Rf24::I_RfPacketSocket> mSocket;
      std::shared_ptr<I_MqttFactory> mMqttFactory;
      Bt::Util::Repository<GatewayConnection> mConnections;
      std::atomic<bool> mRunning;
      std::atomic<bool> mStopping;

};

} // namespace Mqtt
} // namespace Bt

#endif // INC__Bt_Mqtt_SnGateway__hpp
