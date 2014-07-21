//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  PingServer
//  
//*************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <signal.h>


#include <Bt/Util/Timing.hpp>
#include <Bt/CoreInitializer.hpp>
#include <Bt/Mcu/Pin.hpp>
#include <Bt/Mcu/Spi.hpp>
#include <Bt/Mcu/InterruptPin.hpp>
#include <Bt/Rf24/Rf24Device.hpp>
#include <Bt/Rf24/Rf24Controller.hpp>
#include <Bt/Rf24/RfNetworkSocket.hpp>
#include <Bt/Rf24/I_RfNetworkSocket.hpp>
#include <Bt/Concurrency/ExecutionContext.hpp>
#include <Bt/Concurrency/CountdownLatch.hpp>

//-------------------------------------------------------------------------------------------------

#define PIN_POWER 27
#define PIN_CHIP_ENABLE 25

//-------------------------------------------------------------------------------------------------

class PingServer {
   public:

      PingServer(Bt::Rf24::I_RfNetworkSocket& iSocket) : mSocket(iSocket) {
         mSocket.startListening(std::bind(&PingServer::packetReceived,this,std::placeholders::_1));
      }

      ~PingServer() {
         mSocket.stopListening();
      }

      virtual void packetReceived(Bt::Rf24::I_RfNetworkSocket::Packet& iPacket) {
         printf("Packet from %i => %i [%i]\n", (int)iPacket.source(), (int)iPacket.destination(), (int) iPacket.size());
         std::string message((char*)iPacket.payload(),(char*)iPacket.payload() + iPacket.size());
         std::cout << "Message: " << message << std::endl;
         Bt::Rf24::I_RfNetworkSocket::Packet packet;
         packet.destination(iPacket.source());
         packet.writePayload(iPacket.payload(),iPacket.size());
         mSocket.send(packet);
      }


   private:
      Bt::Rf24::I_RfNetworkSocket& mSocket;
};

//-------------------------------------------------------------------------------------------------

Bt::Concurrency::CountdownLatch sStopLatch(1);

//-------------------------------------------------------------------------------------------------

void signalCallbackHandler(int signum) {
   printf("\nstopping ...\n");
   sStopLatch.countDown();
}


//-------------------------------------------------------------------------------------------------

int main(int argc, const char* argv[]) {
   if (argc < 2) {
      return printf("usage %s NodeId \n", argv[0]);
   }
   signal(SIGINT, signalCallbackHandler);

   Bt::CoreInitializer coreInitializer;
   uint8_t nodeId = atoi(argv[1]);

   Bt::Mcu::Pin power(PIN_POWER, Bt::Mcu::I_Pin::MODE_OUTPUT);
   printf("power on\n");
   power.write(true);

   {
      Bt::Mcu::Pin chipEnable(PIN_CHIP_ENABLE, Bt::Mcu::I_Pin::MODE_OUTPUT);
      Bt::Mcu::Spi spi(Bt::Mcu::I_Spi::BIT_ORDER_MSBFIRST,
                       Bt::Mcu::I_Spi::MODE_0,
                       Bt::Mcu::I_Spi::SPEED_8_MHZ,
                       Bt::Mcu::I_Spi::CHIP_SELECT_0);
      Bt::Mcu::InterruptPin irq(24, Bt::Mcu::I_InterruptPin::Edge::FALLING);
      Bt::Concurrency::ExecutionContext executionContext;

      Bt::Rf24::Rf24Device device(spi,chipEnable);
      Bt::Rf24::Rf24DeviceController controller(device,irq,executionContext);
      Bt::Rf24::RfNetworkSocket socket(nodeId, controller);

      PingServer pingServer(socket);

      printf("ping server running for node %i\n",(int) nodeId);
      printf("wait for stop latch (CTR-C)\n");

      sStopLatch.wait();
      printf("... stopping ...\n");
   }

   printf("power off\n");
   power.write(false);
   return 0;
}

//-------------------------------------------------------------------------------------------------
