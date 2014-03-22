//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfPacketSocket
//  
//*************************************************************************************************

#ifndef INC__Bt_Rf24_RfPacketSocket__hpp
#define INC__Bt_Rf24_RfPacketSocket__hpp

#include <future>

#include "Bt/Rf24/I_RfPacketSocket.hpp"
#include "Bt/Rf24/I_RfNetworkSocket.hpp"
#include "Bt/Concurrency/BlockingQueue.hpp"


namespace Bt {
namespace Rf24 {

class RfPacketSocket : public I_RfPacketSocket
{
   public:
      RfPacketSocket(I_RfNetworkSocket& pNetworkSocket);
      ~RfPacketSocket();

      virtual size_t payloadCapacity() const;

      virtual bool hasPendingPacket() const;
      virtual size_t pendingPacketSize() const;

      virtual int32_t send(uint8_t* iPayload, size_t iSize, uint8_t iGatewayNodeId);
      virtual int32_t receive(uint8_t* oPayload, size_t iMaxSize, uint8_t* oGatewayNodeId);

      virtual void workcycle();

   private:
   	  // Constructor to prohibit copy construction.
      RfPacketSocket(const RfPacketSocket&);

      // Operator= to prohibit copy assignment
      RfPacketSocket& operator=(const RfPacketSocket&);

      I_RfNetworkSocket& mNetworkSocket;
      Concurrency::BlockingQueue<std::vector<uint8_t>> mReceivedQueue;
      Concurrency::BlockingQueue<std::pair<std::promise<bool>,std::vector<uint8_t>>> mSendQueue;
};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfPacketSocket__hpp
