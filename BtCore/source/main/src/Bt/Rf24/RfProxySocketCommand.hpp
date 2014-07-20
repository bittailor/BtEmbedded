//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfProxySocketCommand
//  
//*************************************************************************************************

#ifndef INC__Bt_Rf24_RfProxySocketCommand__hpp
#define INC__Bt_Rf24_RfProxySocketCommand__hpp

#include <stdint.h>

namespace Bt {
namespace Rf24 {

enum class RfProxySocketCommand : uint8_t {
      CONNECT
};

struct RfProxySocketHeader {
      RfProxySocketCommand mCommand;
};

struct RfProxySocketConnect {
      RfProxySocketHeader mHeader;
      int16_t mPort;
      char mHost[0];

      void initialize(char* iHost, int16_t iPort) {
         mHeader.mCommand = RfProxySocketCommand::CONNECT;
         //mPort
      }
};





} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfProxySocketCommand__hpp
