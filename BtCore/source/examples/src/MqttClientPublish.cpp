//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  MqttClientPublish
//  
//*************************************************************************************************

#include <iostream>
#include <thread>
#include <chrono>

#include "Bt/Net/Mqtt/MqttClient.hpp"

class Listener : public Bt::Net::Mqtt::MqttClient::I_Listener {
   public:

      virtual bool messageArrived(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::MqttClient::Message> iMessage) {
         std::cout << "messageArrived: "
                  << iTopicName
                  << iMessage->payload
                  << std::endl;
         return true;
      }

   private:

};


//-------------------------------------------------------------------------------------------------

int main(int argc, const char* argv[]) {

   if(argc < 4) {
      std::cout << "usage: " << argv[0] << "server_url user password" << std::endl;
      return -1;
   }


   Listener listener;
   Bt::Net::Mqtt::MqttClient client(listener, argv[1], "ExampleTestId");

   std::cout << "connect ..." << std::endl;
   client.connect(Bt::Net::Mqtt::MqttClient::ConnectOptions{argv[2],argv[3]});

   Bt::Net::Mqtt::MqttClient::Message message {"Hello",false};
   std::cout << "publish ..." << std::endl;
   client.publish("bt/example",message);

   std::cout << "wait ..." << std::endl;
   std::this_thread::sleep_for( std::chrono::seconds(20) );

   std::cout << "disconnect ..." << std::endl;
   client.disconnect(1000);

   std::cout << "bye" << std::endl;

   return 0;
}


//-------------------------------------------------------------------------------------------------

