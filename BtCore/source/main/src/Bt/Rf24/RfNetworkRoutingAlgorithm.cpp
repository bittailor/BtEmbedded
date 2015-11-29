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

RfPipe RfNetworkRoutingAlgorithm::calculateRoutingPipe(RfNode iSelf, RfNode iDestination) {
   if (iSelf.level() >= iDestination.level()){
      return RfPipe::PIPE_0;
   }

   uint8_t idAtLevel[5] = {0};
   idAtLevel[iDestination.level()] = iDestination.id();
   for(uint8_t i = iDestination.level() ; i > iSelf.level() ; i-- ) {
      idAtLevel[i - 1] = (idAtLevel[i] - 1) / 5;
   }

   if (idAtLevel[iSelf.level()] != iSelf.id()) {
      return RfPipe::PIPE_0;
   }

   switch (idAtLevel[iSelf.level() + 1] % 5) {
      case 1 : return RfPipe::PIPE_1;
      case 2 : return RfPipe::PIPE_2;
      case 3 : return RfPipe::PIPE_3;
      case 4 : return RfPipe::PIPE_4;
      case 0 : return RfPipe::PIPE_5;

      default: return RfPipe::PIPE_0;
   }
}

//-------------------------------------------------------------------------------------------------

void RfNetworkRoutingAlgorithm::configurePipe(RfNode iSelf, RfPipe iPipe, I_Rf24DeviceController::Configuration::PipeConfiguration& oPipeConfiguration) {
   if(isLeafNode(iSelf) && iPipe != RfPipe::PIPE_0) {
      oPipeConfiguration.mEnabled = false;
      return;
   }

   oPipeConfiguration.mEnabled = true;
   oPipeConfiguration.mAddress = calculatePipeAddress(iSelf,iPipe);
}

//-------------------------------------------------------------------------------------------------

RfAddress RfNetworkRoutingAlgorithm::calculatePipeAddress(RfNode iSelf, RfPipe iPipe) {
   uint8_t byte0 = iSelf.id();
   uint8_t child = iSelf.id() * 5;
   switch(iPipe) {
      case RfPipe::PIPE_0 : byte0 = iSelf.id(); break;
      case RfPipe::PIPE_1 : byte0 = child + 1; break;
      case RfPipe::PIPE_2 : byte0 = child + 2; break;
      case RfPipe::PIPE_3 : byte0 = child + 3; break;
      case RfPipe::PIPE_4 : byte0 = child + 4; break;
      case RfPipe::PIPE_5 : byte0 = child + 5; break;
   }
   return RfAddress(0xC2,0xC2,0xC2,0xC2,byte0);
}

//-------------------------------------------------------------------------------------------------

bool RfNetworkRoutingAlgorithm::isLeafNode(RfNode iSelf) {
   return iSelf.id() > 50;
}

//-------------------------------------------------------------------------------------------------

} // namespace Rf24
} // namespace Bt
