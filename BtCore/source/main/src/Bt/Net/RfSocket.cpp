//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RfSocket
//  
//*************************************************************************************************

#include <algorithm>

#include "Bt/Device/I_DeviceNRf24L01P.hpp"
#include "Bt/Net/RfSocket.hpp"

namespace Bt {
namespace Net {

namespace {


}

//-------------------------------------------------------------------------------------------------

RfSocket::RfSocket(NodeId pNodeId, Device::I_RfController& pController) : mNodeId(pNodeId), mController(&pController) {

}

//-------------------------------------------------------------------------------------------------

RfSocket::~RfSocket() {

}

//-------------------------------------------------------------------------------------------------

void RfSocket::writeDatagram(NodeId pDestination, uint8_t* pBuffer, size_t size) {
   /*
   Device::I_DeviceNRf24L01P::Pipe pipe = Device::I_DeviceNRf24L01P::PIPE_0; // TODO (BT) calculate pipe !


   size_t remainingBytesToSend = pBuffer.size();

   while(remainingBytesToSend > 0) {
      mSendFrameBuffer[0] = mNodeId;
      mSendFrameBuffer[1] = pDestination;
      mSendFrameBuffer[2] = pBuffer.size();
      size_t bytesToSend = std::min((size_t)MAX_PAYLOAD, remainingBytesToSend);
      remainingBytesToSend -= bytesToSend;
      mController->send()



   }




   while(bytesToSend > mController->payloadSize()) {
         mController.write()
   }



   mSendFrameBuffer[0] = mNodeId;
   mSendFrameBuffer[1] = pDestination;
   mSendFrameBuffer[2] = pBuffer.size();

   size_t bytesToSend = pBuffer.size();
   while(bytesToSend > mController->payloadSize()) {
      mController.write()
   }

   mController->
   */
}

//-------------------------------------------------------------------------------------------------

size_t RfSocket::receive(Util::Buffer<uint8_t>& pBuffer) {
   return 0;
}

//-------------------------------------------------------------------------------------------------


} // namespace Net
} // namespace Bt
