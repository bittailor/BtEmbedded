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
#include "Bt/Util/Logging.hpp"

namespace Bt {
namespace Rf24 {

//-------------------------------------------------------------------------------------------------

RfNetworkSocket::RfNetworkSocket(RfNode pNodeId, I_Rf24DeviceController& pController)
: mNodeId(pNodeId), mController(&pController), mIdCounter(0), mListener(nullptr)  {

   for(StoredPackage& package : mPackages) {
      mFree.pushBack(package);
   }

   uint8_t autoRetransmitDelay = ((mNodeId.id() % 6) * 2) + 5;
   I_Rf24DeviceController::Configuration configuration(autoRetransmitDelay);

   for(RfPipe pipe : RfPipes::ALL_PIPES) {
      mRouting.configurePipe(mNodeId, pipe, configuration[pipe]);
   }

   mController->configure(configuration);
   mController->startListening();

}


//-------------------------------------------------------------------------------------------------

bool RfNetworkSocket::send(Packet& pPacket) {
  pPacket.source(mNodeId.id());
  pPacket.id(mIdCounter++);
  if (pPacket.destination() == mNodeId.id()) {
     receiveInternal(pPacket);
     return true;
  }
  return sendInternal(pPacket);

}

//-------------------------------------------------------------------------------------------------

bool RfNetworkSocket::receive(Packet& pPacket) {
   while(mReceived.empty()) {
      workcycle();
   }

   StoredPackage& received = *mReceived.begin();
   mReceived.remove(received);

   received.copy(pPacket);

   mFree.pushBack(received);
   return true;
}

//-------------------------------------------------------------------------------------------------

bool RfNetworkSocket::available() {
   if(mReceived.empty()) {
      workcycle();
   }
   return !mReceived.empty();
}

//-------------------------------------------------------------------------------------------------

void RfNetworkSocket::workcycle() {
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

bool RfNetworkSocket::setListener(I_Listener& iListener) {
   if(mListener != nullptr) {
      return false;
   }

   mListener = &iListener;

   while(!mReceived.empty()) {
      StoredPackage& received = *mReceived.begin();
      mReceived.remove(received);
      mListener->packetReceived(received);
      mFree.pushBack(received);
   }

   return true;

}

//-------------------------------------------------------------------------------------------------

bool RfNetworkSocket::resetListener() {
   if (mListener == nullptr) {
      return false;
   }
   mListener = nullptr;
   return true;
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
      return;
   }

   if (mFree.empty()) {
      BT_UTIL_LOG_ERROR("drop packet");
      return;
   }

   StoredPackage& storage = *mFree.begin();
   mFree.remove(storage);
   pPacket.copy(storage);
   mReceived.pushBack(storage);

}

//-------------------------------------------------------------------------------------------------

} // namespace Rf24
} // namespace Bt
