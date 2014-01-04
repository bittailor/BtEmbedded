//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RfNetworkRoutingAlgorithm
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_RfNetworkRoutingAlgorithm__hpp
#define INC__Bt_Net_RfNetworkRoutingAlgorithm__hpp

#include "Bt/Device/I_RfController.hpp"
#include "Bt/Net/RfNodeId.hpp"

namespace Bt {
namespace Net {

class RfNetworkRoutingAlgorithm 
{
   public:
      Device::I_RfController::Pipe calculateRoutingPipe(RfNodeId pSelf, RfNodeId pDestination);
};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_RfNetworkRoutingAlgorithm__hpp
