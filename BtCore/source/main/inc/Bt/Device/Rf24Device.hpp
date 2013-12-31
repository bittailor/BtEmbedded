//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::Rf24Device
//  
//*************************************************************************************************

#ifndef INC__Bt_Device_Rf24Device__hpp
#define INC__Bt_Device_Rf24Device__hpp

#include "Bt/Mcu/I_Spi.hpp"
#include "Bt/Mcu/I_Pin.hpp"
#include "Bt/Device/I_Rf24Device.hpp"

namespace Bt {
namespace Device {

class Rf24Device : public I_Rf24Device
{
   public:





      Rf24Device(Mcu::I_Spi& pSpi, Mcu::I_Pin& pChipEnable);
      virtual ~Rf24Device();

      virtual Status status();

      virtual void clearDataReady();
      virtual void clearDataSent();
      virtual void clearRetransmitsExceeded();

      virtual bool powerUp();
      virtual void powerUp(bool pValue);

      virtual TransceiverMode transceiverMode();
      virtual void transceiverMode(TransceiverMode pMode);

      virtual void chipEnable(bool pValue);

      virtual uint8_t autoRetransmitDelay();
      virtual void autoRetransmitDelay(uint8_t pDelay);

      virtual uint8_t autoRetransmitCount();
      virtual void autoRetransmitCount(uint8_t pCount);

      virtual uint8_t autoRetransmitCounter();

      virtual uint8_t channel();
      virtual void channel(uint8_t pChannel);

      virtual Address receiveAddress(Pipe pPipe);
      virtual void receiveAddress(Pipe pPipe, Address pAddress);

      virtual bool receivePipeEnabled(Pipe pPipe);
      virtual void receivePipeEnabled(Pipe pPipe, bool pValue);

      virtual uint8_t receivePayloadSize(Pipe pPipe);
      virtual void receivePayloadSize(Pipe pPipe, uint8_t pSize);

      virtual Address transmitAddress();
      virtual void transmitAddress(Address pAddress);

      virtual bool isTransmitFifoEmpty();
      virtual bool isTransmitFifoFull();
      virtual void flushTransmitFifo();

      virtual bool isReceiveFifoEmpty();
      virtual bool isReceiveFifoFull();

      virtual size_t writeTransmitPayload(uint8_t* pData, size_t pSize);
      virtual size_t readReceivePayload(Pipe& pPipe, uint8_t* pData, size_t pSize);

      virtual bool dynamicPayloadFeatureEnabled();
      virtual void dynamicPayloadFeatureEnabled(bool pValue);

      virtual bool dynamicPayloadEnabled(Pipe pPipe);
      virtual void dynamicPayloadEnabled(Pipe pPipe, bool pValue);


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
