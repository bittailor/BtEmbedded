//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::I_StreamSocket
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_I_StreamSocket__hpp
#define INC__Bt_Net_I_StreamSocket__hpp

#include "Bt/Util/ByteBuffer.hpp"


namespace Bt {
namespace Net {

class I_StreamSocket {
   public:
      virtual ~I_StreamSocket() {}
      
      virtual bool connect(const char* iHost, uint16_t iPort) = 0;
      virtual void close() =0;

      virtual size_t write(const uint8_t* iData, size_t iSize) = 0;
      virtual void flush() = 0;

      virtual size_t available() = 0;
      virtual size_t read(uint8_t* oData, size_t iMaxSize) = 0;


};

} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_I_StreamSocket__hpp
