//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Range
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_Range__hpp
#define INC__Bt_Util_Range__hpp

namespace Bt {
namespace Util {

template <typename T>
class Range {
   public:
      Range(T iBegin, T iEnd) : mBegin(iBegin), mEnd(iEnd){
      }

      T begin() { return mBegin; }
      T end() { return mEnd; }
      
   private:
      T mBegin;
      T mEnd;
};

template <typename T>
Range<T> range(T iBegin, T iEnd) {
   return Range<T>(iBegin, iEnd);
}


} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_Range__hpp
