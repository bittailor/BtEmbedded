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

#include <Bt/Rf24/I_RfPacketSocket.hpp>

#include "Bt/Rf24/I_RfNetworkSocket.hpp"

namespace Bt {
namespace Rf24 {

class RfPacketSocket : public I_RfPacketSocket
{
   public:
      RfPacketSocket(I_RfNetworkSocket& pNetworkSocket);
      ~RfPacketSocket();

      virtual bool send(Packet& pPacket);
   
   private:
   	  // Constructor to prohibit copy construction.
      RfPacketSocket(const RfPacketSocket&);

      // Operator= to prohibit copy assignment
      RfPacketSocket& operator=(const RfPacketSocket&);

      I_RfNetworkSocket* mNetworkSocket;
};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfPacketSocket__hpp
