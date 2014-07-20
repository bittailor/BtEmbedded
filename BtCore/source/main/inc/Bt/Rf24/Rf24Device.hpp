//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::Rf24Device
//  
//*************************************************************************************************

#ifndef INC__Bt_Device_Rf24Device__hpp
#define INC__Bt_Device_Rf24Device__hpp

#include "Bt/Mcu/I_Spi.hpp"
#include "Bt/Mcu/I_Pin.hpp"
#include "Bt/Rf24/I_Rf24Device.hpp"

#include "Bt/Rf24/RfPipe.hpp"

namespace Bt {
namespace Rf24 {

class Rf24Device : public I_Rf24Device
{
   public:

      Rf24Device(Mcu::I_Spi& iSpi, Mcu::I_Pin& iChipEnable);
      virtual ~Rf24Device();

      virtual Status status();

      virtual void clearDataReady();
      virtual void clearDataSent();
      virtual void clearRetransmitsExceeded();

      virtual bool powerUp();
      virtual void powerUp(bool iValue);

      virtual TransceiverMode transceiverMode();
      virtual void transceiverMode(TransceiverMode iMode);

      virtual void chipEnable(bool iValue);

      virtual uint8_t autoRetransmitDelay();
      virtual void autoRetransmitDelay(uint8_t iDelay);

      virtual uint8_t autoRetransmitCount();
      virtual void autoRetransmitCount(uint8_t iCount);

      virtual uint8_t autoRetransmitCounter();

      virtual uint8_t channel();
      virtual void channel(uint8_t iChannel);

      virtual DataRate dataRate();
      virtual void dataRate(DataRate iDataRate);

      virtual RfAddress receiveAddress(RfPipe iPipe);
      virtual void receiveAddress(RfPipe iPipe, RfAddress iAddress);

      virtual bool receivePipeEnabled(RfPipe iPipe);
      virtual void receivePipeEnabled(RfPipe iPipe, bool iValue);

      virtual uint8_t receivePayloadSize(RfPipe iPipe);
      virtual void receivePayloadSize(RfPipe iPipe, uint8_t iSize);

      virtual RfAddress transmitAddress();
      virtual void transmitAddress(RfAddress iAddress);

      virtual bool isTransmitFifoEmpty();
      virtual bool isTransmitFifoFull();
      virtual void flushTransmitFifo();

      virtual bool isReceiveFifoEmpty();
      virtual bool isReceiveFifoFull();
      virtual void flushReceiveFifo();

      virtual size_t writeTransmitPayload(uint8_t* iData, size_t iSize);

      virtual size_t availableReceivePayload();
      virtual size_t readReceivePayload(RfPipe& oPipe, uint8_t* oData, size_t iSize);

      virtual bool dynamicPayloadFeatureEnabled();
      virtual void dynamicPayloadFeatureEnabled(bool iValue);

      virtual bool dynamicPayloadEnabled(RfPipe iPipe);
      virtual void dynamicPayloadEnabled(RfPipe iPipe, bool iValue);


   private:
   	  // Constructor to prohibit copy construction.
      Rf24Device(const Rf24Device&);

      // Operator= to prohibit copy assignment
      Rf24Device& operator=(const Rf24Device&);

      Mcu::I_Spi* mSpi;
      Mcu::I_Pin* mChipEnable;
};

} // namespace Device
} // namespace Bt

#endif // INC__Bt_Device_Rf24Device__hpp
