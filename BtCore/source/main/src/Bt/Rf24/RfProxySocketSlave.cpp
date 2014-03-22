//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfProxySocketSlave
//  
//*************************************************************************************************

#include "Bt/Rf24/RfProxySocketSlave.hpp"
#include "Bt/Rf24/RfProxySocketCommand.hpp"

namespace Bt {
namespace Rf24 {


//-------------------------------------------------------------------------------------------------

RfProxySocketSlave::RfProxySocketSlave(I_Rf24DeviceController& pController) : mController(&pController) {

}

//-------------------------------------------------------------------------------------------------

RfProxySocketSlave::~RfProxySocketSlave() {

}

//-------------------------------------------------------------------------------------------------

bool RfProxySocketSlave::connect(char* pHost, int16_t pPort) {
   //I_Rf24Controller::Packet packet;
   //RfProxySocketConnect* connect = static_cast<RfProxySocketConnect*>(packet.rawBuffer());
   //connect->initialize(connect);





   //mController


   return true;
}

//-------------------------------------------------------------------------------------------------

} // namespace Rf24
} // namespace Bt
