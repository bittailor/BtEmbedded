//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfPacketSocketFactory
//  
//*************************************************************************************************

#ifndef INC__Bt_Rf24_RfPacketSocketFactory__hpp
#define INC__Bt_Rf24_RfPacketSocketFactory__hpp

#include <memory>
#include <Bt/Rf24/I_RfPacketSocket.hpp>
#include "Bt/Mcu/I_Spi.hpp"

namespace Bt {
namespace Rf24 {

class RfPacketSocketFactory 
{
   public:
      RfPacketSocketFactory() = default;

      virtual std::shared_ptr<I_RfPacketSocket> createPacketSocket(uint8_t iChipEnable, Mcu::I_Spi::ChipSelect iChipSelect, uint8_t iIrq, uint8_t iNodeId);
   
   private:
   	  // Constructor to prohibit copy construction.
      RfPacketSocketFactory(const RfPacketSocketFactory&);

      // Operator= to prohibit copy assignment
      RfPacketSocketFactory& operator=(const RfPacketSocketFactory&);
};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfPacketSocketFactory__hpp
