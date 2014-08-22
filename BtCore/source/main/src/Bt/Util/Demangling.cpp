//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Demangling
//  
//*************************************************************************************************

#include "Bt/Util/Demangling.hpp"

#include <cstdlib>

#include <cxxabi.h>

#include <boost/lexical_cast.hpp>

namespace Bt {
namespace Util {


//-------------------------------------------------------------------------------------------------

std::string demangle(const std::type_info& type) {
   int status;
   auto demagled = abi::__cxa_demangle(type.name(), 0, 0, &status);
   if(demagled == nullptr) {
      throw std::runtime_error("demangling failed with" + boost::lexical_cast<std::string>(status));
   }
   std::string realname(demagled);
   std::free(demagled);
   return std::move(realname);
}

//-------------------------------------------------------------------------------------------------

} // namespace Util
} // namespace Bt
