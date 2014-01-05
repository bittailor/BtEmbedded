//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::I_RfTransportSocket
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_I_RfTransportSocket__hpp
#define INC__Bt_Net_I_RfTransportSocket__hpp

namespace Bt {
namespace Net {

class I_RfTransportSocket {
   public:
      virtual ~I_RfTransportSocket() {}
      
      virtual void function() = 0;
};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_I_RfTransportSocket__hpp
