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

#include "Bt/Device/I_Rf24Device.hpp"
#include "Bt/Net/RfSocket.hpp"

namespace Bt {
namespace Net {

namespace {

   class RfSocketHeader {
      public:
         RfSocketHeader(uint8_t* pBuffer) : mBuffer(pBuffer) {

         }

         RfSocket::NodeId source() {
            return mBuffer[0];
         }

         void source(RfSocket::NodeId source) {
            mBuffer[0] = source;
         }

         RfSocket::NodeId destination() {
            return mBuffer[1];
         }

         void destination(RfSocket::NodeId destination) {
            mBuffer[1] = destination;
         }

         uint8_t sequenceId() {
            return mBuffer[2];
         }

         void sequenceId(uint8_t sequenceId) {
            mBuffer[2] = sequenceId;
         }

         bool last() {
            return mBuffer[3] & 0x20;
         }

         void last(bool last) {
            mBuffer[3] = last;
         }





      private:
         uint8_t* mBuffer;
   };

}

//-------------------------------------------------------------------------------------------------

RfSocket::RfSocket(NodeId pNodeId, Rf24::I_RfController& pController) : mNodeId(pNodeId), mController(&pController) {

}

//-------------------------------------------------------------------------------------------------

RfSocket::~RfSocket() {

}

//-------------------------------------------------------------------------------------------------

void RfSocket::writeDatagram(NodeId pDestination, uint8_t* pBuffer, size_t size) {

   //Device::I_Rf24Device::Pipe pipe = calculatePipe();


   //size_t remainingBytesToSend = size;

   /*
   while(remainingBytesToSend > 0) {
      mSendFrameBuffer[0] = mNodeId;
      mSendFrameBuffer[1] = pDestination;
      mSendFrameBuffer[2] = pBuffer.size();
      mSendFrameBuffer[2] = pBuffer.size();
      size_t bytesToSend = std::min((size_t)MAX_PAYLOAD, remainingBytesToSend);
      remainingBytesToSend -= bytesToSend;
      mController->send();



   }




   while(bytesToSend > mController->payloadSize()) {
         mController.write()
   }



   mSendFrameBuffer[0] = mNodeId;
   mSendFrameBuffer[1] = pDestination;
   mSendFrameBuffer[2] = pBuffer.size();

   size_t bytesToSend = pBuffer.size();
   while(bytesToSend > mController->payloadSize()) {
      mController.write();
   }
   */

   // mController->

}

//-------------------------------------------------------------------------------------------------

size_t RfSocket::receive(Util::Buffer<uint8_t>& pBuffer) {
   return 0;
}

//-------------------------------------------------------------------------------------------------


} // namespace Net
} // namespace Bt
