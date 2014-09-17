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

using Bt::Log::setLoggingLevel;
using Bt::Mcu::Pin;

namespace {
   std::promise<void> sQuitPromise;
   std::future<void> sQuitFuture = sQuitPromise.get_future();
}

void signalHandler(int signal)
{
   std::cout << "Hanlde SIGNAL " << signal << std::endl ;
   sQuitPromise.set_value();
}


//-------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
   std::string settingsFile = "settings.xml";
   if(argc > 1) {
      settingsFile = argv[1];
   }
   std::cout << "Start MQTT-SN Gateway 0.1.1 reading settings from " << settingsFile << std::endl;

   try {

      Bt::Mqtt::SnGatewaySettings settings(settingsFile);

      Bt::Log::setLoggingLevel(settings.logging().level);

      Bt::CoreInitializer coreInitializer;
      Bt::Mcu::Pin power(27, Bt::Mcu::Pin::MODE_OUTPUT);
      power.write(false);
      Bt::Util::delayInMilliseconds(10);
      power.write(true);
      Bt::Util::delayInMilliseconds(10);

      Bt::Mqtt::SnGateway gateway(settings);

      std::signal(SIGTERM, signalHandler);
      std::signal(SIGINT, signalHandler);
      std::signal(SIGHUP, signalHandler);
      std::signal(SIGQUIT, signalHandler);

      std::cout    << "gateway run ..."  << std::endl;

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
      std::cout << "start shutdown of gateway ..." << std::endl;

      gateway.stop();
      gatewayThread.join();

      std::cout << "... gateway down" << std::endl;

      std::cout << "power off ... " << std::endl;
      power.write(false);

      std::cout << "... bye" << std::endl;

   } catch (boost::property_tree::ptree_error failure) {
      std::cerr << std::endl <<  "Problem loading settings from (" << settingsFile << "): \n" << "   " << failure.what() << std::endl << std::endl;
      return -1;
   } catch(std::exception& exception) {
      std::cerr << std::endl << "gateway main failed with: " << exception.what() << std::endl;
      return -1;
   } catch(...) {
      std::cerr << std::endl << "gateway main failed with unknown exception " << std::endl;
      return -1;
   }

   return 0;
}


//-------------------------------------------------------------------------------------------------

