//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Demangling
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_Demangling__hpp
#define INC__Bt_Util_Demangling__hpp

#include <string>

namespace Bt {
namespace Util {

std::string demangle(const std::type_info& type);

} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_Demangling__hpp
