//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::NRf24L01P
//  
//*************************************************************************************************

#ifndef INC__Bt_Device_NRf24L01P__hpp
#define INC__Bt_Device_NRf24L01P__hpp

#include "Bt/Mcu/I_Spi.hpp"
#include "Bt/Mcu/I_Pin.hpp"
#include "Bt/Device/I_DeviceNRf24L01P.hpp"

namespace Bt {
namespace Device {

class NRf24L01P : public I_DeviceNRf24L01P
{
   public:

      enum {PAYLOAD_SIZE = 32};



      NRf24L01P(Mcu::I_Spi& pSpi, Mcu::I_Pin& pChipEnable);
      virtual ~NRf24L01P();

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

      virtual uint8_t receivePayloadSize(Pipe pPipe);
      virtual void receivePayloadSize(Pipe pPipe, uint8_t pSize);

      virtual Address transmitAddress();
      virtual void transmitAddress(Address pAddress);

      virtual bool isTransmitFifoEmpty();
      virtual bool isTransmitFifoFull();

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
      NRf24L01P(const NRf24L01P&);

      // Operator= to prohibit copy assignment
      NRf24L01P& operator=(const NRf24L01P&);

      Mcu::I_Spi* mSpi;
      Mcu::I_Pin* mChipEnable;

};

} // namespace Device
} // namespace Bt

#endif // INC__Bt_Device_NRf24L01P__hpp
