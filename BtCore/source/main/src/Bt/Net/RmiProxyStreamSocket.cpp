//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RfProxyStreamSocket
//  
//*************************************************************************************************

#include "Bt/Net/RmiProxyStreamSocket.hpp"

#include <algorithm>

#include "Bt/Util/Logging.hpp"

namespace Bt {
namespace Net {

enum class StreamSocketRmiCommand : uint8_t {
      CONNECT,
      CLOSE,
      WRITE,
      FLUSH,
      AVAILABLE,
      READ

};

//-------------------------------------------------------------------------------------------------

RmiProxyStreamSocket::RmiProxyStreamSocket(Rmi::I_RemoteClient& pRemoteClient) : mRemoteClient(&pRemoteClient) {

}

//-------------------------------------------------------------------------------------------------

RmiProxyStreamSocket::~RmiProxyStreamSocket() {

}

//-------------------------------------------------------------------------------------------------

bool RmiProxyStreamSocket::connect(const char* pHost, uint16_t pPort) {
   mRemoteClient->out() << static_cast<uint8_t>(StreamSocketRmiCommand::CONNECT)
                        << pPort
                        << pHost;
   mRemoteClient->sendQueryRequest();
   return mRemoteClient->in().read<bool>();
}

//-------------------------------------------------------------------------------------------------

void RmiProxyStreamSocket::close() {
   mRemoteClient->out() << static_cast<uint8_t>(StreamSocketRmiCommand::CLOSE);
   mRemoteClient->sendActionRequest();
}

//-------------------------------------------------------------------------------------------------

size_t RmiProxyStreamSocket::write(const uint8_t* pData, size_t pSize) {
   if(0 == pSize){
      return 0;
   }

   size_t maxPayload = mRemoteClient->out().capacity() - sizeof(StreamSocketRmiCommand);
   const uint8_t* data = pData;
   size_t remaining = pSize;
   size_t send = std::min(remaining,maxPayload);

   while(send>0) {
      mRemoteClient->out() << static_cast<uint8_t>(StreamSocketRmiCommand::WRITE);
      mRemoteClient->out().write(data, send);
      mRemoteClient->sendQueryRequest();
      size_t written = mRemoteClient->in().read<uint32_t>();

      if(written <= 0) {
         return written;
      }
      remaining = remaining - written;
      data = data + written;
      send = std::min(remaining,maxPayload);
   }
   return pSize;

   return 0;
}

//-------------------------------------------------------------------------------------------------

void RmiProxyStreamSocket::flush() {
   mRemoteClient->out() << static_cast<uint8_t>(StreamSocketRmiCommand::FLUSH);
   mRemoteClient->sendActionRequest();
}

//-------------------------------------------------------------------------------------------------

size_t RmiProxyStreamSocket::available() {
  mRemoteClient->out() << static_cast<uint8_t>(StreamSocketRmiCommand::AVAILABLE);
  mRemoteClient->sendQueryRequest();
  return mRemoteClient->in().read<uint32_t>();
}

//-------------------------------------------------------------------------------------------------

size_t RmiProxyStreamSocket::read(uint8_t* pData, size_t pMaxSize) {
   size_t maxPayload = mRemoteClient->out().capacity() - sizeof(StreamSocketRmiCommand);

   size_t maxReadSize = std::min(pMaxSize,maxPayload);
   mRemoteClient->out() << static_cast<uint8_t>(StreamSocketRmiCommand::READ);
   mRemoteClient->out() << static_cast<uint32_t>(maxReadSize);

   mRemoteClient->sendQueryRequest();
   size_t received = mRemoteClient->in().size();

   if (received > maxReadSize)
   {
      BT_UTIL_LOG_ERROR("received " << received << "> maxReadSize" << maxReadSize  );
      received = maxReadSize;
   }

   mRemoteClient->in().read(pData, received);

   return received;
}


//-------------------------------------------------------------------------------------------------

} // namespace Net
} // namespace Bt
