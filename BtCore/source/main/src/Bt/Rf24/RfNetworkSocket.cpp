//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfNetworkSocket
//  
//*************************************************************************************************

#include <stdio.h>
#include <string.h>

#include "Bt/Rf24/RfNetworkSocket.hpp"

namespace Bt {
namespace Rf24 {

//-------------------------------------------------------------------------------------------------

RfNetworkSocket::RfNetworkSocket(RfNode pNodeId, I_Rf24Controller& pController)
: mNodeId(pNodeId), mController(&pController), mListener(0)  {

   I_Rf24Controller::Configuration configuration;

   for(RfPipe pipe : RfPipes::ALL_PIPES) {
      mRouting.configurePipe(mNodeId, pipe, configuration[pipe]);
   }


   mController->configure(configuration);

}

//-------------------------------------------------------------------------------------------------

bool RfNetworkSocket::startListening(I_Listener& pListener) {
   if(mListener != nullptr) {
      return false;
   }

   mListener = &pListener;
   mController->startListening();
   return true;

}

//-------------------------------------------------------------------------------------------------

bool RfNetworkSocket::stopListening() {
   if (mListener == nullptr) {
      return false;
   }
   mListener = nullptr;
   mController->stopListening();
   return true;
}

//-------------------------------------------------------------------------------------------------

bool RfNetworkSocket::send(Packet& pPacket) {
  pPacket.source(mNodeId.id());
  if (pPacket.destination() == mNodeId.id()) {
     receiveInternal(pPacket);
     return true;
  }
  return sendInternal(pPacket);

}
//-------------------------------------------------------------------------------------------------

void RfNetworkSocket::workcycle() {
   if (mListener == nullptr) {
      return;
   }

   size_t limiter = 0;
   while (mController->isDataAvailable() && limiter < 3) {
      limiter++;
      Packet packet;
      if (mController->read(packet.mControllerPackage)) {
         if (packet.destination() != mNodeId.id()) {
            printf(" - route  %i => %i\n",(int)packet.source() ,(int)packet.destination());
            sendInternal(packet);
            continue;
         }
         printf(" - receive  %i => %i\n",(int)packet.source() ,(int)packet.destination());
         receiveInternal(packet);
      }
   }
}

//-------------------------------------------------------------------------------------------------

bool RfNetworkSocket::sendInternal(Packet& pPacket) {
   RfPipe pipe = mRouting.calculateRoutingPipe(mNodeId, pPacket.destination());
   return mController->write(pipe, pPacket.mControllerPackage);
}

//-------------------------------------------------------------------------------------------------

void RfNetworkSocket::receiveInternal(Packet& pPacket) {
   if(mListener != nullptr) {
      mListener->packetReceived(pPacket);
   }
}

//-------------------------------------------------------------------------------------------------

} // namespace Rf24
} // namespace Bt
