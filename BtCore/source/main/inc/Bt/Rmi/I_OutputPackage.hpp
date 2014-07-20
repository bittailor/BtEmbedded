//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rmi::I_OutputPackage
//  
//*************************************************************************************************

#ifndef INC__Bt_Rmi_I_OutputPackage__hpp
#define INC__Bt_Rmi_I_OutputPackage__hpp

namespace Bt {
namespace Rmi {

class I_OutputPackage {
   public:
   public:
        virtual ~I_OutputPackage() {}

        virtual size_t capacity();

        virtual I_OutputPackage& write(bool iValue) = 0;
        virtual I_OutputPackage& write(int8_t iValue) = 0;
        virtual I_OutputPackage& write(uint8_t iValue) = 0;
        virtual I_OutputPackage& write(int16_t iValue) = 0;
        virtual I_OutputPackage& write(uint16_t iValue) = 0;
        virtual I_OutputPackage& write(int32_t iValue) = 0;
        virtual I_OutputPackage& write(uint32_t iValue) = 0;
        virtual I_OutputPackage& write(const char* iValue) = 0;
        virtual I_OutputPackage& write(const uint8_t* iData, size_t iSize) = 0;


        // TODO do it with traits !
        I_OutputPackage& operator<<(bool iValue) {
           write(iValue);
           return *this;
        }

        I_OutputPackage& operator<<(int8_t iValue){
           write(iValue);
           return *this;
        }

        I_OutputPackage& operator<<(uint8_t iValue){
           write(iValue);
           return *this;
        }

        I_OutputPackage& operator<<(int16_t iValue){
           write(iValue);
           return *this;
        }

        I_OutputPackage& operator<<(uint16_t iValue){
           write(iValue);
           return *this;
        }

        I_OutputPackage& operator<<(int32_t iValue){
           write(iValue);
           return *this;
        }

        I_OutputPackage& operator<<(uint32_t iValue){
           write(iValue);
           return *this;
        }

        I_OutputPackage& operator<<(const char* iValue){
           write(iValue);
           return *this;
        }

        template<typename T>
        I_OutputPackage& operator<<(const T& iValue) {
           iValue.serialize(*this);
           return *this;
        }

};

} // namespace Rmi
} // namespace Bt

#endif // INC__Bt_Rmi_I_OutputPackage__hpp
