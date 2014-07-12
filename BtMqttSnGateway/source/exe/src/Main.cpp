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

#include <Bt/Log/Logging.hpp>

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
   BT_LOG(INFO) << "Hanlde SIGNAL " << signal ;
   if (sPower != nullptr ) {
      BT_LOG(INFO) << "Power off" ;
      sPower->write(false);
      std::exit(0);
   }
}


//-------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {

   if(argc < 4) {
      std::cout << "usage: " << argv[0] << "broker_url user password" << std::endl ;
      return -1;
   }

   BT_LOG(INFO) << "Main" ;

   Bt::CoreInitializer coreInitializer;
   Bt::Mcu::Pin power(27, Bt::Mcu::Pin::MODE_OUTPUT);
   sPower = &power;
   power.write(false);
   Bt::Util::delayInMilliseconds(10);
   power.write(true);
   Bt::Util::delayInMilliseconds(10);

   Bt::Mqtt::SnGateway gateway(argv[1], argv[2], argv[3]);

   std::signal(SIGINT, signalHandler);

   BT_LOG(INFO) << "gateway run ..." ;
   int result = gateway.run();
   BT_LOG(INFO) << "... gateway end" ;

   power.write(false);

   sPower = nullptr;

   BT_LOG(INFO) << "bye" ;

   return result;
}


//-------------------------------------------------------------------------------------------------

