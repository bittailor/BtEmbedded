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

Rf24::I_RfController::Pipe RfNetworkRoutingAlgorithm::calculateRoutingPipe(RfNode pSelf, RfNode pDestination) {
   if (pSelf.level() >= pDestination.level()){
      return Rf24::I_RfController::Pipe::PIPE_0;
   }

   uint8_t idAtLevel[4] = {0};
   idAtLevel[pDestination.level()] = pDestination.id();
   for(uint8_t i = pDestination.level() ; i > pSelf.level() ; i-- ) {
      idAtLevel[i - 1] = (idAtLevel[i] - 1) / 5;
   }

   if (idAtLevel[pSelf.level()] != pSelf.id()) {
      return Rf24::I_RfController::Pipe::PIPE_0;
   }

   switch (idAtLevel[pSelf.level() + 1] % 5) {
      case 1 : return Rf24::I_RfController::Pipe::PIPE_1;
      case 2 : return Rf24::I_RfController::Pipe::PIPE_2;
      case 3 : return Rf24::I_RfController::Pipe::PIPE_3;
      case 4 : return Rf24::I_RfController::Pipe::PIPE_4;
      case 0 : return Rf24::I_RfController::Pipe::PIPE_5;

      default: return Rf24::I_RfController::Pipe::PIPE_0;
   }
}

//-------------------------------------------------------------------------------------------------

Rf24::I_RfController::Address RfNetworkRoutingAlgorithm::calculatePipeAddress(RfNode pSelf, Rf24::I_RfController::Pipe pPipe) {
   uint8_t byte0;
   uint8_t child = pSelf.id() * 5;
   switch(pPipe) {
      case Rf24::I_RfController::Pipe::PIPE_0 : byte0 = pSelf.id(); break;
      case Rf24::I_RfController::Pipe::PIPE_1 : byte0 = child + 1; break;
      case Rf24::I_RfController::Pipe::PIPE_2 : byte0 = child + 2; break;
      case Rf24::I_RfController::Pipe::PIPE_3 : byte0 = child + 3; break;
      case Rf24::I_RfController::Pipe::PIPE_4 : byte0 = child + 4; break;
      case Rf24::I_RfController::Pipe::PIPE_5 : byte0 = child + 5; break;
   }
   return Rf24::I_RfController::Address(0xC2,0xC2,0xC2,0xC2,byte0);
}

//-------------------------------------------------------------------------------------------------

bool RfNetworkRoutingAlgorithm::isLeafNode(RfNode pSelf) {
   return pSelf.id() > 50;
}

//-------------------------------------------------------------------------------------------------

} // namespace Net
} // namespace Bt
