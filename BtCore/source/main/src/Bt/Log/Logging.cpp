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


namespace Bt {
namespace Log {

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


