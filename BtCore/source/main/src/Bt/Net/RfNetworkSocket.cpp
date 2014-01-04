//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RfNetworkSocket
//  
//*************************************************************************************************

#include <string.h>

#include "Bt/Net/RfNetworkSocket.hpp"

namespace Bt {
namespace Net {

//-------------------------------------------------------------------------------------------------

RfNetworkSocket::RfNetworkSocket(RfNode pNodeId, Device::I_RfController& pController)
: mNodeId(pNodeId), mController(&pController)  {

}

//-------------------------------------------------------------------------------------------------

void RfNetworkSocket::workcycle() {
   size_t limiter = 0;
   while (mController->isDataAvailable() && limiter < 3) {
      limiter++;
      Packet packet;
      if (mController->read(packet.mControllerPackage)) {
         if (packet.destination() != mNodeId.id()) {
            write(packet);
            continue;
         }


      }
   }
}

//-------------------------------------------------------------------------------------------------

void RfNetworkSocket::writeDatagram(RfNode pDestination, uint8_t* pBuffer, size_t pSize) {
   Packet packet;
   packet.source(mNodeId.id());
   packet.destination(pDestination.id());
   packet.writePayload(pBuffer, pSize);
   write(packet);
}

//-------------------------------------------------------------------------------------------------

bool RfNetworkSocket::write(Packet& packet) {
   Device::I_RfController::Pipe pipe = mRouting.calculateRoutingPipe(mNodeId, packet.destination());
   return mController->write(pipe, packet.mControllerPackage);
}

//-------------------------------------------------------------------------------------------------

} // namespace Net
} // namespace Bt
