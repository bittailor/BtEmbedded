//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Buffer
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_Buffer__hpp
#define INC__Bt_Util_Buffer__hpp

#include <stddef.h>

#include "Bt/Util/StaticArray.hpp"

namespace Bt {
namespace Util {

template<typename T>
class Buffer 
{
   public:
      typedef T value_type;
      typedef T* iterator;
      typedef T const *  const_iterator;


      Buffer(T* iRaw, size_t iSize) : mRaw(iRaw), mSize(iSize)  {
      }

      Buffer(iterator iBegin, iterator iEnd) : mRaw(iBegin), mSize(iEnd-iBegin) {
      }

      template<size_t SIZE>
      Buffer(StaticArray<T,SIZE>& iArray) : mRaw(iArray.begin()), mSize(iArray.size())   {
      }

      ~Buffer() {

      }

      size_t size() const {
         return mSize;
      }
   
      virtual const T& operator[](size_t iIndex) const {
         return mRaw[iIndex];
      }

      virtual T& operator[](size_t iIndex) {
         return mRaw[iIndex];
      }

      iterator begin() {
         return mRaw;
      }

      iterator end() {
         return mRaw+mSize;
      }

      const_iterator begin() const {
         return mRaw;
      }

      const_iterator end() const {
         return mRaw+mSize;
      }

   private:
      T* mRaw;
      size_t mSize;

};

} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_Buffer__hpp
