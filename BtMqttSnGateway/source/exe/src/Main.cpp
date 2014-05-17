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

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/unlocked_frontend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/utility/empty_deleter.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>


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

//   boost::log::add_file_log
//   (
//            boost::log::keywords::file_name = "mqtt-sn-gw_%N.log",
//            boost::log::keywords::rotation_size = 10 * 1024 * 1024
//   );
//
//
//   boost::shared_ptr< boost::log::core > core = boost::log::core::get();
//
//   // Set a sink that will write log records to the console
//   boost::shared_ptr< boost::log::sinks::text_ostream_backend > backend =
//            boost::make_shared< boost::log::sinks::text_ostream_backend >();
//   backend->add_stream(boost::shared_ptr< std::ostream >(&std::cout, boost::empty_deleter()));
//
//   typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > sink_t;
//       boost::shared_ptr< sink_t > sink(new sink_t(backend));
//   core->add_sink(sink);


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

