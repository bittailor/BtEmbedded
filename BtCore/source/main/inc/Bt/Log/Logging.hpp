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

namespace Bt {
namespace Log {

enum class LoggingLevel {
      ERROR,
      WARNING,
      INFO,
      DEBUG
};

#define BT_LOG(LEVEL) for (::Bt::Log::LogStatement logStatement(::Bt::Log::LoggingLevel::LEVEL) ; logStatement.once() ; ) logStatement.stream()

class LogStatement {

   public:
      LogStatement(const LogStatement&) = delete;
      LogStatement& operator=(const LogStatement&) = delete;


      LogStatement(LoggingLevel iLevel) : mOnce(false) ,mLevel(iLevel) {

      }

      ~LogStatement() {
         std::cout << message.str() << std::endl;
      }

      std::ostream& stream() {
         return message;
      }

      bool once() {
         if (mOnce) {
            return false;
         }
         mOnce = true;
         return true;
      }

   private:
      bool mOnce;
      LoggingLevel mLevel;
      std::stringstream message;
      

};


} // namespace Log
} // namespace Bt

#endif // INC__Bt_Log_Logging__hpp
