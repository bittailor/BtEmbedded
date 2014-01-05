//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfNetworkSocket
//  
//*************************************************************************************************

#include <string.h>

#include "Bt/Rf24/RfNetworkSocket.hpp"

namespace Bt {
namespace Rf24 {

//-------------------------------------------------------------------------------------------------

RfNetworkSocket::RfNetworkSocket(RfNode pNodeId, Rf24::I_Rf24Controller& pController)
: mNodeId(pNodeId), mController(&pController), mListener(0)  {

   Rf24::I_Rf24Controller::Configuration configuration;


/*
   mController->enablePipe(Device::I_RfController::Pipe::PIPE_0, mRouting.calculatePipeAddress(mNodeId, Device::I_RfController::Pipe::PIPE_0));
   if(!mRouting.isLeafNode(mNodeId)) {
      mController->enablePipe(Device::I_RfController::Pipe::PIPE_1,
                                  mRouting.calculatePipeAddress(mNodeId, Device::I_RfController::Pipe::PIPE_1));
      mController->enablePipe(Device::I_RfController::Pipe::PIPE_2,
                                  mRouting.calculatePipeAddress(mNodeId, Device::I_RfController::Pipe::PIPE_2));
      mController->enablePipe(Device::I_RfController::Pipe::PIPE_3,
                                  mRouting.calculatePipeAddress(mNodeId, Device::I_RfController::Pipe::PIPE_3));
      mController->enablePipe(Device::I_RfController::Pipe::PIPE_4,
                                  mRouting.calculatePipeAddress(mNodeId, Device::I_RfController::Pipe::PIPE_4));
      mController->enablePipe(Device::I_RfController::Pipe::PIPE_5,
                                  mRouting.calculatePipeAddress(mNodeId, Device::I_RfController::Pipe::PIPE_5));
   }
*/


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
            sendInternal(packet);
            continue;
         }
         receiveInternal(packet);
      }
   }
}

//-------------------------------------------------------------------------------------------------

bool RfNetworkSocket::sendInternal(Packet& pPacket) {
   Rf24::I_Rf24Controller::Pipe pipe = mRouting.calculateRoutingPipe(mNodeId, pPacket.destination());
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
