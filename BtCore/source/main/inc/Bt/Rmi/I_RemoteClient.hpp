//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rmi::I_RemoteClient
//  
//*************************************************************************************************

#ifndef INC__Bt_Rmi_I_RemoteClient__hpp
#define INC__Bt_Rmi_I_RemoteClient__hpp

#include <Bt/Rmi/I_InputPackage.hpp>
#include <Bt/Rmi/I_OutputPackage.hpp>

namespace Bt {
namespace Rmi {

class I_RemoteClient {
   public:
      virtual ~I_RemoteClient() {}
      
      virtual I_OutputPackage& out() = 0;
      virtual I_InputPackage& in() = 0;



      /**
       * Sends an action request.
       * An action request is a request without return value.
       */
      virtual void sendActionRequest() = 0;

      /**
       * Sends a function request.
       * An query request is a request with return value.
       */
      virtual void sendQueryRequest() = 0;
};

} // namespace Rmi
} // namespace Bt

#endif // INC__Bt_Rmi_I_RemoteClient__hpp
