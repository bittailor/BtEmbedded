//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfTransportSocket
//  
//*************************************************************************************************

#ifndef INC__Bt_Rf24_RfTransportSocket__hpp
#define INC__Bt_Rf24_RfTransportSocket__hpp

#include "Bt/Rf24/I_RfNetworkSocket.hpp"

namespace Bt {
namespace Rf24 {

class RfTransportSocket 
{
   public:
      RfTransportSocket(I_RfNetworkSocket& pNetworkSocket);
      ~RfTransportSocket();
   
   private:
   	  // Constructor to prohibit copy construction.
      RfTransportSocket(const RfTransportSocket&);

      // Operator= to prohibit copy assignment
      RfTransportSocket& operator=(const RfTransportSocket&);

      I_RfNetworkSocket* mNetworkSocket;
};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfTransportSocket__hpp
