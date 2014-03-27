//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::I_Logger
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_I_Logger__hpp
#define INC__Bt_Util_I_Logger__hpp

namespace Bt {
namespace Util {

class I_Logger {
   public:
      virtual ~I_Logger() {}
      
      virtual I_Logger& operator<<(I_Logger& (*pManuipulator)(I_Logger&)) = 0;
      virtual I_Logger& operator<<(const char* pCString) = 0;
      virtual I_Logger& operator<<(int pInteger) = 0;

      virtual void flush() = 0;
      virtual void endl() = 0;
      virtual void hex() = 0;
      virtual void dec() = 0;
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

inline I_Logger& flush(I_Logger& iLogger) {
   iLogger.flush();
   return iLogger;
}

//-------------------------------------------------------------------------------------------------

inline I_Logger& endl(I_Logger& iLogger){
   iLogger.endl();
   return iLogger;
}

//-------------------------------------------------------------------------------------------------

inline I_Logger& hex(I_Logger& iLogger){
   iLogger.hex();
   return iLogger;
}

//-------------------------------------------------------------------------------------------------

inline I_Logger& dec(I_Logger& iLogger){
   iLogger.dec();
   return iLogger;
}

//-------------------------------------------------------------------------------------------------

} // namespace Util
} // namespace Bt

namespace Log = Bt::Util;

#endif // INC__Bt_Util_I_Logger__hpp
