//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::RfControllerNRf24L01PIntegrationTest
//  
//*************************************************************************************************

#include <chrono>
#include <thread>
#include <algorithm>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


#include "Bt/Device/RfControllerNRf24L01P.hpp"
#include "Bt/Device/NRf24L01P.hpp"
#include "Bt/Mcu/Pin.hpp"
#include "Bt/Mcu/Spi.hpp"
#include "Bt/Util/Delay.hpp"
#include "Bt/Util/CollectionUtility.hpp"

namespace Bt {
namespace Device {


/**
 * This test need to have two NRf24L01P transceivers connected!
 *
 * Raspberry Pi:
 *
 *    Pi                   nRf24 (1)
 *    ------------------------------
 *    4                    VCC
 *    GND                  GND
 *    17                   CE
 *    8                    CSN
 *    11(SCK)              SCK
 *    10(MOSI)             MO
 *    9(MISO)              MI
 *
 *    Pi                   nRf24 (2)
 *    ------------------------------
 *    4                    VCC
 *    GND                  GND
 *    24                   CE
 *    7                    CSN
 *    11(SCK)              SCK
 *    10(MOSI)             MO
 *    9(MISO)              MI
 *
 */
class RfControllerNRf24L01PIntegrationTest : public ::testing::Test {

   protected:
      
      RfControllerNRf24L01PIntegrationTest()
               : mPower(4, Mcu::I_Pin::MODE_OUTPUT)
               , mChipEnable1(17, Mcu::I_Pin::MODE_OUTPUT)
               , mChipEnable2(24, Mcu::I_Pin::MODE_OUTPUT)
               , mChipSelect1(8, Mcu::I_Pin::MODE_OUTPUT)
               , mChipSelect2(7, Mcu::I_Pin::MODE_OUTPUT)
               , mSpi1(Mcu::I_Spi::BIT_ORDER_MSBFIRST, Mcu::I_Spi::MODE_0, Mcu::I_Spi::SPEED_8_MHZ , mChipSelect1)
               , mSpi2(Mcu::I_Spi::BIT_ORDER_MSBFIRST, Mcu::I_Spi::MODE_0, Mcu::I_Spi::SPEED_8_MHZ , mChipSelect2)
               , mNRf24L01P1(powerOn(),mChipEnable1)
               , mNRf24L01P2(mSpi2,mChipEnable2)
               , mRfControllerNRf24L01P1(mNRf24L01P1)
               , mRfControllerNRf24L01P2(mNRf24L01P2)
            {

            }

      virtual void SetUp() {
      }

      Mcu::I_Spi& powerOn() {
         mPower.write(false);
         Util::delayInMilliseconds(1);
         mPower.write(true);
         Util::delayInMilliseconds(200);
         return mSpi1;
      }

      virtual void TearDown() {
         mPower.write(false);
      }

      Mcu::Pin mPower;
      Mcu::Pin mChipEnable1;
      Mcu::Pin mChipEnable2;
      Mcu::Pin mChipSelect1;
      Mcu::Pin mChipSelect2;
      Mcu::Spi mSpi1;
      Mcu::Spi mSpi2;

      NRf24L01P mNRf24L01P1;
      NRf24L01P mNRf24L01P2;

