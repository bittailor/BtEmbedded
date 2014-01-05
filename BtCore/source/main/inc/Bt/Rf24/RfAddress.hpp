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

      RfAddress(Util::StaticArray<uint8_t,5> pRaw) : mRaw(pRaw){
      }

      RfAddress(uint8_t pByte4, uint8_t pByte3, uint8_t pByte2, uint8_t pByte1, uint8_t pByte0) {
         mRaw[0] = pByte0;
         mRaw[1] = pByte1;
         mRaw[2] = pByte2;
         mRaw[3] = pByte3;
         mRaw[4] = pByte4;
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
