//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  BtMqttSnGateway
//  
//*************************************************************************************************


#include <iostream>
#include <csignal>
#include <cstdlib>

#include <Bt/CoreInitializer.hpp>
#include <Bt/Mqtt/SnGateway.hpp>
#include <Bt/Mcu/Pin.hpp>
#include <Bt/Util/Timing.hpp>

using Bt::Mcu::Pin;

namespace {
   Bt::Mcu::Pin* volatile sPower;
}

void signalHandler(int signal)
{
   std::cout << "Hanlde SIGNAL " << signal << std::endl;
   if (sPower != nullptr ) {
      std::cout << "Power off" << std::endl;
      sPower->write(false);
      std::exit(0);
   }
}


//-------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {

   if(argc < 4) {
      std::cout << "usage: " << argv[0] << "broker_url user password" << std::endl;
      return -1;
   }


   std::cout << "Main" << std::endl;

   Bt::CoreInitializer coreInitializer;
   Bt::Mcu::Pin power(4, Bt::Mcu::Pin::MODE_OUTPUT);
   sPower = &power;
   power.write(false);
   Bt::Util::delayInMilliseconds(10);
   power.write(true);
   Bt::Util::delayInMilliseconds(10);

   Bt::Mqtt::SnGateway gateway(argv[1], argv[2], argv[3]);

   std::signal(SIGINT, signalHandler);

   std::cout << "gateway run ..." << std::endl;
   int result = gateway.run();
   std::cout << "... gateway end" << std::endl;

   power.write(false);

   sPower = nullptr;

   std::cout << "bye" << std::endl;

   return result;
}


//-------------------------------------------------------------------------------------------------

