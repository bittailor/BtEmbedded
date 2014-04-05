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
   Bt::Net::Mqtt::MqttClient client(listener, argv[1], "ExampleMqttClientPub");

   std::cout << "connect ..." << std::endl;
   client.connect(Bt::Net::Mqtt::MqttClient::ConnectOptions{argv[2],argv[3]});

   for (int qos = 0; qos < 3; ++qos) {
      std::cout << "publish qos = " <<  qos  << "..." << std::endl;
      auto message = std::string("CppClient Hello ") + std::to_string(qos);
      auto future = client.publish("ch/bittailor/test", message, qos, false);
      std::cout << "... wait for publish future ..." << std::endl;
      bool delivery = future.get();
      std::cout << "... delivery = "  << delivery << std::endl;
   }



   std::cout << "disconnect ..." << std::endl;
   client.disconnect(1000);

   std::cout << "bye" << std::endl;

   return 0;
}


//-------------------------------------------------------------------------------------------------

