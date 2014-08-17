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
   std::promise<void> sQuitPromise;
   std::future<void> sQuitFuture = sQuitPromise.get_future();
}

void signalHandler(int signal)
{
   BT_LOG(INFO) << "Hanlde SIGNAL " << signal ;
   sQuitPromise.set_value();
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
   power.write(false);
   Bt::Util::delayInMilliseconds(10);
   power.write(true);
   Bt::Util::delayInMilliseconds(10);

   Bt::Mqtt::SnGateway gateway(argv[1], argv[2], argv[3]);

   std::signal(SIGINT, signalHandler);

   BT_LOG(INFO) << "gateway run ..." ;

   std::thread gatewayThread([&gateway](){
      try {
         gateway.run();
      } catch(std::exception& exception) {
         BT_LOG(ERROR) << "gateway.run() failed with: " << exception.what();
      } catch(...) {
         BT_LOG(ERROR) << "gateway.run() failed with unknown exception ";
      }
   });

   sQuitFuture.get();
   gateway.stop();
   gatewayThread.join();

   BT_LOG(INFO) << "... gateway end" ;

   BT_LOG(INFO) << "Power off" ;
   power.write(false);

   BT_LOG(INFO) << "bye" ;

   return 0;
}


//-------------------------------------------------------------------------------------------------

