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
      enum { FRAME_BUFFER_SIZE = I_Rf24DeviceController::MAX_PAYLOAD_SIZE };
      enum { HEADER_SIZE = 2 };
      enum { PACKAGE_STORAGE_SIZE = 6 };

   public:
      RfNetworkSocket(RfNode pNodeId, I_Rf24DeviceController& pController);

      virtual bool send(Packet& pPacket);
      virtual bool receive(Packet& pPacket);
      virtual bool available();

      virtual bool setListener(I_Listener& iListener);
      virtual bool resetListener();

      virtual void workcycle();

   private:
      class StoredPackage : public Packet, public Util::IntrusiveList<StoredPackage>::Element {
      };


   	  // Constructor to prohibit copy construction.
      RfNetworkSocket(const RfNetworkSocket&);

      // Operator= to prohibit copy assignment
      RfNetworkSocket& operator=(const RfNetworkSocket&);

      bool sendInternal(Packet& pPacket);
      void receiveInternal(Packet& pPacket);


      RfNode mNodeId;
      I_Rf24DeviceController* mController;
      RfNetworkRoutingAlgorithm mRouting;
      uint8_t mIdCounter;
      I_Listener* mListener;
      StoredPackage mPackages[PACKAGE_STORAGE_SIZE];
      Util::IntrusiveList<StoredPackage> mFree;
      Util::IntrusiveList<StoredPackage> mReceived;


};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfNetworkSocket__hpp
