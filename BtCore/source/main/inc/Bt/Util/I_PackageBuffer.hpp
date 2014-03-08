//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Com::I_PackageBuffer
//  
//*************************************************************************************************

#ifndef INC__Bt_Com_I_PackageBuffer__hpp
#define INC__Bt_Com_I_PackageBuffer__hpp

#include <stddef.h>
#include <stdint.h>

namespace Bt {
namespace Com {

class I_PackageBuffer {
   public:
      virtual ~I_PackageBuffer() {}
      
      virtual size_t put(uint8_t iValue) = 0;
      virtual int16_t get() = 0;
      virtual size_t length() const = 0;

      virtual void clear() = 0;
      virtual uint8_t* raw() = 0;
      virtual void filled(size_t iLength) = 0;

};

} // namespace Com
} // namespace Bt

#endif // INC__Bt_Com_I_PackageBuffer__hpp
