//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Com::StaticArrayPackageBuffer
//  
//*************************************************************************************************

#ifndef INC__Bt_Com_StaticArrayPackageBuffer__hpp
#define INC__Bt_Com_StaticArrayPackageBuffer__hpp

#include <stdio.h>

#include "Bt/Util/StaticArray.hpp"
#include "Bt/Com/I_PackageBuffer.hpp"

namespace Bt {
namespace Com {

template<size_t SIZE>
class StaticArrayPackageBuffer : public I_PackageBuffer
{
   public:
      StaticArrayPackageBuffer()
      : mPutPosition(0), mGetPosition(0)  {

      }

      ~StaticArrayPackageBuffer() {

      }

      virtual size_t put(uint8_t iValue) {
         if (mPutPosition < SIZE) {
            mData[mPutPosition] = iValue;
            mPutPosition++;
            return 1;
         }
         return 0;
      }

      virtual int16_t get() {
         if (mGetPosition >= SIZE) {
            return EOF;
         }

         if (mGetPosition >= mPutPosition) {
            return EOF;
         }


         return mData[mGetPosition++];
      }

      virtual size_t length() const {
         return mPutPosition - mGetPosition;
      }

      virtual void clear() {
         mPutPosition = 0;
         mGetPosition = 0;
      }

      virtual uint8_t* raw() {
         return &mData[0];
      }

      virtual void filled(size_t iLength) {
         mPutPosition = iLength;
         mGetPosition = 0;
      }

   private:
   	  // Constructor to prohibit copy construction.
      StaticArrayPackageBuffer(const StaticArrayPackageBuffer&);

      // Operator= to prohibit copy assignment
      StaticArrayPackageBuffer& operator=(const StaticArrayPackageBuffer&);

      Util::StaticArray<uint8_t,SIZE> mData;
      size_t mPutPosition;
      size_t mGetPosition;

};

} // namespace Com
} // namespace Bt

#endif // INC__Bt_Com_StaticArrayPackageBuffer__hpp
