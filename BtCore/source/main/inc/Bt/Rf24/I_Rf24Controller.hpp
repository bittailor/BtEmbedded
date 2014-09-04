//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::I_Rf24Controller
//  
//*************************************************************************************************

#ifndef INC__Bt_Device_I_RfController__hpp
#define INC__Bt_Device_I_RfController__hpp

#include <functional>
#include <memory>
#include <stddef.h>
#include <string.h>

#include "Bt/Rf24/I_Rf24Device.hpp"

namespace Bt {
namespace Rf24 {

class I_Rf24DeviceController {
   public:

      class Packet;
      class Configuration;


      enum { MAX_PAYLOAD_SIZE = I_Rf24Device::MAX_PAYLOAD_SIZE };

      virtual ~I_Rf24DeviceController() {}

      virtual void configure(const Configuration& iConfiguration) = 0;

      virtual bool write(RfPipe iPipe, Packet& iPacket) = 0;

      virtual void startListening(std::function<void(RfPipe iPipe, Packet& iPacket)> iCallback) = 0;
      virtual void stopListening() = 0;

      virtual void reboot() = 0;

};

class I_Rf24DeviceController::Packet {
   public:
      enum { CAPACITY = I_Rf24Device::MAX_PAYLOAD_SIZE };

      Packet() : mBuffer(std::make_shared<Buffer>()) {
      }

      uint8_t* buffer() {
         return mBuffer->second;
      }

      void* rawBuffer() {
         return mBuffer->second;
      }

      size_t size() const {
         return mBuffer->first;
      }

      void size(size_t iSize) {
         mBuffer->first = iSize;
      }

      void copy(I_Rf24DeviceController::Packet& iPacket) const {
         memcpy(iPacket.mBuffer->second, mBuffer->second, CAPACITY);
         iPacket.mBuffer->first = mBuffer->first;
      }

   private:
      typedef std::pair<size_t,uint8_t[CAPACITY]> Buffer;
      std::shared_ptr<Buffer> mBuffer;
};

class I_Rf24DeviceController::Configuration {
   public:

      struct PipeConfiguration {
         bool mEnabled;
         RfAddress mAddress;
      };

      Configuration(uint8_t iAutoRetransmitDelay = 0x6) : mAutoRetransmitDelay(iAutoRetransmitDelay) {

      }

      uint8_t autoRetransmitDelay() const {
         return mAutoRetransmitDelay;
      }

      PipeConfiguration& operator[](RfPipe iPipe) {
         return mPipeConfigurations[static_cast<size_t>(iPipe)];
      }

   private:
      Util::StaticArray<PipeConfiguration,RfPipes::NUMBER_OF_PIPES> mPipeConfigurations;
      uint8_t mAutoRetransmitDelay;
};



} // namespace Device
} // namespace Bt

#endif // INC__Bt_Device_I_RfController__hpp
