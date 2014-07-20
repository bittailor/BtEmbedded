//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfNetworkRoutingAlgorithm
//  
//*************************************************************************************************

#ifndef INC__Bt_Rf24_RfNetworkRoutingAlgorithm__hpp
#define INC__Bt_Rf24_RfNetworkRoutingAlgorithm__hpp

#include "Bt/Rf24/I_Rf24Controller.hpp"
#include "Bt/Rf24/RfNode.hpp"

namespace Bt {
namespace Rf24 {

class RfNetworkRoutingAlgorithm 
{
   public:
      RfPipe calculateRoutingPipe(RfNode iSelf, RfNode iDestination);
      void configurePipe(RfNode iSelf, RfPipe iPipe, I_Rf24DeviceController::Configuration::PipeConfiguration& oPipeConfiguration);

      RfAddress calculatePipeAddress(RfNode iSelf, RfPipe iPipe);
      bool isLeafNode(RfNode iSelf);
};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfNetworkRoutingAlgorithm__hpp
