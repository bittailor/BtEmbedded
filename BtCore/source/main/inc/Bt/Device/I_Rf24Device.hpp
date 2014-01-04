//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::I_Rf24Device
//  
//*************************************************************************************************

#ifndef INC__Bt_Device_I_Rf24Device__hpp
#define INC__Bt_Device_I_Rf24Device__hpp

#include <stdint.h>
#include "Bt/Util/StaticArray.hpp"

namespace Bt {
namespace Device {

class I_Rf24Device {

   public:

      enum {
         MAX_PAYLOAD_SIZE = 32
      };

      enum class Pipe {
         PIPE_0,
         PIPE_1,
         PIPE_2,
         PIPE_3,
         PIPE_4,
         PIPE_5,
      };

      enum TransceiverMode {
         TX_MODE,
         RX_MODE
      };

      class Status {
         public:
            Status(bool pDataReady, bool pDataSent, bool pRetransmitsExceeded)
            : mDataReady(pDataReady), mDataSent(pDataSent), mRetransmitsExceeded(pRetransmitsExceeded)  {}

            bool dataReady() {return mDataReady;}
            bool dataSent() {return mDataSent;}
            bool retransmitsExceeded() {return mRetransmitsExceeded;}


         private:
            bool mDataReady;
            bool mDataSent;
            bool mRetransmitsExceeded;
      };

      class Address {
         public:
            Address(){
            }

            Address(Util::StaticArray<uint8_t,5> pRaw) : mRaw(pRaw){
            }

            Address(uint8_t pByte4, uint8_t pByte3, uint8_t pByte2, uint8_t pByte1, uint8_t pByte0) {
               mRaw[0] = pByte0;
               mRaw[1] = pByte1;
               mRaw[2] = pByte2;
               mRaw[3] = pByte3;
               mRaw[4] = pByte4;
            }

            Util::StaticArray<uint8_t,5>& raw() {
               return mRaw;
            }

         private:
            Util::StaticArray<uint8_t,5> mRaw;
      };

      virtual ~I_Rf24Device() {}

      virtual Status status() = 0;

      virtual void clearDataReady() = 0;
      virtual void clearDataSent() = 0;
      virtual void clearRetransmitsExceeded() = 0;

      virtual bool powerUp() = 0;
      virtual void powerUp(bool pValue) = 0;

      virtual TransceiverMode transceiverMode() = 0;
      virtual void transceiverMode(TransceiverMode pMode) = 0;

      virtual void chipEnable(bool pValue) = 0;

      virtual uint8_t autoRetransmitDelay() = 0;
      virtual void autoRetransmitDelay(uint8_t pDelay) = 0;

      virtual uint8_t autoRetransmitCount() = 0;
      virtual void autoRetransmitCount(uint8_t pCount) = 0;

      virtual uint8_t autoRetransmitCounter() = 0;

      virtual uint8_t channel() = 0;
      virtual void channel(uint8_t pChannel) = 0;

      virtual Address receiveAddress(Pipe pPipe) = 0;
      virtual void receiveAddress(Pipe pPipe, Address pAddress) = 0;

      virtual bool receivePipeEnabled(Pipe pPipe) = 0;
      virtual void receivePipeEnabled(Pipe pPipe, bool pValue) = 0;

      virtual uint8_t receivePayloadSize(Pipe pPipe) = 0;
      virtual void receivePayloadSize(Pipe pPipe, uint8_t pSize) = 0;

      virtual Address transmitAddress() = 0;
      virtual void transmitAddress(Address pAddress) = 0;

      virtual bool isTransmitFifoEmpty() = 0;
      virtual bool isTransmitFifoFull() = 0;
      virtual void flushTransmitFifo() =0;

      virtual bool isReceiveFifoEmpty() = 0;
      virtual bool isReceiveFifoFull() = 0;

      virtual size_t writeTransmitPayload(uint8_t* pData, size_t pSize) = 0;

      virtual size_t availableReceivePayload() = 0;
      virtual size_t readReceivePayload(Pipe& pPipe, uint8_t* pData, size_t pSize) = 0;

      virtual bool dynamicPayloadFeatureEnabled() = 0;
      virtual void dynamicPayloadFeatureEnabled(bool pValue) = 0;

      virtual bool dynamicPayloadEnabled(Pipe pPipe) = 0;
      virtual void dynamicPayloadEnabled(Pipe pPipe, bool pValue) = 0;

};

} // namespace Device
} // namespace Bt

#endif // INC__Bt_Device_I_Rf24Device__hpp
