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
#include "Bt/Net/RfNodeId.hpp"
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
      RfNetworkSocket(RfNodeId pNodeId, Device::I_RfController& pController);

      void workcycle();

      void writeDatagram(RfNodeId pDestination, uint8_t* pBuffer, size_t size);





   private:

      class NetworkPacket;

   	  // Constructor to prohibit copy construction.
      RfNetworkSocket(const RfNetworkSocket&);

      // Operator= to prohibit copy assignment
      RfNetworkSocket& operator=(const RfNetworkSocket&);

      size_t write(NetworkPacket& packet);


      RfNodeId mNodeId;
      Device::I_RfController* mController;
      RfNetworkRoutingAlgorithm mRouting;

};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_RfNetworkSocket__hpp
