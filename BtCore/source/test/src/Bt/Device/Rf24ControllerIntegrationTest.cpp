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

//-------------------------------------------------------------------------------------------------

/**
 * These tests need to have two NRf24L01P transceivers connected!
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

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

namespace {

template<typename T> T* asArray(std::vector<T>& pVector) {
   return &pVector[0];
}

void waitForDataAvailable(Rf24Controller& pController) {
   int counter = 0;
   while(!pController.isDataAvailable() && counter < 200) {
      Util::delayInMilliseconds(5);
      counter++;
   }
   ASSERT_TRUE(pController.isDataAvailable());
}

}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

class Rf24ControllerIntegrationTestBase : public ::testing::Test {

   protected:
      
      Rf24ControllerIntegrationTestBase()
               : mPower(4, Mcu::I_Pin::MODE_OUTPUT)
               , mChipEnable1(17, Mcu::I_Pin::MODE_OUTPUT)
               , mChipEnable2(24, Mcu::I_Pin::MODE_OUTPUT)
               , mChipSelect1(8, Mcu::I_Pin::MODE_OUTPUT)
               , mChipSelect2(7, Mcu::I_Pin::MODE_OUTPUT)
               , mSpi1(Mcu::I_Spi::BIT_ORDER_MSBFIRST, Mcu::I_Spi::MODE_0, Mcu::I_Spi::SPEED_8_MHZ , mChipSelect1)
               , mSpi2(Mcu::I_Spi::BIT_ORDER_MSBFIRST, Mcu::I_Spi::MODE_0, Mcu::I_Spi::SPEED_8_MHZ , mChipSelect2)
               , mDevice1(powerOn(),mChipEnable1)
               , mDevice2(mSpi2,mChipEnable2)
               , mController1(mDevice1)
               , mController2(mDevice2)
            {

            }

      virtual void SetUp() {
      }

      Mcu::I_Spi& powerOn() {
         mPower.write(false);
         Util::delayInMilliseconds(1);
         mPower.write(true);
         Util::delayInMilliseconds(1);
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

      Rf24Device mDevice1;
      Rf24Device mDevice2;

      Rf24Controller mController1;
      Rf24Controller mController2;

};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

class Rf24ControllerIntegrationTest : public Rf24ControllerIntegrationTestBase {

};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

class Rf24Controller$Pipe$Size$IntegrationTest
         : public Rf24ControllerIntegrationTestBase
         , public ::testing::WithParamInterface< ::std::tr1::tuple<I_Rf24Device::Pipe, size_t>>{

};

INSTANTIATE_TEST_CASE_P(AllPipes,
                        Rf24Controller$Pipe$Size$IntegrationTest,
                        ::testing::Combine(
                                 ::testing::Values(
                                          I_Rf24Device::Pipe::PIPE_0,
                                          I_Rf24Device::Pipe::PIPE_1,
                                          I_Rf24Device::Pipe::PIPE_2,
                                          I_Rf24Device::Pipe::PIPE_3,
                                          I_Rf24Device::Pipe::PIPE_4,
                                          I_Rf24Device::Pipe::PIPE_5
                                 ),
                                 ::testing::Values(
                                          1u,
                                          2u,
                                          16u,
                                          Rf24Device::MAX_PAYLOAD_SIZE-1u,
                                          Rf24Device::MAX_PAYLOAD_SIZE
                                 )
                        )
);

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24Controller$Pipe$Size$IntegrationTest, sendDevice1ToDevice2) {
   size_t size = ::std::tr1::get<1>(GetParam());
   std::vector<uint8_t> data(size);
   std::iota(data.begin(), data.end(), 1);

   mController2.startListening();
   mController1.write(::std::tr1::get<0>(GetParam()), asArray(data), data.size());

   waitForDataAvailable(mController2);

   I_Rf24Device::Pipe pipe;
   std::vector<uint8_t> buffer(size);
   size_t readSize = mController2.read(asArray(buffer), buffer.size(), pipe);

   EXPECT_EQ(::std::tr1::get<0>(GetParam()), pipe);
   EXPECT_EQ(size, readSize);
   ASSERT_THAT(buffer, testing::ElementsAreArray(asArray(data), data.size()));


}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24Controller$Pipe$Size$IntegrationTest, sendDevice2ToDevice1) {
   size_t size = ::std::tr1::get<1>(GetParam());
   std::vector<uint8_t> data(size);
   std::iota(data.begin(), data.end(), 1);

   mController1.startListening();
   mController2.write(::std::tr1::get<0>(GetParam()), asArray(data), data.size());

   waitForDataAvailable(mController1);

   I_Rf24Device::Pipe pipe;
   std::vector<uint8_t> buffer(size);
   size_t readSize = mController1.read(asArray(buffer), buffer.size(), pipe);

   EXPECT_EQ(::std::tr1::get<0>(GetParam()), pipe);
   EXPECT_EQ(size, readSize);
   ASSERT_THAT(buffer, testing::ElementsAreArray(asArray(data), data.size()));
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24Controller$Pipe$Size$IntegrationTest, sendAndReveive) {
   size_t size = ::std::tr1::get<1>(GetParam());
   std::vector<uint8_t> data(size);
   std::iota(data.begin(), data.end(), 1);

   mController2.startListening();
   mController1.write(::std::tr1::get<0>(GetParam()), asArray(data), data.size());
   mController1.startListening();

   {
      waitForDataAvailable(mController2);
      std::vector<uint8_t> buffer(size);
      mController2.read(asArray(buffer), buffer.size());
      mController2.write(::std::tr1::get<0>(GetParam()), asArray(buffer), buffer.size());
   }

   waitForDataAvailable(mController1);
   std::vector<uint8_t> readBuffer(size);
   size_t readSize = mController1.read(asArray(readBuffer), readBuffer.size());


   EXPECT_EQ(size, readSize);

   ASSERT_THAT(readBuffer, testing::ElementsAreArray(asArray(data), data.size()));
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24Controller$Pipe$Size$IntegrationTest, sendAndReveiveInALoop) {
   size_t size = ::std::tr1::get<1>(GetParam());
   std::vector<uint8_t> data(size);
   std::iota(data.begin(), data.end(), 1);

   for (size_t i = 0; i < 3 ; i++) {

      // std::cout << "Loop " << i << std::endl;
      mController2.startListening();

      mController1.write(::std::tr1::get<0>(GetParam()), asArray(data), data.size());
      mController1.startListening();

      {
         waitForDataAvailable(mController2);
         std::vector<uint8_t> buffer(size);
         mController2.read(asArray(buffer), buffer.size());
         mController2.write(::std::tr1::get<0>(GetParam()), asArray(buffer), buffer.size());
      }

      waitForDataAvailable(mController1);
      std::vector<uint8_t> readBuffer(size);
      size_t readSize = mController1.read(asArray(readBuffer), readBuffer.size());


      EXPECT_EQ(size, readSize);
      ASSERT_THAT(readBuffer, testing::ElementsAreArray(asArray(data), data.size()));
   }
}


//-------------------------------------------------------------------------------------------------

} // namespace Device
} // namespace Bt
