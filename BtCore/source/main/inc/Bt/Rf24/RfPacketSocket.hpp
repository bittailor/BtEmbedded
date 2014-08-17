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

#include <Bt/Rf24/I_RfPacketSocket.hpp>
#include <Bt/Rf24/I_RfNetworkSocket.hpp>
#include <Bt/Concurrency/BlockingQueue.hpp>
#include <Bt/Concurrency/I_ExecutionContext.hpp>


namespace Bt {
namespace Rf24 {

class RfPacketSocket : public I_RfPacketSocket
{
   public:
      RfPacketSocket(I_RfNetworkSocket& iNetworkSocket, Concurrency::I_ExecutionContext& iExecutionContext);
      ~RfPacketSocket();

      virtual size_t payloadCapacity() const;

      virtual bool hasPendingPacket() const;
      virtual size_t pendingPacketSize() const;

      virtual int32_t send(const uint8_t* iPayload, size_t iSize, uint8_t iNodeId);
      virtual int32_t receive(uint8_t* oPayload, size_t iMaxSize, uint8_t* oNodeId);

      virtual void close();

   private:
   	  // Constructor to prohibit copy construction.
      RfPacketSocket(const RfPacketSocket&);

      // Operator= to prohibit copy assignment
      RfPacketSocket& operator=(const RfPacketSocket&);

      void onPacketReceived(I_RfNetworkSocket::Packet& iPacket);

      typedef std::pair<bool,I_RfNetworkSocket::Packet> Item;

      I_RfNetworkSocket& mNetworkSocket;
      Concurrency::I_ExecutionContext& mExecutionContext;
      Concurrency::BlockingQueue<Item> mReceivedQueue;

};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfPacketSocket__hpp
