//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RfNodeId
//  
//*************************************************************************************************

#include "Bt/Net/RfNodeId.hpp"

namespace Bt {
namespace Net {


//-------------------------------------------------------------------------------------------------

RfNodeId::RfNodeId(uint8_t pId) : mId(pId), mLevel(calculateLevel(pId)) {

}

//-------------------------------------------------------------------------------------------------

RfNodeId::RfNodeId(uint8_t pId, uint8_t pLevel) : mId(pId), mLevel(pLevel) {

}

//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------

bool RfNodeId::isDirectChild(const RfNodeId& pNodeId) const {
   if (mLevel >= pNodeId.level()) {
      return false;
   }
   uint8_t leveldiff = pNodeId.level() - mLevel;
   uint8_t id = pNodeId.id();

   for(uint8_t i = 0 ; i < leveldiff ; i++ ) {
      id = (id - 1) / 5;
   }
   return mId == id;
}

//-------------------------------------------------------------------------------------------------

uint8_t RfNodeId::calculateLevel(uint8_t pId) {
   if (pId <   1) return 0;
   if (pId <   6) return 1;
   if (pId <  31) return 2;
   if (pId < 155) return 3;
   return 4;
}

//-------------------------------------------------------------------------------------------------

} // namespace Net
} // namespace Bt
