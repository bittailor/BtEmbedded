//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::NRf24L01PTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Bt/Device/NRf24L01P.hpp"
#include "Bt/Mcu/Pin.hpp"
#include "Bt/Mcu/Spi.hpp"
#include "Bt/Util/Delay.hpp"
#include "Bt/Util/CollectionUtility.hpp"

namespace Bt {
namespace Device {

//-------------------------------------------------------------------------------------------------

struct NRf24L01PTestParameters {
      uint8_t chipSelect;
      uint8_t chipEnable;
};



class NRf24L01PTest : public ::testing::Test, public ::testing::WithParamInterface<NRf24L01PTestParameters> {
   
   protected:
      
      NRf24L01PTest()
         : mPower(4, Mcu::I_Pin::MODE_OUTPUT)
         , mChipSelect(GetParam().chipSelect, Mcu::I_Pin::MODE_OUTPUT)
         , mSpi(Mcu::I_Spi::BIT_ORDER_MSBFIRST, Mcu::I_Spi::MODE_0, Mcu::I_Spi::SPEED_8_MHZ , mChipSelect)
         , mChipEnable(GetParam().chipEnable, Mcu::I_Pin::MODE_OUTPUT)
         , mNRf24L01P(powerOn(),mChipEnable) {

      }

      virtual void SetUp() {
      }

      Mcu::I_Spi& powerOn() {
         mPower.write(false);
         Util::delayInMilliseconds(1);
         mPower.write(true);
         Util::delayInMilliseconds(1);
         return mSpi;
      }

      virtual void TearDown() {
         mPower.write(false);
      }




      Mcu::Pin mPower;
      Mcu::Pin mChipSelect;
      Mcu::Spi mSpi;
      Mcu::Pin mChipEnable;

