//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Log::Logging
//  
//*************************************************************************************************

#include "Bt/Log/Logging.hpp"

#include <cstdlib>
#include <thread>
#include <mutex>
#include <ctime>
#include <time.h>
#include <atomic>
#include <stdexcept>

#include "Bt/Util/Timing.hpp"


namespace Bt {
namespace Log {

namespace {
   std::mutex sMutex;
   std::atomic<LoggingLevel> sLevel(LoggingLevel::ERROR);

   const char* toMarkerString(LoggingLevel iLoggingLevel) {
      switch(iLoggingLevel) {
         case LoggingLevel::ERROR   : return "[Error]   ";
         case LoggingLevel::WARNING : return "[Warning] ";
         case LoggingLevel::INFO    : return "[Info]    ";
         case LoggingLevel::DEBUG   : return "[Debug]   ";
      }
      throw std::invalid_argument("invalid logging level");
   }

}

//-------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& iOStream, LoggingLevel iLoggingLevel) {
   switch(iLoggingLevel) {
      case LoggingLevel::ERROR   : iOStream << "Error";   break;
      case LoggingLevel::WARNING : iOStream << "Warning"; break;
      case LoggingLevel::INFO    : iOStream << "Info";    break;
      case LoggingLevel::DEBUG   : iOStream << "Debug";   break;
   }
   return iOStream;
}

//-------------------------------------------------------------------------------------------------

LoggingLevel setLoggingLevel(LoggingLevel iLoggingLevel) {
   return sLevel.exchange(iLoggingLevel);
}


//-------------------------------------------------------------------------------------------------

LogStatement::LogStatement(LoggingLevel iLevel) : mNeedWrite(iLevel <= sLevel.load()), mLevel(iLevel) {
}

//-------------------------------------------------------------------------------------------------


LogStatement::~LogStatement() {
   if(!message.str().empty()) {
      auto timestamp = std::chrono::system_clock::now();
      std::lock_guard<std::mutex> lock(sMutex);
      auto time = std::chrono::system_clock::to_time_t(timestamp);
      auto localTime = std::localtime(&time);

      std::cout << std::setfill('0')
      << std::setw(2) << localTime->tm_mday << "."  << std::setw(2) << localTime->tm_mon << "." << std::setw(4) << (localTime->tm_year + 1900)
      << " " << std::setw(2) << localTime->tm_hour << ":" << std::setw(2) << localTime->tm_min << ":" << std::setw(2) << localTime->tm_sec << " "
      << std::setfill(' ') << std::setw(6) << (Bt::Util::microseconds() % (1000 * 1000) )
      << "[" << std::hex << std::setw(8) << std::this_thread::get_id() << "]" << std::dec
      << toMarkerString(mLevel) << message.str() << std::endl;
   }
}

//-------------------------------------------------------------------------------------------------

} // namespace Log
} // namespace Bt

// Simply callable from the debugger
Bt::Log::LoggingLevel bt_log_setLoggingLevel(Bt::Log::LoggingLevel iLoggingLevel) {
   return Bt::Log::setLoggingLevel(iLoggingLevel);
}

//-------------------------------------------------------------------------------------------------
