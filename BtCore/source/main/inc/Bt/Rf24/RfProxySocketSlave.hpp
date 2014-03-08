//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfProxySocketSlave
//  
//*************************************************************************************************

#ifndef INC__Bt_Rf24_RfProxySocketSlave__hpp
#define INC__Bt_Rf24_RfProxySocketSlave__hpp

#include <stdint.h>

#include "Bt/Rf24/I_Rf24Controller.hpp"

namespace Bt {
namespace Rf24 {

class RfProxySocketSlave 
{
   public:
      RfProxySocketSlave(I_Rf24Controller& pController);
      ~RfProxySocketSlave();

      virtual bool connect(char* pHost, int16_t pPort);

   
   private:
   	  // Constructor to prohibit copy construction.
      RfProxySocketSlave(const RfProxySocketSlave&);

      // Operator= to prohibit copy assignment
      RfProxySocketSlave& operator=(const RfProxySocketSlave&);

      I_Rf24Controller* mController;
};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfProxySocketSlave__hpp
