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

#include <stddef.h>
#include <string.h>

#include "Bt/Rf24/I_Rf24Device.hpp"

namespace Bt {
namespace Rf24 {

class I_Rf24DeviceController {
   public:

      class Packet;
      class Configuration;
      class I_Listener;

      enum { MAX_PAYLOAD_SIZE = I_Rf24Device::MAX_PAYLOAD_SIZE };

      virtual ~I_Rf24DeviceController() {}

      virtual void configure(const Configuration& iConfiguration) = 0;

      virtual bool write(RfPipe iPipe, Packet& iPacket) = 0;
      virtual size_t write(RfPipe iPipe, uint8_t* iData, size_t iSize) = 0;

      virtual void startListening() = 0;
      virtual void stopListening() = 0;
      virtual bool isDataAvailable() = 0;

      virtual bool read(Packet& oPacket) = 0;
      virtual bool read(Packet& oPacket, RfPipe& oPipe) = 0;

      virtual size_t read(uint8_t* oBuffer, size_t iSize) = 0;
      virtual size_t read(uint8_t* oBuffer, size_t iSize, RfPipe& oPipe) = 0;
};

class I_Rf24DeviceController::I_Listener {
      virtual ~I_Listener() {}

      void received(Packet& iPacket, RfPipe& iPipe);

};

class I_Rf24DeviceController::Packet {
   public:
      enum { BUFFER_CAPACITY = I_Rf24Device::MAX_PAYLOAD_SIZE };

      Packet() : mBuffer(), mSize(0) {

      }

      uint8_t* buffer() {
         return mBuffer;
      }

      void* rawBuffer() {
         return mBuffer;
      }

      size_t size() {
         return mSize;
      }

      void size(size_t iSize) {
         mSize = iSize;
      }

      void copy(I_Rf24DeviceController::Packet& iPacket) const {
         memcpy(iPacket.mBuffer, mBuffer, BUFFER_CAPACITY);
         iPacket.mSize = mSize;
      }

   private:
      // Constructor to prohibit copy construction.
      Packet(const Packet&);

      // Operator= to prohibit copy assignment
      Packet& operator=(const Packet&);



      uint8_t mBuffer[BUFFER_CAPACITY] ;
      size_t mSize;
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
