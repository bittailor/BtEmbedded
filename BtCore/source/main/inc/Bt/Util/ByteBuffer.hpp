//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::ByteBuffer
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_ByteBuffer__hpp
#define INC__Bt_Util_ByteBuffer__hpp

#include <stddef.h>
#include <stdint.h>

namespace Bt {
namespace Util {

class ByteBuffer 
{
   public:
      ByteBuffer(uint8_t* iRaw, size_t iSize);
      ~ByteBuffer();


   
   private:
   	  // Constructor to prohibit copy construction.
      ByteBuffer(const ByteBuffer&);

      // Operator= to prohibit copy assignment
      ByteBuffer& operator=(const ByteBuffer&);

      uint8_t* mRaw;
      size_t mSize;

};

} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_ByteBuffer__hpp
