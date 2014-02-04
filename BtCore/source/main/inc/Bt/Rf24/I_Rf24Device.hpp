//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::I_Rf24Device
//  
//*************************************************************************************************

#ifndef INC__Bt_Device_I_Rf24Device__hpp
#define INC__Bt_Device_I_Rf24Device__hpp

#include <stdint.h>

#include "Bt/Util/StaticArray.hpp"
#include "Bt/Rf24/RfPipe.hpp"
#include "Bt/Rf24/RfAddress.hpp"

namespace Bt {
namespace Rf24 {

class I_Rf24Device {

   public:

      enum {
         MAX_PAYLOAD_SIZE = 32
      };

      enum TransceiverMode {
         TX_MODE,
         RX_MODE
      };

      enum DataRate {
         DR_250_KBPS,
         DR_1_MBPS,
         DR_2_MBPS
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

      virtual DataRate dataRate() = 0;
      virtual void dataRate(DataRate pDataRate) = 0;

      virtual RfAddress receiveAddress(RfPipe pPipe) = 0;
      virtual void receiveAddress(RfPipe pPipe, RfAddress pAddress) = 0;

      virtual bool receivePipeEnabled(RfPipe pPipe) = 0;
      virtual void receivePipeEnabled(RfPipe pPipe, bool pValue) = 0;

      virtual uint8_t receivePayloadSize(RfPipe pPipe) = 0;
      virtual void receivePayloadSize(RfPipe pPipe, uint8_t pSize) = 0;

      virtual RfAddress transmitAddress() = 0;
      virtual void transmitAddress(RfAddress pAddress) = 0;

      virtual bool isTransmitFifoEmpty() = 0;
      virtual bool isTransmitFifoFull() = 0;
      virtual void flushTransmitFifo() =0;

      virtual bool isReceiveFifoEmpty() = 0;
      virtual bool isReceiveFifoFull() = 0;

      virtual size_t writeTransmitPayload(uint8_t* pData, size_t pSize) = 0;

      virtual size_t availableReceivePayload() = 0;
      virtual size_t readReceivePayload(RfPipe& pPipe, uint8_t* pData, size_t pSize) = 0;

      virtual bool dynamicPayloadFeatureEnabled() = 0;
      virtual void dynamicPayloadFeatureEnabled(bool pValue) = 0;

      virtual bool dynamicPayloadEnabled(RfPipe pPipe) = 0;
      virtual void dynamicPayloadEnabled(RfPipe pPipe, bool pValue) = 0;

};

} // namespace Device
} // namespace Bt

#endif // INC__Bt_Device_I_Rf24Device__hpp
