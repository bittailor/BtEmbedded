//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::CollectionUtility
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_CollectionUtility__hpp
#define INC__Bt_Util_CollectionUtility__hpp

#include <cstddef>

namespace Bt {
namespace Util {

template<typename T, int N> std::size_t sizeOfArray(const T (&pArray)[N]) {
   return N;
}


} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_CollectionUtility__hpp
