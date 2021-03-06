//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::Rf24DeviceIntegrationTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Bt/Rf24/Rf24Device.hpp"
#include "Bt/Mcu/Pin.hpp"
#include "Bt/Mcu/Spi.hpp"
#include "Bt/Util/Timing.hpp"
#include "Bt/Util/CollectionUtility.hpp"

namespace Bt {
namespace Rf24 {

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

struct Rf24DeviceIntegrationTestParameters {
      Mcu::I_Spi::ChipSelect chipSelect;
      uint8_t chipEnable;
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

class Rf24DeviceIntegrationTest : public ::testing::Test, public ::testing::WithParamInterface<Rf24DeviceIntegrationTestParameters> {
   
   protected:
      
      Rf24DeviceIntegrationTest()
         : mPower(27, Mcu::I_Pin::MODE_OUTPUT)
         , mSpi(Mcu::I_Spi::BIT_ORDER_MSBFIRST, Mcu::I_Spi::MODE_0, Mcu::I_Spi::SPEED_8_MHZ , GetParam().chipSelect)
         , mChipEnable(GetParam().chipEnable, Mcu::I_Pin::MODE_OUTPUT)
         , mDevice(powerOn(),mChipEnable) {

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
      Mcu::Spi mSpi;
      Mcu::Pin mChipEnable;

      Rf24Device mDevice;

};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

INSTANTIATE_TEST_CASE_P(Default, Rf24DeviceIntegrationTest,::testing::Values(
         Rf24DeviceIntegrationTestParameters{Mcu::I_Spi::CHIP_SELECT_0,25}
         //Rf24DeviceIntegrationTestParameters{7,24}
));


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, readDefaultPowerUp) {
   EXPECT_FALSE(mDevice.powerUp());

}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeAndReadBackPowerUp) {
   mDevice.powerUp(true);
   EXPECT_TRUE(mDevice.powerUp());
}


//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, readDefaultTransceiverMode) {
   EXPECT_EQ(I_Rf24Device::TX_MODE, mDevice.transceiverMode());
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeAndReadBackTransceiverMode) {
   mDevice.transceiverMode(I_Rf24Device::RX_MODE);
   EXPECT_EQ(I_Rf24Device::RX_MODE, mDevice.transceiverMode());
}


//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, readDefaultChannel) {
   EXPECT_EQ(0x2,(int)mDevice.channel());

}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeAndReadBackChannel) {
   uint8_t rf = 0x4c;
   mDevice.channel(rf);
   EXPECT_EQ((int)rf,(int)mDevice.channel());
}


//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, readDefaultDataRate) {
   EXPECT_EQ(I_Rf24Device::DR_2_MBPS, mDevice.dataRate());

}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeAndReadBackDataRate) {
   mDevice.dataRate(I_Rf24Device::DR_250_KBPS);
   EXPECT_EQ(I_Rf24Device::DR_250_KBPS, mDevice.dataRate());

   mDevice.dataRate(I_Rf24Device::DR_1_MBPS);
   EXPECT_EQ(I_Rf24Device::DR_1_MBPS, mDevice.dataRate());
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, readDefaultAutoRetransmitCount) {
   EXPECT_EQ(0x03,(int)mDevice.autoRetransmitCount());
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeAndReadBackAutoRetransmitCount) {
   uint8_t count = 0x0a;
   mDevice.autoRetransmitCount(count);
   EXPECT_EQ((int)count,(int)mDevice.autoRetransmitCount());
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, readDefaultAutoRetransmitDelay) {
   EXPECT_EQ(0x00,(int)mDevice.autoRetransmitDelay());

}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeAndReadBackAutoRetransmitDelay) {
   uint8_t delay = 0x04;
   mDevice.autoRetransmitDelay(delay);
   EXPECT_EQ((int)delay,(int)mDevice.autoRetransmitDelay());
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeSameRegister) {
   uint8_t delay = 0x04;
   uint8_t count = 0x0a;
   mDevice.autoRetransmitDelay(delay);
   EXPECT_EQ((int)delay,(int)mDevice.autoRetransmitDelay());
   EXPECT_EQ(0x03,(int)mDevice.autoRetransmitCount());
   mDevice.autoRetransmitCount(count);
   EXPECT_EQ((int)delay,(int)mDevice.autoRetransmitDelay());
   EXPECT_EQ((int)count,(int)mDevice.autoRetransmitCount());
}


