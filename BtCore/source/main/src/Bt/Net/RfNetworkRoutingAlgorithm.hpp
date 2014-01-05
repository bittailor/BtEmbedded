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

#include "Bt/Rf24/I_RfController.hpp"
#include "Bt/Net/RfNode.hpp"

namespace Bt {
namespace Net {

class RfNetworkRoutingAlgorithm 
{
   public:
      Rf24::I_RfController::Pipe calculateRoutingPipe(RfNode pSelf, RfNode pDestination);
      Rf24::I_RfController::Address calculatePipeAddress(RfNode pSelf, Rf24::I_RfController::Pipe pPipe);
      bool isLeafNode(RfNode pSelf);
};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_RfNetworkRoutingAlgorithm__hpp
