//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RmiProxyStreamSocket
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_RmiProxyStreamSocket__hpp
#define INC__Bt_Net_RmiProxyStreamSocket__hpp

#include "Bt/Net/I_StreamSocket.hpp"
#include "Bt/Rmi/I_RemoteClient.hpp"

namespace Bt {
namespace Net {

class RmiProxyStreamSocket : I_StreamSocket
{
   public:
      RmiProxyStreamSocket(Rmi::I_RemoteClient& pRemoteClient);
      ~RmiProxyStreamSocket();

      virtual bool connect(const char* pHost, uint16_t pPort);
      virtual void close();

      virtual size_t write(const uint8_t* pData, size_t pSize);
      virtual void flush();

      virtual size_t available();
      virtual size_t read(uint8_t* pData, size_t pMaxSize);

   private:
   	  // Constructor to prohibit copy construction.
      RmiProxyStreamSocket(const RmiProxyStreamSocket&);

      // Operator= to prohibit copy assignment
      RmiProxyStreamSocket& operator=(const RmiProxyStreamSocket&);

      Rmi::I_RemoteClient* mRemoteClient;
};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_RmiProxyStreamSocket__hpp
