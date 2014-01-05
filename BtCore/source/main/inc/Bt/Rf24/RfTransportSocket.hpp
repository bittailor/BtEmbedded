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

namespace Bt {
namespace Rf24 {

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

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfTransportSocket__hpp