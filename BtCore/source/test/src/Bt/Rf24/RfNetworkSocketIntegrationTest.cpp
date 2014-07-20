//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfNetworkSocketIntegrationTest
//  
//*************************************************************************************************

#include <chrono>
#include <thread>
#include <algorithm>

#include <gtest/gtest.h>
#include <gmock/gmock.h>


#include "Bt/Rf24/Rf24Controller.hpp"
#include "Bt/Rf24/Rf24Device.hpp"
#include "Bt/Mcu/Pin.hpp"
#include "Bt/Mcu/Spi.hpp"
#include "Bt/Util/Timing.hpp"
#include "Bt/Util/CollectionUtility.hpp"
#include "Bt/Rf24/RfNetworkSocket.hpp"

#include "Bt/Rf24/RfNetworkSocketListenerGMock.hpp"

using namespace ::testing;
using ::testing::_;

namespace Bt {
namespace Rf24 {

//namespace {
//
//void waitForFlag(RfNetworkSocket& iSocket, bool& iFlag) {
//   int counter = 0;
//   iSocket.workcycle();
//   while(!pFlag && counter < 200) {
//      Util::delayInMilliseconds(5);
//      counter++;
//      iSocket.workcycle();
//   }
//   ASSERT_TRUE(iFlag);
//}
//
//
//}
//
//class DISABLED_RfNetworkSocketIntegrationTest : public ::testing::Test {
//
//   protected:
//
//      DISABLED_RfNetworkSocketIntegrationTest()
//      : mPower(4, Mcu::I_Pin::MODE_OUTPUT)
//      , mChipEnable1(17, Mcu::I_Pin::MODE_OUTPUT)
//      , mChipEnable2(24, Mcu::I_Pin::MODE_OUTPUT)
//      , mChipSelect1(8, Mcu::I_Pin::MODE_OUTPUT)
//      , mChipSelect2(7, Mcu::I_Pin::MODE_OUTPUT)
//      , mSpi1(Mcu::I_Spi::BIT_ORDER_MSBFIRST, Mcu::I_Spi::MODE_0, Mcu::I_Spi::SPEED_8_MHZ , mChipSelect1)
//      , mSpi2(Mcu::I_Spi::BIT_ORDER_MSBFIRST, Mcu::I_Spi::MODE_0, Mcu::I_Spi::SPEED_8_MHZ , mChipSelect2)
//      , mDevice1(powerOn(),mChipEnable1)
//      , mDevice2(mSpi2,mChipEnable2)
//      , mController1(mDevice1)
//      , mController2(mDevice2)
//      , mNetworkSocket1(0, mController1)
//      , mNetworkSocket2(1, mController2)
//      {
//
//      }
//
//      virtual void SetUp() {
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
//      RfNetworkSocket mNetworkSocket1;
//      RfNetworkSocket mNetworkSocket2;
//
//};
//
//
//
//
//
//TEST_F(DISABLED_RfNetworkSocketIntegrationTest, SendStringFromOneToTwo) {
//
//   std::string payload = "Hello Rf 24";
//   std::string receivedPayload;
//   RfNetworkSocketListenerGMock listenerMock;
//   bool callback = false;
//   EXPECT_CALL(listenerMock, packetReceived(_))
//      .WillOnce(Invoke([&receivedPayload,&callback](I_RfNetworkSocket::Packet& iPacket) {
//         receivedPayload = (char*)iPacket.payload();
//         callback = true;
//      }));
//
//   mNetworkSocket2.setListener(listenerMock);
//
//   I_RfNetworkSocket::Packet packet;
//   packet.destination(1);
//   packet.writePayload(payload.c_str(), payload.size()+1);
//
//   bool sendSuccessful = mNetworkSocket1.send(packet);
//   EXPECT_TRUE(sendSuccessful);
//
//   waitForFlag(mNetworkSocket2, callback);
//
//   EXPECT_EQ(payload, receivedPayload);
//
//}


} // namespace Rf24
} // namespace Bt
