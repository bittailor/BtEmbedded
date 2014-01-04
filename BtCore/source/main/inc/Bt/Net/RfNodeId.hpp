//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RfNodeId
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_RfNodeId__hpp
#define INC__Bt_Net_RfNodeId__hpp

#include <stdint.h>

namespace Bt {
namespace Net {

class RfNodeId 
{
   public:
      RfNodeId(uint8_t pId);

      uint8_t id() const { return mId; }
      uint8_t level() const { return mLevel; }




      bool isDirectChild(const RfNodeId& pNodeId) const;



   
   private:
      static uint8_t calculateLevel(uint8_t pId);

      RfNodeId(uint8_t pId, uint8_t pLevel);



      uint8_t mId;
      uint8_t mLevel;
};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_RfNodeId__hpp
