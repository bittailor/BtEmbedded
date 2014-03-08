//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rmi::I_InputPackage
//  
//*************************************************************************************************

#ifndef INC__Bt_Rmi_I_InputPackage__hpp
#define INC__Bt_Rmi_I_InputPackage__hpp

namespace Bt {
namespace Rmi {

class I_InputPackage {
   public:
         virtual ~I_InputPackage() {}

         virtual size_t size() = 0;

         virtual void readIn(bool& oValue) = 0;
         virtual void readIn(int8_t& oValue) = 0;
         virtual void readIn(uint8_t& oValue) = 0;
         virtual void readIn(uint32_t& oValue) = 0;

         virtual void read(uint8_t* pData, size_t pSize) = 0;

         template<typename T>
         T read() {
            T value;
            readIn(value);
            return value;
         }


         // TODO do it with traits !
         I_InputPackage& operator>>(bool& oValue) {
            readIn(oValue);
            return *this;
         }

         I_InputPackage& operator>>(int8_t& oValue) {
            readIn(oValue);
            return *this;
         }

         I_InputPackage& operator>>(uint8_t& oValue) {
            readIn(oValue);
            return *this;
         }
};

} // namespace Rmi
} // namespace Bt

#endif // INC__Bt_Rmi_I_InputPackage__hpp
