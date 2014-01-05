//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::I_RfController
//  
//*************************************************************************************************

#ifndef INC__Bt_Device_I_RfController__hpp
#define INC__Bt_Device_I_RfController__hpp

#include <stddef.h>

#include "Bt/Device/I_Rf24Device.hpp"

namespace Bt {
namespace Rf24 {

class I_RfController {
   public:

      class Packet;
      struct Configuration;

      using Pipe = I_Rf24Device::Pipe;
      using Address = I_Rf24Device::Address;

      enum { MAX_PAYLOAD_SIZE = I_Rf24Device::MAX_PAYLOAD_SIZE };

      virtual ~I_RfController() {}

      virtual void configure(const Configuration& pConfiguration) = 0;

      virtual bool write(Pipe pPipe, Packet pPacket) = 0;

      virtual size_t write(Pipe pPipe, uint8_t* pData, size_t pSize) = 0;

      virtual void startListening() = 0;
      virtual void stopListening() = 0;
      virtual bool isDataAvailable() = 0;

      virtual bool read(Packet pPacket) = 0;
      virtual bool read(Packet pPacket, Pipe& pPipe) = 0;

      virtual size_t read(uint8_t* pBuffer, size_t pSize) = 0;
      virtual size_t read(uint8_t* pBuffer, size_t pSize, Pipe& pPipe) = 0;
};

class I_RfController::Packet {
   public:
      enum { BUFFER_CAPACITY = I_Rf24Device::MAX_PAYLOAD_SIZE };

      uint8_t* buffer() {
         return mBuffer;
      }

      size_t size() {
         return mSize;
      }

      void size(size_t pSize) {
         mSize = pSize;
      }

   private:
      uint8_t mBuffer[BUFFER_CAPACITY] ;
      size_t mSize;
};

class I_RfController::Configuration {
   public:

      struct PipeConfiguration {
         bool mEnabled;
         I_RfController::Address mAddress;
      };

      PipeConfiguration& operator[](I_Rf24Device::Pipe pPipe) {
         return mPipeConfigurations[static_cast<size_t>(pPipe)];
      }

   private:
      Util::StaticArray<PipeConfiguration,I_Rf24Device::NUMBER_OF_PIPES> mPipeConfigurations;
};



} // namespace Device
} // namespace Bt

#endif // INC__Bt_Device_I_RfController__hpp