      RfControllerNRf24L01P mRfControllerNRf24L01P1;
      RfControllerNRf24L01P mRfControllerNRf24L01P2;

};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void waitForDataAvailable(RfControllerNRf24L01P& pController) {
   int counter = 0;
   while(!pController.isDataAvailable() && counter < 200) {
      Util::delayInMilliseconds(5);
      counter++;
   }
   ASSERT_TRUE(pController.isDataAvailable());
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

TEST_F(RfControllerNRf24L01PIntegrationTest, send1To2OverPipe0) {
   uint8_t data[NRf24L01P::PAYLOAD_SIZE] = {1,2,3,4,5};


   mRfControllerNRf24L01P2.startListening();
   mRfControllerNRf24L01P1.write(I_DeviceNRf24L01P::PIPE_0, data , Util::sizeOfArray(data));

   waitForDataAvailable(mRfControllerNRf24L01P2);

   uint8_t buffer[NRf24L01P::PAYLOAD_SIZE] = {0};
   size_t readSize = mRfControllerNRf24L01P2.read(buffer,Util::sizeOfArray(data));

   EXPECT_EQ((size_t)NRf24L01P::PAYLOAD_SIZE, readSize);
   ASSERT_THAT(data, testing::ElementsAreArray(buffer));


}

//-------------------------------------------------------------------------------------------------

TEST_F(RfControllerNRf24L01PIntegrationTest, send2To1OverPipe0) {
   uint8_t data[NRf24L01P::PAYLOAD_SIZE] = {1,2,3,4,5};


   mRfControllerNRf24L01P1.startListening();
   mRfControllerNRf24L01P2.write(I_DeviceNRf24L01P::PIPE_0, data , Util::sizeOfArray(data));

   waitForDataAvailable(mRfControllerNRf24L01P1);

   uint8_t buffer[NRf24L01P::PAYLOAD_SIZE] = {0};
   size_t readSize = mRfControllerNRf24L01P1.read(buffer,Util::sizeOfArray(data));

   EXPECT_EQ((size_t)NRf24L01P::PAYLOAD_SIZE, readSize);
   ASSERT_THAT(data, testing::ElementsAreArray(buffer));


}

//-------------------------------------------------------------------------------------------------

TEST_F(RfControllerNRf24L01PIntegrationTest, sendAndReveive) {
   uint8_t data[NRf24L01P::PAYLOAD_SIZE];

   mRfControllerNRf24L01P2.startListening();
   mRfControllerNRf24L01P1.write(I_DeviceNRf24L01P::PIPE_0, data , Util::sizeOfArray(data));
   mRfControllerNRf24L01P1.startListening();

   {
      waitForDataAvailable(mRfControllerNRf24L01P2);
      uint8_t buffer[NRf24L01P::PAYLOAD_SIZE] = {0};
      mRfControllerNRf24L01P2.read(buffer,Util::sizeOfArray(buffer));
      mRfControllerNRf24L01P2.write(I_DeviceNRf24L01P::PIPE_0,buffer,Util::sizeOfArray(buffer));
   }

   waitForDataAvailable(mRfControllerNRf24L01P1);
   uint8_t readBuffer[NRf24L01P::PAYLOAD_SIZE] = {0};
   size_t readSize = mRfControllerNRf24L01P1.read(readBuffer, Util::sizeOfArray(readBuffer));


   EXPECT_EQ((size_t)NRf24L01P::PAYLOAD_SIZE, readSize);

   ASSERT_THAT(data, testing::ElementsAreArray(readBuffer));
}

//-------------------------------------------------------------------------------------------------


TEST_F(RfControllerNRf24L01PIntegrationTest, sendAndReveiveInALoop) {
   uint8_t data[NRf24L01P::PAYLOAD_SIZE];
   std::iota(std::begin(data),std::end(data),0);

   for (size_t i = 0; i < 10 ; i++) {

      std::cout << "Loop " << i << std::endl;
      mRfControllerNRf24L01P2.startListening();

      mRfControllerNRf24L01P1.write(I_DeviceNRf24L01P::PIPE_0, data , Util::sizeOfArray(data));
      mRfControllerNRf24L01P1.startListening();

      {
         waitForDataAvailable(mRfControllerNRf24L01P2);
         uint8_t buffer[NRf24L01P::PAYLOAD_SIZE] = {0};
         mRfControllerNRf24L01P2.read(buffer,Util::sizeOfArray(buffer));
         mRfControllerNRf24L01P2.write(I_DeviceNRf24L01P::PIPE_0,buffer,Util::sizeOfArray(buffer));
      }

      waitForDataAvailable(mRfControllerNRf24L01P1);
      uint8_t readBuffer[NRf24L01P::PAYLOAD_SIZE] = {0};
      size_t readSize = mRfControllerNRf24L01P1.read(readBuffer, Util::sizeOfArray(readBuffer));


      EXPECT_EQ((size_t)NRf24L01P::PAYLOAD_SIZE, readSize);
      ASSERT_THAT(data, testing::ElementsAreArray(readBuffer));
   }
}

//-------------------------------------------------------------------------------------------------

} // namespace Device
} // namespace Bt
