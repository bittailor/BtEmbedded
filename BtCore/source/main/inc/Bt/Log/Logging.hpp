//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Log::Logging
//  
//*************************************************************************************************

#ifndef INC__Bt_Log_Logging__hpp
#define INC__Bt_Log_Logging__hpp

#include <sstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <iomanip>
#include <ctime>

#include <time.h>

#include "Bt/Util/Timing.hpp"

namespace Bt {
namespace Log {

enum class LoggingLevel {
      ERROR,
      WARNING,
      INFO,
      DEBUG
};

std::ostream& operator<<(std::ostream& iOStream, LoggingLevel iLoggingLevel);

#define BT_LOG(LEVEL) for (::Bt::Log::LogStatement logStatement(::Bt::Log::LoggingLevel::LEVEL) ; logStatement.once() ; ) logStatement.stream()

class LogStatement {

   public:
      LogStatement(const LogStatement&) = delete;
      LogStatement& operator=(const LogStatement&) = delete;


      LogStatement(LoggingLevel iLevel) : mOnce(false) ,mLevel(iLevel) {
//         clock_gettime(CLOCK_REALTIME, &mTimestamp);
      }

      ~LogStatement() {
         {
            std::cout << std::endl;
//            auto timestamp = std::chrono::system_clock::now();
//            std::lock_guard<std::mutex> lock(sMutex);
//            auto time = std::chrono::system_clock::to_time_t(timestamp);
//            auto localTime = std::localtime(&time);

//            std::cout << std::setfill('0') << std::setw(2)
//                      << localTime->tm_mday << "." << localTime->tm_mon << "." << localTime->tm_year
//                      << " " << localTime->tm_hour << ":" << localTime->tm_min << ":" << localTime->tm_sec << " "
//                      << Bt::Util::microseconds()
//                      << "[" << std::hex  << std::this_thread::get_id() << std::dec <<  "]" << "[" << mLevel << "] " << message.str() << std::endl;
         }
      }

      std::ostream& stream() {
         return std::cout;
//         return message;
      }

      bool once() {
         if (mOnce) {
            return false;
         }
         mOnce = true;
         return true;
      }

   private:
      static std::mutex sMutex;

      bool mOnce;
      LoggingLevel mLevel;
      timespec mTimestamp;
      std::stringstream message;
      

};


} // namespace Log
} // namespace Bt

#endif // INC__Bt_Log_Logging__hpp
