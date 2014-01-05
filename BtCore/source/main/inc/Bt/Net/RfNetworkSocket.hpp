//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RfNetworkSocket
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_RfNetworkSocket__hpp
#define INC__Bt_Net_RfNetworkSocket__hpp

#include "Bt/Device/I_RfController.hpp"
#include "Bt/Net/RfNode.hpp"
#include "Bt/Net/I_RfNetworkSocket.hpp"
#include "Bt/Net/RfNetworkRoutingAlgorithm.hpp"

namespace Bt {
namespace Net {

class RfNetworkSocket : public I_RfNetworkSocket
{
   private:
      enum { FRAME_BUFFER_SIZE = Device::I_RfController::MAX_PAYLOAD_SIZE };
      enum { HEADER_SIZE = 2 };

   public:
      RfNetworkSocket(RfNode pNodeId, Device::I_RfController& pController);

      virtual bool startListening(I_Listener& pListener);
      virtual bool stopListening();

      virtual bool send(Packet& pPacket);

      virtual void workcycle();

   private:


   	  // Constructor to prohibit copy construction.
      RfNetworkSocket(const RfNetworkSocket&);

      // Operator= to prohibit copy assignment
      RfNetworkSocket& operator=(const RfNetworkSocket&);

      bool sendInternal(Packet& pPacket);
      void receiveInternal(Packet& pPacket);


      RfNode mNodeId;
      Device::I_RfController* mController;
      RfNetworkRoutingAlgorithm mRouting;
      I_Listener* mListener;

};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_RfNetworkSocket__hpp
