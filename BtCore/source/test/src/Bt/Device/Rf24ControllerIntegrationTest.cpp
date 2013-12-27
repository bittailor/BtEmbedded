//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::Rf24ControllerIntegrationTest
//  
//*************************************************************************************************

#include <chrono>
#include <thread>
#include <algorithm>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


#include "Bt/Device/Rf24Controller.hpp"
#include "Bt/Device/Rf24Device.hpp"
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
class Rf24ControllerIntegrationTest : public ::testing::Test {

   protected:
      
      Rf24ControllerIntegrationTest()
               : mPower(4, Mcu::I_Pin::MODE_OUTPUT)
               , mChipEnable1(17, Mcu::I_Pin::MODE_OUTPUT)
               , mChipEnable2(24, Mcu::I_Pin::MODE_OUTPUT)
               , mChipSelect1(8, Mcu::I_Pin::MODE_OUTPUT)
               , mChipSelect2(7, Mcu::I_Pin::MODE_OUTPUT)
               , mSpi1(Mcu::I_Spi::BIT_ORDER_MSBFIRST, Mcu::I_Spi::MODE_0, Mcu::I_Spi::SPEED_8_MHZ , mChipSelect1)
               , mSpi2(Mcu::I_Spi::BIT_ORDER_MSBFIRST, Mcu::I_Spi::MODE_0, Mcu::I_Spi::SPEED_8_MHZ , mChipSelect2)
               , mNRf24L01P1(powerOn(),mChipEnable1)
               , mNRf24L01P2(mSpi2,mChipEnable2)
               , mController1(mNRf24L01P1)
               , mController2(mNRf24L01P2)
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

      Rf24Device mNRf24L01P1;
      Rf24Device mNRf24L01P2;

      Rf24Controller mController1;
      Rf24Controller mController2;

};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

void waitForDataAvailable(Rf24Controller& pController) {
   int counter = 0;
   while(!pController.isDataAvailable() && counter < 200) {
      Util::delayInMilliseconds(5);
      counter++;
   }
   ASSERT_TRUE(pController.isDataAvailable());
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

TEST_F(Rf24ControllerIntegrationTest, send1To2OverPipe0) {
   uint8_t data[Rf24Device::PAYLOAD_SIZE] = {1,2,3,4,5};


   mController2.startListening();
   mController1.write(I_Rf24Device::PIPE_0, data , Util::sizeOfArray(data));

   waitForDataAvailable(mController2);

   uint8_t buffer[Rf24Device::PAYLOAD_SIZE] = {0};
   size_t readSize = mController2.read(buffer,Util::sizeOfArray(data));

   EXPECT_EQ((size_t)Rf24Device::PAYLOAD_SIZE, readSize);
   ASSERT_THAT(data, testing::ElementsAreArray(buffer));


}

//-------------------------------------------------------------------------------------------------

TEST_F(Rf24ControllerIntegrationTest, send2To1OverPipe0) {
   uint8_t data[Rf24Device::PAYLOAD_SIZE] = {1,2,3,4,5};


   mController1.startListening();
   mController2.write(I_Rf24Device::PIPE_0, data , Util::sizeOfArray(data));

   waitForDataAvailable(mController1);

   uint8_t buffer[Rf24Device::PAYLOAD_SIZE] = {0};
   size_t readSize = mController1.read(buffer,Util::sizeOfArray(data));

   EXPECT_EQ((size_t)Rf24Device::PAYLOAD_SIZE, readSize);
   ASSERT_THAT(data, testing::ElementsAreArray(buffer));


}

//-------------------------------------------------------------------------------------------------

TEST_F(Rf24ControllerIntegrationTest, sendAndReveive) {
   uint8_t data[Rf24Device::PAYLOAD_SIZE];

   mController2.startListening();
   mController1.write(I_Rf24Device::PIPE_0, data , Util::sizeOfArray(data));
   mController1.startListening();

   {
      waitForDataAvailable(mController2);
      uint8_t buffer[Rf24Device::PAYLOAD_SIZE] = {0};
      mController2.read(buffer,Util::sizeOfArray(buffer));
      mController2.write(I_Rf24Device::PIPE_0,buffer,Util::sizeOfArray(buffer));
   }

   waitForDataAvailable(mController1);
   uint8_t readBuffer[Rf24Device::PAYLOAD_SIZE] = {0};
   size_t readSize = mController1.read(readBuffer, Util::sizeOfArray(readBuffer));


   EXPECT_EQ((size_t)Rf24Device::PAYLOAD_SIZE, readSize);

   ASSERT_THAT(data, testing::ElementsAreArray(readBuffer));
}

//-------------------------------------------------------------------------------------------------


TEST_F(Rf24ControllerIntegrationTest, sendAndReveiveInALoop) {
   uint8_t data[Rf24Device::PAYLOAD_SIZE];
   std::iota(std::begin(data),std::end(data),0);

   for (size_t i = 0; i < 10 ; i++) {

      std::cout << "Loop " << i << std::endl;
      mController2.startListening();

      mController1.write(I_Rf24Device::PIPE_0, data , Util::sizeOfArray(data));
      mController1.startListening();

      {
         waitForDataAvailable(mController2);
         uint8_t buffer[Rf24Device::PAYLOAD_SIZE] = {0};
         mController2.read(buffer,Util::sizeOfArray(buffer));
         mController2.write(I_Rf24Device::PIPE_0,buffer,Util::sizeOfArray(buffer));
      }

      waitForDataAvailable(mController1);
      uint8_t readBuffer[Rf24Device::PAYLOAD_SIZE] = {0};
      size_t readSize = mController1.read(readBuffer, Util::sizeOfArray(readBuffer));


      EXPECT_EQ((size_t)Rf24Device::PAYLOAD_SIZE, readSize);
      ASSERT_THAT(data, testing::ElementsAreArray(readBuffer));
   }
}

//-------------------------------------------------------------------------------------------------

} // namespace Device
} // namespace Bt
