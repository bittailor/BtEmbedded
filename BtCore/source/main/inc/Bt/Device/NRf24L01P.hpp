//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::NRf24L01P
//  
//*************************************************************************************************

#ifndef INC__Bt_Device_NRf24L01P__hpp
#define INC__Bt_Device_NRf24L01P__hpp

#include "Bt/Mcu/I_Spi.hpp"
#include "Bt/Mcu/I_Pin.hpp"
#include "Bt/Util/StaticArray.hpp"

namespace Bt {
namespace Device {

class NRf24L01P 
{
   public:

      enum Pipe {
         PIPE_0,
         PIPE_1,
         PIPE_2,
         PIPE_3,
         PIPE_4,
         PIPE_5,
      };

      class Address {
         public:
            Address(){
            }

            Address(Util::StaticArray<uint8_t,5> pRaw) : mRaw(pRaw){
            }

            Address(uint8_t pByte4, uint8_t pByte3, uint8_t pByte2, uint8_t pByte1, uint8_t pByte0) {
               mRaw[0] = pByte0;
               mRaw[1] = pByte1;
               mRaw[2] = pByte2;
               mRaw[3] = pByte3;
               mRaw[4] = pByte4;
            }

            Util::StaticArray<uint8_t,5>& raw() {
               return mRaw;
            }

         private:
            Util::StaticArray<uint8_t,5> mRaw;
      };

      //typedef Util::StaticArray<uint8_t,5> Address;


      NRf24L01P(Mcu::I_Spi& pSpi, Mcu::I_Pin& pChipEnable);
      ~NRf24L01P();

      uint8_t autoRetransmitDelay();
      void autoRetransmitDelay(uint8_t pDelay);


      uint8_t autoRetransmitCount();
      void autoRetransmitCount(uint8_t pCount);

      uint8_t channel();
      void channel(uint8_t pChannel);

      Address receiveAddress(Pipe pPipe);
      void receiveAddress(Pipe pPipe, Address pAddress);

      Address transmitAddress();
      void transmitAddress(Address pAddress);


   
   private:
   	  // Constructor to prohibit copy construction.
      NRf24L01P(const NRf24L01P&);

      // Operator= to prohibit copy assignment
      NRf24L01P& operator=(const NRf24L01P&);

      Mcu::I_Spi* mSpi;
      Mcu::I_Pin* mChipEnable;

};

} // namespace Device
} // namespace Bt

#endif // INC__Bt_Device_NRf24L01P__hpp
