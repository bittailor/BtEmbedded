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
#include <sstream>

#include "Bt/Net/Mqtt/MqttClient.hpp"

class Listener : public Bt::Net::Mqtt::MqttClient::I_Listener {
   public:

      virtual void connectionLost(const std::string& iCause) {
         std::cout << "connectionLost: " << iCause << std::endl;
      }


      virtual bool messageArrived(const std::string& iTopicName, std::shared_ptr<Bt::Net::Mqtt::MqttClient::Message> iMessage) {
         std::cout << "messageArrived: "
                   << iTopicName
                   << iMessage->data
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

   const char* topic = "ch/bittailor/test";

   Listener listener;
   Bt::Net::Mqtt::MqttClient client(listener, argv[1], "ExampleMqttClientPub");

   std::cout << "connect ..." << std::endl;
   client.connect(Bt::Net::Mqtt::MqttClient::ConnectOptions{argv[2],argv[3]});

   client.subscribe("ch/bittailor/test",1);

   for (int qos = 0; qos < 3; ++qos) {
      std::cout << "publish qos = " <<  qos  << "..." << std::endl;
      std::stringstream message;
      message << "CppClient Hello " << qos;
      auto future = client.publish(topic, message.str(), qos, false);
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

