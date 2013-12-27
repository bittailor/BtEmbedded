//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::RfControllerNRf24L01P
//  
//*************************************************************************************************

#ifndef INC__Bt_Device_RfControllerNRf24L01P__hpp
#define INC__Bt_Device_RfControllerNRf24L01P__hpp

#include <stddef.h>
#include <stdint.h>

#include "Bt/Device/I_DeviceNRf24L01P.hpp"

namespace Bt {
namespace Device {

class RfControllerNRf24L01P 
{
   public:
      RfControllerNRf24L01P(I_DeviceNRf24L01P& pDevice);
      ~RfControllerNRf24L01P();

      virtual size_t write(I_DeviceNRf24L01P::Pipe pPipe, uint8_t* data, size_t size);

      virtual void startListening();
      virtual bool isDataAvailable();
      virtual size_t read(uint8_t* buffer, size_t size);
      virtual size_t read(uint8_t* buffer, size_t size, I_DeviceNRf24L01P::Pipe& pPipe);



   private:

      class StateBase {
         public:
         StateBase(RfControllerNRf24L01P& pController) : mController(&pController){}
         virtual ~StateBase(){}
         virtual void ApplyTo(StateBase& other)= 0;
         virtual void ToPowerDown(){};
         virtual void ToStandbyI(){};
         virtual void ToRxMode(){};
         virtual void ToTxMode(){};
         protected:
         RfControllerNRf24L01P* mController;
      };

      class PowerDown : public StateBase {
         public:
            PowerDown(RfControllerNRf24L01P& pController) : StateBase(pController){}
            virtual void ApplyTo(StateBase& other) {other.ToPowerDown();};
            virtual void ToStandbyI();
            virtual void ToRxMode();
            virtual void ToTxMode();
      };

      class StandbyI : public StateBase {
         public:
            StandbyI(RfControllerNRf24L01P& pController) : StateBase(pController){}
            virtual void ApplyTo(StateBase& other) {other.ToStandbyI();};
            virtual void ToPowerDown();
            virtual void ToRxMode();
            virtual void ToTxMode();
      };

      class RxMode : public StateBase {
         public:
            RxMode(RfControllerNRf24L01P& pController) : StateBase(pController){}
            virtual void ApplyTo(StateBase& other) {other.ToRxMode();};
            virtual void ToPowerDown();
            virtual void ToStandbyI();
            virtual void ToTxMode();
      };

      class TxMode : public StateBase {
         public:
            TxMode(RfControllerNRf24L01P& pController) : StateBase(pController){}
            virtual void ApplyTo(StateBase& other) {other.ToTxMode();};
            virtual void ToPowerDown();
            virtual void ToStandbyI();
            virtual void ToRxMode();
      };


      // Constructor to prohibit copy construction.
      RfControllerNRf24L01P(const RfControllerNRf24L01P&);

      // Operator= to prohibit copy assignment
      RfControllerNRf24L01P& operator=(const RfControllerNRf24L01P&);


      I_DeviceNRf24L01P* mDevice;
      PowerDown mPowerDown;
      StandbyI mStandbyI;
      RxMode mRxMode;
      TxMode mTxMode;
      StateBase* mCurrentState;




};

} // namespace Device
} // namespace Bt

#endif // INC__Bt_Device_RfControllerNRf24L01P__hpp
