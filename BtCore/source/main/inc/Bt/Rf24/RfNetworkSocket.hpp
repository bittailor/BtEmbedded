//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfNetworkSocket
//  
//*************************************************************************************************

#ifndef INC__Bt_Rf24_RfNetworkSocket__hpp
#define INC__Bt_Rf24_RfNetworkSocket__hpp

#include "Bt/Rf24/I_Rf24Controller.hpp"
#include "Bt/Rf24/RfNode.hpp"
#include "Bt/Rf24/I_RfNetworkSocket.hpp"
#include "Bt/Rf24/RfNetworkRoutingAlgorithm.hpp"
#include "Bt/Workcycle/I_Runnable.hpp"

namespace Bt {
namespace Rf24 {

class RfNetworkSocket : public I_RfNetworkSocket
{
   private:
      enum { FRAME_BUFFER_SIZE = I_Rf24DeviceController::MAX_PAYLOAD_SIZE };
      enum { HEADER_SIZE = 2 };
      enum { PACKAGE_STORAGE_SIZE = 6 };

   public:
      RfNetworkSocket(RfNode iNodeId, I_Rf24DeviceController& iController);

      virtual bool send(Packet& iPacket);

      virtual void startListening(std::function<void(Packet& iPacket)> iCallback);
      virtual void stopListening();

   private:

   	  // Constructor to prohibit copy construction.
      RfNetworkSocket(const RfNetworkSocket&);

      // Operator= to prohibit copy assignment
      RfNetworkSocket& operator=(const RfNetworkSocket&);

      bool sendInternal(Packet& iPacket);
      void receiveInternal(Packet& iPacket);
      void onPacketReceived(RfPipe iPipe, I_Rf24DeviceController::Packet& iPacket);


      RfNode mNodeId;
      I_Rf24DeviceController* mController;
      RfNetworkRoutingAlgorithm mRouting;
      uint8_t mIdCounter;
      std::function<void(Packet& iPacket)> mCallback;

};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfNetworkSocket__hpp
