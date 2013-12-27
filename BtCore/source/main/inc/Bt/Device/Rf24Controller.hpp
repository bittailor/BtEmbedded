//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Device::Rf24Controller
//  
//*************************************************************************************************

#ifndef INC__Bt_Device_Rf24Controller__hpp
#define INC__Bt_Device_Rf24Controller__hpp

#include <stddef.h>
#include <stdint.h>

#include "Bt/Device/I_Rf24Device.hpp"

namespace Bt {
namespace Device {

class Rf24Controller 
{
   public:
      Rf24Controller(I_Rf24Device& pDevice);
      ~Rf24Controller();

      virtual size_t write(I_Rf24Device::Pipe pPipe, uint8_t* data, size_t size);

      virtual void startListening();
      virtual bool isDataAvailable();
      virtual size_t read(uint8_t* buffer, size_t size);
      virtual size_t read(uint8_t* buffer, size_t size, I_Rf24Device::Pipe& pPipe);



   private:

      class StateBase {
         public:
         StateBase(Rf24Controller& pController) : mController(&pController){}
         virtual ~StateBase(){}
         virtual void ApplyTo(StateBase& other)= 0;
         virtual void ToPowerDown(){};
         virtual void ToStandbyI(){};
         virtual void ToRxMode(){};
         virtual void ToTxMode(){};
         protected:
         Rf24Controller* mController;
      };

      class PowerDown : public StateBase {
         public:
            PowerDown(Rf24Controller& pController) : StateBase(pController){}
            virtual void ApplyTo(StateBase& other) {other.ToPowerDown();};
            virtual void ToStandbyI();
            virtual void ToRxMode();
            virtual void ToTxMode();
      };

      class StandbyI : public StateBase {
         public:
            StandbyI(Rf24Controller& pController) : StateBase(pController){}
            virtual void ApplyTo(StateBase& other) {other.ToStandbyI();};
            virtual void ToPowerDown();
            virtual void ToRxMode();
            virtual void ToTxMode();
      };

      class RxMode : public StateBase {
         public:
            RxMode(Rf24Controller& pController) : StateBase(pController){}
            virtual void ApplyTo(StateBase& other) {other.ToRxMode();};
            virtual void ToPowerDown();
            virtual void ToStandbyI();
            virtual void ToTxMode();
      };

      class TxMode : public StateBase {
         public:
            TxMode(Rf24Controller& pController) : StateBase(pController){}
            virtual void ApplyTo(StateBase& other) {other.ToTxMode();};
            virtual void ToPowerDown();
            virtual void ToStandbyI();
            virtual void ToRxMode();
      };


      // Constructor to prohibit copy construction.
      Rf24Controller(const Rf24Controller&);

      // Operator= to prohibit copy assignment
      Rf24Controller& operator=(const Rf24Controller&);


      I_Rf24Device* mDevice;
      PowerDown mPowerDown;
      StandbyI mStandbyI;
      RxMode mRxMode;
      TxMode mTxMode;
      StateBase* mCurrentState;




};

} // namespace Device
} // namespace Bt

#endif // INC__Bt_Device_Rf24Controller__hpp
