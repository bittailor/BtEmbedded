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
#include <functional>

#include <Bt/Log/Logging.hpp>
#include "Bt/Rf24/RfNetworkSocket.hpp"

namespace Bt {
namespace Rf24 {

//-------------------------------------------------------------------------------------------------

RfNetworkSocket::RfNetworkSocket(RfNode iNodeId, I_Rf24DeviceController& iController)
: mNodeId(iNodeId), mController(&iController), mIdCounter(0) {

   uint8_t autoRetransmitDelay = ((mNodeId.id() % 6) * 2) + 5;
   I_Rf24DeviceController::Configuration configuration(autoRetransmitDelay);
   for(RfPipe pipe : RfPipes::ALL_PIPES) {
      mRouting.configurePipe(mNodeId, pipe, configuration[pipe]);
   }
   mController->configure(configuration);
}

//-------------------------------------------------------------------------------------------------

bool RfNetworkSocket::send(Packet& iPacket) {
  iPacket.source(mNodeId.id());
  iPacket.id(mIdCounter++);
  if (iPacket.destination() == mNodeId.id()) {
     receiveInternal(iPacket);
     return true;
  }
  return sendInternal(iPacket);
}

//-------------------------------------------------------------------------------------------------

void RfNetworkSocket::startListening(std::function<void(Packet& iPacket)> iCallback) {
   mCallback = iCallback;
   mController->startListening(std::bind(&RfNetworkSocket::onPacketReceived,this,std::placeholders::_1,std::placeholders::_2));
}

//-------------------------------------------------------------------------------------------------

void RfNetworkSocket::stopListening() {
   mController->stopListening();
   mCallback = std::function<void(Packet& iPacket)>();
}

//-------------------------------------------------------------------------------------------------

bool RfNetworkSocket::sendInternal(Packet& iPacket) {
   RfPipe pipe = mRouting.calculateRoutingPipe(mNodeId, iPacket.destination());

   int counter = 0;
   while(!mController->write(pipe, iPacket.mControllerPackage)) {
      counter++;
      if (counter >= 5) {
         return false;
      }
      BT_LOG(WARNING) << "send failed do retry " << counter <<  " after delay";
      Util::delayInMicroseconds(counter*100);
   }

   return true;
}

//-------------------------------------------------------------------------------------------------

void RfNetworkSocket::receiveInternal(Packet& iPacket) {
   if(mCallback) {
      mCallback(iPacket);
   }
}

//-------------------------------------------------------------------------------------------------

void RfNetworkSocket::onPacketReceived(RfPipe iPipe, I_Rf24DeviceController::Packet& iPacket) {
   Packet packet(iPacket);
   if (packet.destination() != mNodeId.id()) {
      BT_LOG(DEBUG) << " - route  " << (int)packet.source() << " => " << (int)packet.destination();
      sendInternal(packet);
      return;
   }
   BT_LOG(DEBUG) << " - receive  " << (int)packet.source() << " => " << (int)packet.destination();
   receiveInternal(packet);
}

//-------------------------------------------------------------------------------------------------

} // namespace Rf24
} // namespace Bt
