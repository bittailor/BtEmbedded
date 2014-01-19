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

#include <stddef.h>
#include <stdint.h>

#include <Bt/Rf24/I_Rf24Controller.hpp>
#include "Bt/Rf24/I_Rf24Device.hpp"

namespace Bt {
namespace Rf24 {

class Rf24Controller : public I_Rf24Controller
{
   public:

      Rf24Controller(I_Rf24Device& pDevice);
      ~Rf24Controller();

      virtual void configure(const Configuration& pConfiguration);

      virtual bool write(RfPipe pPipe, Packet pPacket);

      virtual size_t write(RfPipe pPipe, uint8_t* pData, size_t pSize);

      virtual void startListening();
      virtual void stopListening();
      virtual bool isDataAvailable();
      virtual bool read(Packet pPacket);
      virtual bool read(Packet pPacket, RfPipe& pPipe);


      virtual size_t read(uint8_t* pBuffer, size_t pSize);
      virtual size_t read(uint8_t* pBuffer, size_t pSize, RfPipe& pPipe);




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

      void configureDevice();

      Configuration mConfiguration;
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