//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::StaticArray
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_StaticArray__hpp
#define INC__Bt_Util_StaticArray__hpp

#include <stddef.h>

namespace Bt {
namespace Util {

template<typename T, size_t SIZE>
class StaticArray {
   public:

      enum {ARRAY_SIZE = SIZE};

      typedef T value_type;
      typedef T* iterator;
      typedef T const *  const_iterator;

      StaticArray()
      : mData() {
      }

      StaticArray(const T(& iArray)[SIZE])
      : mData(iArray) {
      }

      ~StaticArray() {
      }

      size_t size() const {
         return SIZE;
      }

      virtual const T& operator[](size_t iIndex) const {
         return mData[iIndex];
      }

      virtual T& operator[](size_t iIndex) {
         return mData[iIndex];
      }

      iterator begin() {
         return mData;
      }

      iterator end() {
         return mData+SIZE;
      }

      const_iterator begin() const {
         return mData;
      }

      const_iterator end() const {
         return mData+SIZE;
      }

   private:
      //StaticArray(const StaticArray<T,SIZE>& iArray);
      //StaticArray& operator=(const StaticArray& iArray);

      void copy(const StaticArray<T,SIZE>& iArray) {
         for (int i = 0; i < iArray.mSize; ++i) {
            mData[i] = iArray[i];
         }
      }

      T mData[SIZE];

};

} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_StaticArray__hpp
