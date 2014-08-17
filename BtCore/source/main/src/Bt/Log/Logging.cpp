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

namespace Bt {
namespace Log {

namespace {

   LoggingLevel getLoggingLevel() {
      const char* level = std::getenv("BT_CORE_LOGGING_LEVEL");
      if (level != nullptr) {
         std::cout << "BT_CORE_LOGGING_LEVEL is " <<  level << std::endl;
         std::string levelName(level);
         if(levelName == "ERROR")   return LoggingLevel::ERROR;
         if(levelName == "WARNING") return LoggingLevel::WARNING;
         if(levelName == "INFO")    return LoggingLevel::INFO;
         if(levelName == "DEBUG")   return LoggingLevel::DEBUG;
      }
      std::cout << "BT_CORE_LOGGING_LEVEL is not set => ERROR" << std::endl;
      return LoggingLevel::ERROR;
   }

   LoggingLevel sLevel = getLoggingLevel();
}

LogStatement::LogStatement(LoggingLevel iLevel) : mOnce(iLevel > sLevel) ,mLevel(iLevel) {
}

std::ostream& operator<<(std::ostream& iOStream, LoggingLevel iLoggingLevel) {
   switch(iLoggingLevel) {
      case LoggingLevel::ERROR   : iOStream << "ERROR";   break;
      case LoggingLevel::WARNING : iOStream << "WARNING"; break;
      case LoggingLevel::INFO    : iOStream << "INFO";    break;
      case LoggingLevel::DEBUG   : iOStream << "DEBUG";   break;
   }
   return iOStream;
}

std::mutex LogStatement::sMutex;


} // namespace Log
} // namespace Bt