      NRf24L01P mNRf24L01P;

};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

INSTANTIATE_TEST_CASE_P(Default, NRf24L01PTest,::testing::Values(
         NRf24L01PTestParameters{8,17},
         NRf24L01PTestParameters{7,24}
));

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

template<typename T>
void printArray(const T& pArray)
{
  printf(" 0x");
  for(std::size_t i = pArray.size() ; i > 0 ; i--)
  {
    printf("%02x",pArray[i-1]);
  }
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

TEST_P(NRf24L01PTest, readDefaultChannel) {
   EXPECT_EQ(0x2,(int)mNRf24L01P.channel());

}

//-------------------------------------------------------------------------------------------------

TEST_P(NRf24L01PTest, writeAndReadBackChannel) {
   uint8_t rf = 0x4c;
   mNRf24L01P.channel(rf);
   EXPECT_EQ((int)rf,(int)mNRf24L01P.channel());
}


//-------------------------------------------------------------------------------------------------

TEST_P(NRf24L01PTest, readDefaultAutoRetransmitCount) {
   EXPECT_EQ(0x03,(int)mNRf24L01P.autoRetransmitCount());
}

//-------------------------------------------------------------------------------------------------

TEST_P(NRf24L01PTest, writeAndReadBackAutoRetransmitCount) {
   uint8_t count = 0x0a;
   mNRf24L01P.autoRetransmitCount(count);
   EXPECT_EQ((int)count,(int)mNRf24L01P.autoRetransmitCount());
}

//-------------------------------------------------------------------------------------------------

TEST_P(NRf24L01PTest, readDefaultAutoRetransmitDelay) {
   EXPECT_EQ(0x00,(int)mNRf24L01P.autoRetransmitDelay());

}

//-------------------------------------------------------------------------------------------------

TEST_P(NRf24L01PTest, writeAndReadBackAutoRetransmitDelay) {
   uint8_t delay = 0x04;
   mNRf24L01P.autoRetransmitDelay(delay);
   EXPECT_EQ((int)delay,(int)mNRf24L01P.autoRetransmitDelay());
}

//-------------------------------------------------------------------------------------------------

TEST_P(NRf24L01PTest, writeSameRegister) {
   uint8_t delay = 0x04;
   uint8_t count = 0x0a;
   mNRf24L01P.autoRetransmitDelay(delay);
   EXPECT_EQ((int)delay,(int)mNRf24L01P.autoRetransmitDelay());
   EXPECT_EQ(0x03,(int)mNRf24L01P.autoRetransmitCount());
   mNRf24L01P.autoRetransmitCount(count);
   EXPECT_EQ((int)delay,(int)mNRf24L01P.autoRetransmitDelay());
   EXPECT_EQ((int)count,(int)mNRf24L01P.autoRetransmitCount());
}


//-------------------------------------------------------------------------------------------------

TEST_P(NRf24L01PTest, readDefaultRxPipes) {

   NRf24L01P::Pipe pipes[] = {
            NRf24L01P::PIPE_0,
            NRf24L01P::PIPE_1,
            NRf24L01P::PIPE_2,
            NRf24L01P::PIPE_3,
            NRf24L01P::PIPE_4,
            NRf24L01P::PIPE_5,
   };

   NRf24L01P::Address defaultAddresses[] = {
            {0xE7, 0xE7, 0xE7, 0xE7, 0xE7},
            {0xC2, 0xC2, 0xC2, 0xC2, 0xC2},
            {0xC2, 0xC2, 0xC2, 0xC2, 0xC3},
            {0xC2, 0xC2, 0xC2, 0xC2, 0xC4},
            {0xC2, 0xC2, 0xC2, 0xC2, 0xC5},
            {0xC2, 0xC2, 0xC2, 0xC2, 0xC6},
   };

   for (std::size_t i = 0 ; i < Util::sizeOfArray(pipes) ; i++) {
      NRf24L01P::Address address = mNRf24L01P.receiveAddress(pipes[i]).raw();
      ASSERT_THAT(address.raw(), testing::ElementsAreArray(defaultAddresses[i].raw().begin(),defaultAddresses[i].raw().size()));
   }
}

//-------------------------------------------------------------------------------------------------

TEST_P(NRf24L01PTest, writeAndReadBackRxPipes) {

   NRf24L01P::Pipe pipes[] = {
            NRf24L01P::PIPE_0,
            NRf24L01P::PIPE_1,
            NRf24L01P::PIPE_2,
            NRf24L01P::PIPE_3,
            NRf24L01P::PIPE_4,
            NRf24L01P::PIPE_5
   };

   NRf24L01P::Address addresses[] = {
            {0x22, 0x23, 0x24, 0x25, 0x26},
            {0xA1, 0xB1, 0xC1, 0xD1, 0xE1},
            {0xA1, 0xB1, 0xC1, 0xD1, 0xE2},
            {0xA1, 0xB1, 0xC1, 0xD1, 0xE3},
            {0xA1, 0xB1, 0xC1, 0xD1, 0xE4},
            {0xA1, 0xB1, 0xC1, 0xD1, 0xE5}
   };

   for (std::size_t i = 0 ; i < Util::sizeOfArray(pipes) ; i++) {
      mNRf24L01P.receiveAddress(pipes[i], addresses[i]);
   }

   for (std::size_t i = 0 ; i < Util::sizeOfArray(pipes) ; i++) {
      NRf24L01P::Address address = mNRf24L01P.receiveAddress(pipes[i]).raw();
      ASSERT_THAT(address.raw(), testing::ElementsAreArray(addresses[i].raw().begin(),addresses[i].raw().size()));
   }

}

//-------------------------------------------------------------------------------------------------

TEST_P(NRf24L01PTest, readDefaultTx) {
   NRf24L01P::Address defaultAddress = NRf24L01P::Address(0xE7,0xE7,0xE7,0xE7,0xE7);

   NRf24L01P::Address address = mNRf24L01P.transmitAddress();
   ASSERT_THAT(defaultAddress.raw(), testing::ElementsAreArray(address.raw().begin(),address.raw().size()));
}

//-------------------------------------------------------------------------------------------------

TEST_P(NRf24L01PTest, writeAndReadBackTx) {
   NRf24L01P::Address newAddress = NRf24L01P::Address(0xE7,0xE7,0xE7,0xE7,0xE7);
   mNRf24L01P.transmitAddress(newAddress);

   NRf24L01P::Address address = mNRf24L01P.transmitAddress();
   ASSERT_THAT(newAddress.raw(), testing::ElementsAreArray(address.raw().begin(),address.raw().size()));
 }

//-------------------------------------------------------------------------------------------------


} // namespace Device
} // namespace Bt
