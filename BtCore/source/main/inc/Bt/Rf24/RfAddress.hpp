//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfAddress
//  
//*************************************************************************************************

#ifndef INC__Bt_Rf24_RfAddress__hpp
#define INC__Bt_Rf24_RfAddress__hpp

#include "Bt/Util/StaticArray.hpp"

namespace Bt {
namespace Rf24 {

class RfAddress {
   public:
      RfAddress(){
      }

      RfAddress(Util::StaticArray<uint8_t,5> iRaw) : mRaw(iRaw){
      }

      RfAddress(uint8_t iByte4, uint8_t iByte3, uint8_t iByte2, uint8_t iByte1, uint8_t iByte0) {
         mRaw[0] = iByte0;
         mRaw[1] = iByte1;
         mRaw[2] = iByte2;
         mRaw[3] = iByte3;
         mRaw[4] = iByte4;
      }

      Util::StaticArray<uint8_t,5>& raw() {
         return mRaw;
      }

   private:
      Util::StaticArray<uint8_t,5> mRaw;
};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfAddress__hpp
