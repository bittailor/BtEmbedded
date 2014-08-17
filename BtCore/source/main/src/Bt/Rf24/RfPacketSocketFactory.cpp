//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfPacketSocketFactory
//  
//*************************************************************************************************

#include "Bt/Rf24/RfPacketSocketFactory.hpp"

#include <thread>
#include <atomic>

#include <Bt/Rf24/I_RfPacketSocket.hpp>
#include <Bt/Mcu/Pin.hpp>
#include <Bt/Mcu/Spi.hpp>
#include <Bt/Rf24/Rf24Device.hpp>
#include <Bt/Rf24/RfNetworkSocket.hpp>
#include <Bt/Rf24/Rf24Controller.hpp>
#include <Bt/Rf24/RfPacketSocket.hpp>
#include <Bt/Concurrency/ExecutionContext.hpp>

namespace Bt {
namespace Rf24 {

namespace {

class RfPacketSocketWrapper : public I_RfPacketSocket {
   public:

      RfPacketSocketWrapper(uint8_t iChipEnable, Mcu::I_Spi::ChipSelect iChipSelect, uint8_t iIrq,  uint8_t iNodeId)
      : mChipEnable(iChipEnable, Mcu::I_Pin::MODE_OUTPUT)
      , mSpi(Mcu::I_Spi::BIT_ORDER_MSBFIRST, Mcu::I_Spi::MODE_0, Mcu::I_Spi::SPEED_8_MHZ , iChipSelect)
      , mIrq(iIrq, Mcu::I_InterruptPin::Edge::FALLING)
      , mDevice(mSpi,mChipEnable)
      , mController(mDevice,mIrq,mExecutionContext)
      , mNetworkSocket(iNodeId, mController)
      , mPacketSocket(mNetworkSocket,mExecutionContext) {

      }

      ~RfPacketSocketWrapper() {
      }

      virtual size_t payloadCapacity() const {
         return mPacketSocket.payloadCapacity();
      }

      virtual bool hasPendingPacket() const {
         return mPacketSocket.hasPendingPacket();
      }

      virtual size_t pendingPacketSize() const {
         return mPacketSocket.pendingPacketSize();
      }

      virtual int32_t send(const uint8_t* iPayload, size_t iSize, uint8_t iNodeId) {
         return mPacketSocket.send(iPayload, iSize, iNodeId);
      }

      virtual int32_t receive(uint8_t* oPayload, size_t iMaxSize, uint8_t* oNodeId) {
         return mPacketSocket.receive(oPayload, iMaxSize, oNodeId);
      }

      virtual void close() {
         mPacketSocket.close();
      }

   private:
      Mcu::Pin mChipEnable;
      Mcu::Spi mSpi;
      Mcu::InterruptPin mIrq;
      Concurrency::ExecutionContext mExecutionContext;
      Rf24Device mDevice;
      Rf24DeviceController mController;
      RfNetworkSocket mNetworkSocket;
      RfPacketSocket mPacketSocket;
};

} // namespace

//-------------------------------------------------------------------------------------------------

std::shared_ptr<I_RfPacketSocket> RfPacketSocketFactory::createPacketSocket(uint8_t iChipEnable, Mcu::I_Spi::ChipSelect iChipSelect, uint8_t iIrq, uint8_t iNodeId) {

   std::shared_ptr<I_RfPacketSocket> socket(new RfPacketSocketWrapper(iChipEnable,iChipSelect,iIrq,iNodeId));


   return socket;
}

//-------------------------------------------------------------------------------------------------

} // namespace Rf24
} // namespace Bt
