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
   std::string settingsFile = "settings.xml";
   try {

      if(argc > 1) {
         settingsFile = argv[1];
      }

      BT_LOG(INFO) << "Main" ;

      Bt::CoreInitializer coreInitializer;
      Bt::Mcu::Pin power(27, Bt::Mcu::Pin::MODE_OUTPUT);
      power.write(false);
      Bt::Util::delayInMilliseconds(10);
      power.write(true);
      Bt::Util::delayInMilliseconds(10);

      Bt::Mqtt::SnGatewaySettings settings(settingsFile);
      Bt::Mqtt::SnGateway gateway(settings);

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