//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, readDefaultRxPipes) {

   RfPipe pipes[] = {
            RfPipe::PIPE_0,
            RfPipe::PIPE_1,
            RfPipe::PIPE_2,
            RfPipe::PIPE_3,
            RfPipe::PIPE_4,
            RfPipe::PIPE_5,
   };

   RfAddress defaultAddresses[] = {
            {0xE7, 0xE7, 0xE7, 0xE7, 0xE7},
            {0xC2, 0xC2, 0xC2, 0xC2, 0xC2},
            {0xC2, 0xC2, 0xC2, 0xC2, 0xC3},
            {0xC2, 0xC2, 0xC2, 0xC2, 0xC4},
            {0xC2, 0xC2, 0xC2, 0xC2, 0xC5},
            {0xC2, 0xC2, 0xC2, 0xC2, 0xC6},
   };

   for (size_t i = 0 ; i < Util::sizeOfArray(pipes) ; i++) {
      RfAddress address = mDevice.receiveAddress(pipes[i]).raw();
      ASSERT_THAT(address.raw(), testing::ElementsAreArray(defaultAddresses[i].raw().begin(),defaultAddresses[i].raw().size()));
   }
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeAndReadBackRxPipes) {

   RfPipe pipes[] = {
            RfPipe::PIPE_0,
            RfPipe::PIPE_1,
            RfPipe::PIPE_2,
            RfPipe::PIPE_3,
            RfPipe::PIPE_4,
            RfPipe::PIPE_5
   };

   RfAddress addresses[] = {
            {0x22, 0x23, 0x24, 0x25, 0x26},
            {0xA1, 0xB1, 0xC1, 0xD1, 0xE1},
            {0xA1, 0xB1, 0xC1, 0xD1, 0xE2},
            {0xA1, 0xB1, 0xC1, 0xD1, 0xE3},
            {0xA1, 0xB1, 0xC1, 0xD1, 0xE4},
            {0xA1, 0xB1, 0xC1, 0xD1, 0xE5}
   };

   for (size_t i = 0 ; i < Util::sizeOfArray(pipes) ; i++) {
      mDevice.receiveAddress(pipes[i], addresses[i]);
   }

   for (size_t i = 0 ; i < Util::sizeOfArray(pipes) ; i++) {
      RfAddress address = mDevice.receiveAddress(pipes[i]).raw();
      ASSERT_THAT(address.raw(), testing::ElementsAreArray(addresses[i].raw().begin(),addresses[i].raw().size()));
   }

}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, readDefaultReceivePipeEnabled) {

   RfPipe pipes[] = {
            RfPipe::PIPE_0,
            RfPipe::PIPE_1,
            RfPipe::PIPE_2,
            RfPipe::PIPE_3,
            RfPipe::PIPE_4,
            RfPipe::PIPE_5,
   };

   bool defaults[] = {
            true,
            true,
            false,
            false,
            false,
            false,
   };

   for (size_t i = 0 ; i < Util::sizeOfArray(pipes) ; i++) {
      EXPECT_EQ(defaults[i], mDevice.receivePipeEnabled(pipes[i]));
   }
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeAndReadBackReceivePipeEnabled) {

   RfPipe pipes[] = {
            RfPipe::PIPE_0,
            RfPipe::PIPE_1,
            RfPipe::PIPE_2,
            RfPipe::PIPE_3,
            RfPipe::PIPE_4,
            RfPipe::PIPE_5,
   };

   bool value[] = {
            false,
            false,
            true,
            true,
            true,
            true,
   };

   for (size_t i = 0 ; i < Util::sizeOfArray(pipes) ; i++) {
      mDevice.receivePipeEnabled(pipes[i], value[i]);
      EXPECT_EQ(value[i], mDevice.receivePipeEnabled(pipes[i]));
   }
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, readDefaultReceivePayloadSize) {

   RfPipe pipes[] = {
            RfPipe::PIPE_0,
            RfPipe::PIPE_1,
            RfPipe::PIPE_2,
            RfPipe::PIPE_3,
            RfPipe::PIPE_4,
            RfPipe::PIPE_5,
   };

   for (size_t i = 0 ; i < Util::sizeOfArray(pipes) ; i++) {
      uint8_t size = mDevice.receivePayloadSize(pipes[i]);
      EXPECT_EQ(0, (int)size);
   }
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeAndReadBackReceivePayloadSize) {

   RfPipe pipes[] = {
            RfPipe::PIPE_0,
            RfPipe::PIPE_1,
            RfPipe::PIPE_2,
            RfPipe::PIPE_3,
            RfPipe::PIPE_4,
            RfPipe::PIPE_5,
   };

   for (size_t i = 0 ; i < Util::sizeOfArray(pipes) ; i++) {
      mDevice.receivePayloadSize(pipes[i], Rf24Device::MAX_PAYLOAD_SIZE);
      uint8_t size = mDevice.receivePayloadSize(pipes[i]);
      EXPECT_EQ((int)Rf24Device::MAX_PAYLOAD_SIZE, (int)size);
   }
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, readDefaultTx) {
   RfAddress defaultAddress = RfAddress(0xE7,0xE7,0xE7,0xE7,0xE7);

   RfAddress address = mDevice.transmitAddress();
   ASSERT_THAT(defaultAddress.raw(), testing::ElementsAreArray(address.raw().begin(),address.raw().size()));
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeAndReadBackTx) {
   RfAddress newAddress = RfAddress(0xE7,0xE7,0xE7,0xE7,0xE7);
   mDevice.transmitAddress(newAddress);

   RfAddress address = mDevice.transmitAddress();
   ASSERT_THAT(newAddress.raw(), testing::ElementsAreArray(address.raw().begin(),address.raw().size()));
 }

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, readDefaultIsTransmitFifoEmpty) {
   EXPECT_TRUE(mDevice.isTransmitFifoEmpty());
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, readDefaultIsTransmitFifoFull) {
   EXPECT_FALSE(mDevice.isTransmitFifoFull());
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeOneTransmitPayload) {
   uint8_t data[] = {1,2,3,4,5};

   mDevice.writeTransmitPayload(data, Util::sizeOfArray(data));

   EXPECT_FALSE(mDevice.isTransmitFifoEmpty());
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeThreeTransmitPayloads) {
   uint8_t data[] = {1,2,3,4,5};

   mDevice.writeTransmitPayload(data, Util::sizeOfArray(data));
   mDevice.writeTransmitPayload(data, Util::sizeOfArray(data));
   mDevice.writeTransmitPayload(data, Util::sizeOfArray(data));

   EXPECT_TRUE(mDevice.isTransmitFifoFull());
}
//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeUndersizeTransmitPayload) {
   uint8_t data[] = {1,2,3,4,5};

   size_t written = mDevice.writeTransmitPayload(data, Util::sizeOfArray(data));

   EXPECT_EQ(Util::sizeOfArray(data), written);
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeExactTransmitPayload) {
   uint8_t data[Rf24Device::MAX_PAYLOAD_SIZE] = {1,2,3,4,5};

   size_t written = mDevice.writeTransmitPayload(data, Util::sizeOfArray(data));

   EXPECT_EQ(Util::sizeOfArray(data), written);
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeOversizeTransmitPayload) {
   uint8_t data[Rf24Device::MAX_PAYLOAD_SIZE + 20] = {1,2,3,4,5};

   size_t written = mDevice.writeTransmitPayload(data, Util::sizeOfArray(data));

   EXPECT_EQ((size_t)Rf24Device::MAX_PAYLOAD_SIZE, written);
   EXPECT_FALSE(mDevice.isTransmitFifoEmpty());
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, readDefaultDynamicPayloadFeatureEnabled) {
   EXPECT_FALSE(mDevice.dynamicPayloadFeatureEnabled());

}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeAndReadBackDynamicPayloadFeatureEnabled) {
   mDevice.dynamicPayloadFeatureEnabled(true);
   EXPECT_TRUE(mDevice.dynamicPayloadFeatureEnabled());
   mDevice.dynamicPayloadFeatureEnabled(false);
   EXPECT_FALSE(mDevice.dynamicPayloadFeatureEnabled());
}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, readDefaultDynamicPayloadEnabled) {
   EXPECT_FALSE(mDevice.dynamicPayloadEnabled(RfPipe::PIPE_0));

}

//-------------------------------------------------------------------------------------------------

TEST_P(Rf24DeviceIntegrationTest, writeAndReadBackDynamicPayloadEnabled) {
   mDevice.dynamicPayloadEnabled(RfPipe::PIPE_0, true);
   EXPECT_TRUE(mDevice.dynamicPayloadEnabled(RfPipe::PIPE_0));
   mDevice.dynamicPayloadEnabled(RfPipe::PIPE_0, false);
   EXPECT_FALSE(mDevice.dynamicPayloadEnabled(RfPipe::PIPE_0));
}

//-------------------------------------------------------------------------------------------------


} // namespace Device
} // namespace Bt
