//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::Rf24ControllerIntegrationTest
//  
//*************************************************************************************************

#include <chrono>
#include <thread>
#include <algorithm>
#include <numeric>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


#include "Bt/Rf24/Rf24Controller.hpp"
#include "Bt/Rf24/Rf24Device.hpp"
#include "Bt/Mcu/Pin.hpp"
#include "Bt/Mcu/Spi.hpp"
#include "Bt/Util/Timing.hpp"
#include "Bt/Util/CollectionUtility.hpp"

namespace Bt {
namespace Rf24 {
//
////-------------------------------------------------------------------------------------------------
//
///**
// * These tests need to have two NRf24L01P transceivers connected!
// *
// * Raspberry Pi:
// *
// *    Pi                   nRf24 (1)
// *    ------------------------------
// *    4                    VCC
// *    GND                  GND
// *    17                   CE
// *    8                    CSN
// *    11(SCK)              SCK
// *    10(MOSI)             MO
// *    9(MISO)              MI
// *
// *    Pi                   nRf24 (2)
// *    ------------------------------
// *    4                    VCC
// *    GND                  GND
// *    24                   CE
// *    7                    CSN
// *    11(SCK)              SCK
// *    10(MOSI)             MO
// *    9(MISO)              MI
// *
// */
//
////-------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------
//
//namespace {
//
//template<typename T> T* asArray(std::vector<T>& iVector) {
//   return &iVector[0];
//}
//
//void waitForDataAvailable(Rf24DeviceController& iController) {
//   int counter = 0;
//   while(!iController.isDataAvailable() && counter < 200) {
//      Util::delayInMilliseconds(5);
//      counter++;
//   }
//   ASSERT_TRUE(iController.isDataAvailable());
//}
//
//}
//
////-------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------
//
//class Rf24ControllerIntegrationTestBase : public ::testing::Test {
//
//   protected:
//
//      Rf24ControllerIntegrationTestBase()
//               : mPower(4, Mcu::I_Pin::MODE_OUTPUT)
//               , mChipEnable1(17, Mcu::I_Pin::MODE_OUTPUT)
//               , mChipEnable2(24, Mcu::I_Pin::MODE_OUTPUT)
//               , mChipSelect1(8, Mcu::I_Pin::MODE_OUTPUT)
//               , mChipSelect2(7, Mcu::I_Pin::MODE_OUTPUT)
//               , mSpi1(Mcu::I_Spi::BIT_ORDER_MSBFIRST, Mcu::I_Spi::MODE_0, Mcu::I_Spi::SPEED_8_MHZ , mChipSelect1)
//               , mSpi2(Mcu::I_Spi::BIT_ORDER_MSBFIRST, Mcu::I_Spi::MODE_0, Mcu::I_Spi::SPEED_8_MHZ , mChipSelect2)
//               , mDevice1(powerOn(),mChipEnable1)
//               , mDevice2(mSpi2,mChipEnable2)
//               , mController1(mDevice1)
//               , mController2(mDevice2)
//            {
//
//            }
//
//      virtual void SetUp() {
//
//
//         I_Rf24DeviceController::Configuration configuration;
//         configuration[RfPipe::PIPE_0] = {true,RfAddress(0xE7,0xE7,0xE7,0xE7,0xE7)};
//         configuration[RfPipe::PIPE_1] = {true,RfAddress(0xC2,0xC2,0xC2,0xC2,0xC2)};
//         configuration[RfPipe::PIPE_2] = {true,RfAddress(0xC2,0xC2,0xC2,0xC2,0xC3)};
//         configuration[RfPipe::PIPE_3] = {true,RfAddress(0xC2,0xC2,0xC2,0xC2,0xC4)};
//         configuration[RfPipe::PIPE_4] = {true,RfAddress(0xC2,0xC2,0xC2,0xC2,0xC5)};
//         configuration[RfPipe::PIPE_5] = {true,RfAddress(0xC2,0xC2,0xC2,0xC2,0xC6)};
//
//
//         mController1.configure(configuration);
//         mController2.configure(configuration);
//      }
//
//      Mcu::I_Spi& powerOn() {
//         mPower.write(false);
//         Util::delayInMilliseconds(1);
//         mPower.write(true);
//         Util::delayInMilliseconds(1);
//         return mSpi1;
//      }
//
//      virtual void TearDown() {
//         mPower.write(false);
//      }
//
//      Mcu::Pin mPower;
//      Mcu::Pin mChipEnable1;
//      Mcu::Pin mChipEnable2;
//      Mcu::Pin mChipSelect1;
//      Mcu::Pin mChipSelect2;
//      Mcu::Spi mSpi1;
//      Mcu::Spi mSpi2;
//
//      Rf24Device mDevice1;
//      Rf24Device mDevice2;
//
//      Rf24DeviceController mController1;
//      Rf24DeviceController mController2;
//
//};
//
////-------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------
//
//class Rf24ControllerIntegrationTest : public Rf24ControllerIntegrationTestBase {
//
//};
//
////-------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------
//
//
////-------------------------------------------------------------------------------------------------
////-------------------------------------------------------------------------------------------------
//
//class DISABLED_Rf24DeviceController$RfPipe$Size$IntegrationTest
//         : public Rf24ControllerIntegrationTestBase
//         , public ::testing::WithParamInterface< ::std::tr1::tuple<RfPipe, size_t>>{
//
//};
//
//INSTANTIATE_TEST_CASE_P(AllPipes,
//                        DISABLED_Rf24DeviceController$RfPipe$Size$IntegrationTest,
//                        ::testing::Combine(
//                                 ::testing::Values(
//                                          RfPipe::PIPE_0,
//                                          RfPipe::PIPE_1,
//                                          RfPipe::PIPE_2,
//                                          RfPipe::PIPE_3,
//                                          RfPipe::PIPE_4,
//                                          RfPipe::PIPE_5
//                                 ),
//                                 ::testing::Values(
//                                          1u,
//                                          2u,
//                                          16u,
//                                          Rf24Device::MAX_PAYLOAD_SIZE-1u,
//                                          Rf24Device::MAX_PAYLOAD_SIZE
//                                 )
//                        )
//);
//
////-------------------------------------------------------------------------------------------------
//
//TEST_P(DISABLED_Rf24DeviceController$RfPipe$Size$IntegrationTest, sendDevice1ToDevice2) {
//   size_t size = ::std::tr1::get<1>(GetParam());
//   std::vector<uint8_t> data(size);
//   std::iota(data.begin(), data.end(), 1);
//
//   mController2.startListening();
//   mController1.write(::std::tr1::get<0>(GetParam()), asArray(data), data.size());
//
//   waitForDataAvailable(mController2);
//
//   RfPipe pipe;
//   std::vector<uint8_t> buffer(size);
//   size_t readSize = mController2.read(asArray(buffer), buffer.size(), pipe);
//
//   EXPECT_EQ(::std::tr1::get<0>(GetParam()), pipe);
//   EXPECT_EQ(size, readSize);
//   ASSERT_THAT(buffer, testing::ElementsAreArray(asArray(data), data.size()));
//
//
//}
//
////-------------------------------------------------------------------------------------------------
//
//TEST_P(DISABLED_Rf24DeviceController$RfPipe$Size$IntegrationTest, sendDevice2ToDevice1) {
//   size_t size = ::std::tr1::get<1>(GetParam());
//   std::vector<uint8_t> data(size);
//   std::iota(data.begin(), data.end(), 1);
//
//   mController1.startListening();
//   mController2.write(::std::tr1::get<0>(GetParam()), asArray(data), data.size());
//
//   waitForDataAvailable(mController1);
//
//   RfPipe pipe;
//   std::vector<uint8_t> buffer(size);
//   size_t readSize = mController1.read(asArray(buffer), buffer.size(), pipe);
//
//   EXPECT_EQ(::std::tr1::get<0>(GetParam()), pipe);
//   EXPECT_EQ(size, readSize);
//   ASSERT_THAT(buffer, testing::ElementsAreArray(asArray(data), data.size()));
//}
//
////-------------------------------------------------------------------------------------------------
//
//TEST_P(DISABLED_Rf24DeviceController$RfPipe$Size$IntegrationTest, sendAndReveive) {
//   size_t size = ::std::tr1::get<1>(GetParam());
//   std::vector<uint8_t> data(size);
//   std::iota(data.begin(), data.end(), 1);
//
//   mController2.startListening();
//   mController1.write(::std::tr1::get<0>(GetParam()), asArray(data), data.size());
//   mController1.startListening();
//
//   {
//      waitForDataAvailable(mController2);
//      std::vector<uint8_t> buffer(size);
//      mController2.read(asArray(buffer), buffer.size());
//      mController2.write(::std::tr1::get<0>(GetParam()), asArray(buffer), buffer.size());
//   }
//
//   waitForDataAvailable(mController1);
//   std::vector<uint8_t> readBuffer(size);
//   size_t readSize = mController1.read(asArray(readBuffer), readBuffer.size());
//
//
//   EXPECT_EQ(size, readSize);
//
//   ASSERT_THAT(readBuffer, testing::ElementsAreArray(asArray(data), data.size()));
//}
//
////-------------------------------------------------------------------------------------------------
//
//TEST_P(DISABLED_Rf24DeviceController$RfPipe$Size$IntegrationTest, sendAndReveiveInALoop) {
//   size_t size = ::std::tr1::get<1>(GetParam());
//   std::vector<uint8_t> data(size);
//   std::iota(data.begin(), data.end(), 1);
//
//   for (size_t i = 0; i < 3 ; i++) {
//
//      // std::cout << "Loop " << i << std::endl;
//      mController2.startListening();
//
//      mController1.write(::std::tr1::get<0>(GetParam()), asArray(data), data.size());
//      mController1.startListening();
//
//      {
//         waitForDataAvailable(mController2);
//         std::vector<uint8_t> buffer(size);
//         mController2.read(asArray(buffer), buffer.size());
//         mController2.write(::std::tr1::get<0>(GetParam()), asArray(buffer), buffer.size());
//      }
//
//      waitForDataAvailable(mController1);
//      std::vector<uint8_t> readBuffer(size);
//      size_t readSize = mController1.read(asArray(readBuffer), readBuffer.size());
//
//
//      EXPECT_EQ(size, readSize);
//      ASSERT_THAT(readBuffer, testing::ElementsAreArray(asArray(data), data.size()));
//   }
//}
//
//
////-------------------------------------------------------------------------------------------------

} // namespace Device
} // namespace Bt
