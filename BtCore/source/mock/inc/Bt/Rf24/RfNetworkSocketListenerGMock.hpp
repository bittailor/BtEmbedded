//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfNetworkSocketListenerGMock
//  
//*************************************************************************************************

#ifndef INC__Bt_Rf24_RfNetworkSocketListenerGMock__hpp
#define INC__Bt_Rf24_RfNetworkSocketListenerGMock__hpp

#include <gmock/gmock.h>

#include "Bt/Rf24/I_RfNetworkSocket.hpp"

namespace Bt {
namespace Rf24 {

class RfNetworkSocketListenerGMock : public I_RfNetworkSocket::I_Listener {
   public:
      
      MOCK_METHOD1(packetReceived, void(I_RfNetworkSocket::Packet&));
};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfNetworkSocket::I_ListenerGMock__hpp
