//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::RfTransportSocket
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_RfTransportSocket__hpp
#define INC__Bt_Net_RfTransportSocket__hpp

namespace Bt {
namespace Net {

class RfTransportSocket 
{
   public:
      RfTransportSocket();
      ~RfTransportSocket();
   
   private:
   	  // Constructor to prohibit copy construction.
      RfTransportSocket(const RfTransportSocket&);

      // Operator= to prohibit copy assignment
      RfTransportSocket& operator=(const RfTransportSocket&);
};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_RfTransportSocket__hpp