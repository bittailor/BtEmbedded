//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::Rf24Controller
//  
//*************************************************************************************************

#ifndef INC__Bt_Device_Rf24Controller__hpp
#define INC__Bt_Device_Rf24Controller__hpp

#include <random>
#include <vector>
#include <stddef.h>
#include <stdint.h>

#include <Bt/Concurrency/I_ExecutionContext.hpp>
#include <Bt/Rf24/I_Rf24Controller.hpp>
#include <Bt/Rf24/I_Rf24Device.hpp>
#include <Bt/Mcu/InterruptPin.hpp>
#include <Bt/Mcu/Pin.hpp>

namespace Bt {
namespace Rf24 {

class Rf24DeviceController : public I_Rf24DeviceController
{
   public:

      Rf24DeviceController(I_Rf24Device& iDevice, Mcu::I_InterruptPin& iInterruptPin, uint8_t iChannel,
                           Concurrency::I_ExecutionContext& iExecutionContext);
      ~Rf24DeviceController();

      virtual void configure(const Configuration& iConfiguration);

      virtual bool write(RfPipe iPipe, Packet& iPacket);

      virtual void startListening(std::function<void(RfPipe iPipe, Packet& iPacket)> iCallback);
      virtual void stopListening();

      virtual void reboot();

   private:

      enum class InterruptState {
         Ignore,
         Rx,
         Tx
      };

      class StateBase {
         public:
         StateBase(Rf24DeviceController& iController) : mController(&iController){}
         virtual ~StateBase(){}
         virtual void ApplyTo(StateBase& other)= 0;
         virtual void ToPowerDown(){};
         virtual void ToStandbyI(){};
         virtual void ToRxMode(){};
         virtual void ToTxMode(){};
         protected:
         Rf24DeviceController* mController;
      };

      class PowerDown : public StateBase {
         public:
            PowerDown(Rf24DeviceController& iController) : StateBase(iController){}
            virtual void ApplyTo(StateBase& other) {other.ToPowerDown();};
            virtual void ToStandbyI();
            virtual void ToRxMode();
            virtual void ToTxMode();
      };

      class StandbyI : public StateBase {
         public:
            StandbyI(Rf24DeviceController& iController) : StateBase(iController){}
            virtual void ApplyTo(StateBase& other) {other.ToStandbyI();};
            virtual void ToPowerDown();
            virtual void ToRxMode();
            virtual void ToTxMode();
      };

      class RxMode : public StateBase {
         public:
            RxMode(Rf24DeviceController& iController) : StateBase(iController){}
            virtual void ApplyTo(StateBase& other) {other.ToRxMode();};
            virtual void ToPowerDown();
            virtual void ToStandbyI();
            virtual void ToTxMode();
      };

      class TxMode : public StateBase {
         public:
            TxMode(Rf24DeviceController& iController) : StateBase(iController){}
            virtual void ApplyTo(StateBase& other) {other.ToTxMode();};
            virtual void ToPowerDown();
            virtual void ToStandbyI();
            virtual void ToRxMode();
      };


      // Constructor to prohibit copy construction.
      Rf24DeviceController(const Rf24DeviceController&);

      // Operator= to prohibit copy assignment
      Rf24DeviceController& operator=(const Rf24DeviceController&);


      void configureDevice();
      size_t transmitPacket(RfPipe iPipe, Packet& iPacket);
      void onInterrupt();
      void readReceiveData();
      void handleReceiveData(RfPipe iPipe, Packet& iPacket);

      I_Rf24Device& mDevice;
      Mcu::I_InterruptPin& mInterruptPin;
      uint8_t mChannel;
      Concurrency::I_ExecutionContext& mExecutionContext;
      Configuration mConfiguration;
      std::function<void(RfPipe iPipe, Packet& iPacket)> mCallback;
      PowerDown mPowerDown;
      StandbyI mStandbyI;
      RxMode mRxMode;
      TxMode mTxMode;
      StateBase* mCurrentState;
      std::atomic<InterruptState> mInterruptState;


#include <random>



};

} // namespace Device
} // namespace Bt

#endif // INC__Bt_Device_Rf24Controller__hpp
