//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RfSocket
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_RfSocket__hpp
#define INC__Bt_Net_RfSocket__hpp

#include <stdint.h>

#include "Bt/Util/Buffer.hpp"
#include <Bt/Device/I_RfController.hpp>


namespace Bt {
namespace Net {

class RfSocket 
{
   private:
      enum { FRAME_BUFFER_SIZE = Device::I_RfController::MAX_PAYLOAD_SIZE };
      enum { HEADER_SIZE = 2 };

   public:
      enum { MAX_PAYLOAD = FRAME_BUFFER_SIZE - HEADER_SIZE};

      typedef uint8_t NodeId;

      RfSocket(NodeId pNodeId, Device::I_RfController& pController);
      ~RfSocket();
   
      void startListening();
      void stopListening();

      void writeDatagram(NodeId pDestination, uint8_t* pBuffer, size_t size);
      size_t receive(Util::Buffer<uint8_t>& pBuffer);

   private:

   	  // Constructor to prohibit copy construction.
      RfSocket(const RfSocket&);

      // Operator= to prohibit copy assignment
      RfSocket& operator=(const RfSocket&);



      NodeId mNodeId;
      Util::StaticArray<uint8_t,Device::I_RfController::MAX_PAYLOAD_SIZE> mSendFrameBuffer;
      Device::I_RfController* mController;

};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_RfSocket__hpp
