//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfNetworkRoutingAlgorithm
//  
//*************************************************************************************************

#include "Bt/Rf24/RfNetworkRoutingAlgorithm.hpp"

namespace Bt {
namespace Rf24 {

//-------------------------------------------------------------------------------------------------

RfPipe RfNetworkRoutingAlgorithm::calculateRoutingPipe(RfNode pSelf, RfNode pDestination) {
   if (pSelf.level() >= pDestination.level()){
      return RfPipe::PIPE_0;
   }

   uint8_t idAtLevel[4] = {0};
   idAtLevel[pDestination.level()] = pDestination.id();
   for(uint8_t i = pDestination.level() ; i > pSelf.level() ; i-- ) {
      idAtLevel[i - 1] = (idAtLevel[i] - 1) / 5;
   }

   if (idAtLevel[pSelf.level()] != pSelf.id()) {
      return RfPipe::PIPE_0;
   }

   switch (idAtLevel[pSelf.level() + 1] % 5) {
      case 1 : return RfPipe::PIPE_1;
      case 2 : return RfPipe::PIPE_2;
      case 3 : return RfPipe::PIPE_3;
      case 4 : return RfPipe::PIPE_4;
      case 0 : return RfPipe::PIPE_5;

      default: return RfPipe::PIPE_0;
   }
}

//-------------------------------------------------------------------------------------------------

RfAddress RfNetworkRoutingAlgorithm::calculatePipeAddress(RfNode pSelf, RfPipe pPipe) {
   uint8_t byte0;
   uint8_t child = pSelf.id() * 5;
   switch(pPipe) {
      case RfPipe::PIPE_0 : byte0 = pSelf.id(); break;
      case RfPipe::PIPE_1 : byte0 = child + 1; break;
      case RfPipe::PIPE_2 : byte0 = child + 2; break;
      case RfPipe::PIPE_3 : byte0 = child + 3; break;
      case RfPipe::PIPE_4 : byte0 = child + 4; break;
      case RfPipe::PIPE_5 : byte0 = child + 5; break;
   }
   return RfAddress(0xC2,0xC2,0xC2,0xC2,byte0);
}

//-------------------------------------------------------------------------------------------------

bool RfNetworkRoutingAlgorithm::isLeafNode(RfNode pSelf) {
   return pSelf.id() > 50;
}

//-------------------------------------------------------------------------------------------------

} // namespace Rf24
} // namespace Bt
