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
#include <iomanip>

#include <boost/io/ios_state.hpp>


namespace Bt {
namespace Log {

//-------------------------------------------------------------------------------------------------


enum class LoggingLevel {
      ERROR,
      WARNING,
      INFO,
      DEBUG
};

//-------------------------------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& iOStream, LoggingLevel iLoggingLevel);

//-------------------------------------------------------------------------------------------------


#define BT_LOG(LEVEL) for (::Bt::Log::LogStatement logStatement(::Bt::Log::LoggingLevel::LEVEL) ; logStatement.needWrite() ; logStatement.written() ) logStatement.stream()

//-------------------------------------------------------------------------------------------------

LoggingLevel setLoggingLevel(LoggingLevel iLoggingLevel);

//-------------------------------------------------------------------------------------------------

class LogStatement {

   public:
      LogStatement(const LogStatement&) = delete;
      LogStatement& operator=(const LogStatement&) = delete;


      LogStatement(LoggingLevel iLevel);
      ~LogStatement();

      std::ostream& stream() {
         return message;
      }

      bool needWrite() {
         return mNeedWrite;
      }

      void written() {
         mNeedWrite = false;
      }

   private:

      bool mNeedWrite;
      LoggingLevel mLevel;
      std::stringstream message;
      
};

//-------------------------------------------------------------------------------------------------

template<typename T> struct HexLogStreamer {
      const int witdh;
      const T value;

};

//-------------------------------------------------------------------------------------------------

template<typename T> std::ostream& operator<<(std::ostream& iOut, const HexLogStreamer<T>& iStreamer) {
   boost::io::ios_flags_saver  ifs( iOut );
   iOut << std::showbase << std::internal << std::setfill('0') << std::setw(iStreamer.witdh) << std::hex << iStreamer.value;
   return iOut;
}

//-------------------------------------------------------------------------------------------------

} // namespace Log
} // namespace Bt

#endif // INC__Bt_Log_Logging__hpp
