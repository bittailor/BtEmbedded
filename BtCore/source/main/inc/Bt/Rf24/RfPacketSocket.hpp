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
#include <memory>

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

      virtual int32_t send(const uint8_t* iPayload, size_t iSize, uint8_t iNodeId);
      virtual int32_t receive(uint8_t* oPayload, size_t iMaxSize, uint8_t* oNodeId);

      virtual void workcycle();

   private:
   	  // Constructor to prohibit copy construction.
      RfPacketSocket(const RfPacketSocket&);

      // Operator= to prohibit copy assignment
      RfPacketSocket& operator=(const RfPacketSocket&);

      typedef std::pair<std::promise<bool>,std::shared_ptr<I_RfNetworkSocket::Packet>> SendMessage;
      typedef std::shared_ptr<I_RfNetworkSocket::Packet> ReceiveMessage;

      I_RfNetworkSocket& mNetworkSocket;
      Concurrency::BlockingQueue<ReceiveMessage> mReceivedQueue;
      Concurrency::BlockingQueue<SendMessage> mSendQueue;
};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfPacketSocket__hpp
