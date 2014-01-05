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

#include "Bt/Rf24/I_Rf24Controller.hpp"
#include "Bt/Net/RfNode.hpp"

namespace Bt {
namespace Net {

class RfNetworkRoutingAlgorithm 
{
   public:
      Rf24::I_Rf24Controller::Pipe calculateRoutingPipe(RfNode pSelf, RfNode pDestination);
      Rf24::I_Rf24Controller::Address calculatePipeAddress(RfNode pSelf, Rf24::I_Rf24Controller::Pipe pPipe);
      bool isLeafNode(RfNode pSelf);
};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_RfNetworkRoutingAlgorithm__hpp
