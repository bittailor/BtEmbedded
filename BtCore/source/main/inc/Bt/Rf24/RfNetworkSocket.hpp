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

class RfNetworkSocket : public I_RfNetworkSocket, public Workcycle::I_Runnable
{
   private:
      enum { FRAME_BUFFER_SIZE = I_Rf24Controller::MAX_PAYLOAD_SIZE };
      enum { HEADER_SIZE = 2 };

   public:
      RfNetworkSocket(RfNode pNodeId, I_Rf24Controller& pController);

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
      I_Rf24Controller* mController;
      RfNetworkRoutingAlgorithm mRouting;
      uint8_t mIdCounter;
      I_Listener* mListener;

};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfNetworkSocket__hpp
