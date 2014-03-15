//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Logging
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_Logging__hpp
#define INC__Bt_Util_Logging__hpp

#include "Bt/Util/Singleton.hpp"
#include "Bt/Util/I_Logger.hpp"


namespace Bt {
namespace Util {

#define BT_UTIL_LOG_ERROR( message ) \
   (*::Bt::Util::Singleton<::Bt::Util::I_Logger>::instance()) << __FILE__ << __LINE__ << ":E:" << message << ::Bt::Util::endl ;

} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_Logging__hpp
