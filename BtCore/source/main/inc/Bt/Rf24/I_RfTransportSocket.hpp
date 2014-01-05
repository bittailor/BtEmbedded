//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::I_RfTransportSocket
//  
//*************************************************************************************************

#ifndef INC__Bt_Rf24_I_RfTransportSocket__hpp
#define INC__Bt_Rf24_I_RfTransportSocket__hpp

namespace Bt {
namespace Rf24 {

class I_RfTransportSocket {
   public:
      virtual ~I_RfTransportSocket() {}
      
      virtual void function() = 0;
};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_I_RfTransportSocket__hpp
