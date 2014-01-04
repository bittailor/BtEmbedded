//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RfNetworkRoutingAlgorithm
//  
//*************************************************************************************************

#include "Bt/Net/RfNetworkRoutingAlgorithm.hpp"

namespace Bt {
namespace Net {

//-------------------------------------------------------------------------------------------------

Device::I_RfController::Pipe RfNetworkRoutingAlgorithm::calculateRoutingPipe(RfNodeId pSelf, RfNodeId pDestination) {
   if (pSelf.level() >= pDestination.level()){
      return Device::I_RfController::Pipe::PIPE_0;
   }

   uint8_t idAtLevel[4] = {0};
   idAtLevel[pDestination.level()] = pDestination.id();
   for(uint8_t i = pDestination.level() ; i > pSelf.level() ; i-- ) {
      idAtLevel[i - 1] = (idAtLevel[i] - 1) / 5;
   }

   if (idAtLevel[pSelf.level()] != pSelf.id()) {
      return Device::I_RfController::Pipe::PIPE_0;
   }

   switch (idAtLevel[pSelf.level() + 1] % 5) {
      case 1 : return Device::I_RfController::Pipe::PIPE_1;
      case 2 : return Device::I_RfController::Pipe::PIPE_2;
      case 3 : return Device::I_RfController::Pipe::PIPE_3;
      case 4 : return Device::I_RfController::Pipe::PIPE_4;
      case 0 : return Device::I_RfController::Pipe::PIPE_5;

      default: return Device::I_RfController::Pipe::PIPE_0;
   }
}

//-------------------------------------------------------------------------------------------------

} // namespace Net
} // namespace Bt
