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

#include <boost/log/trivial.hpp>

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
   BOOST_LOG_TRIVIAL(info) << "Hanlde SIGNAL " << signal ;
   if (sPower != nullptr ) {
      BOOST_LOG_TRIVIAL(info) << "Power off" ;
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


   BOOST_LOG_TRIVIAL(info) << "Main" ;

   Bt::CoreInitializer coreInitializer;
   Bt::Mcu::Pin power(4, Bt::Mcu::Pin::MODE_OUTPUT);
   sPower = &power;
   power.write(false);
   Bt::Util::delayInMilliseconds(10);
   power.write(true);
   Bt::Util::delayInMilliseconds(10);

   Bt::Mqtt::SnGateway gateway(argv[1], argv[2], argv[3]);

   std::signal(SIGINT, signalHandler);

   BOOST_LOG_TRIVIAL(info) << "gateway run ..." ;
   int result = gateway.run();
   BOOST_LOG_TRIVIAL(info) << "... gateway end" ;

   power.write(false);

   sPower = nullptr;

   BOOST_LOG_TRIVIAL(info) << "bye" ;

   return result;
}


//-------------------------------------------------------------------------------------------------

