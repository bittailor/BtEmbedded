//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RfNode
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_RfNode__hpp
#define INC__Bt_Net_RfNode__hpp

#include <stdint.h>

namespace Bt {
namespace Net {

class RfNode 
{
   public:
      RfNode(uint8_t pId);

      uint8_t id() const { return mId; }
      uint8_t level() const { return mLevel; }

   private:
      static uint8_t calculateLevel(uint8_t pId);

      uint8_t mId;
      uint8_t mLevel;
};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_RfNode__hpp
