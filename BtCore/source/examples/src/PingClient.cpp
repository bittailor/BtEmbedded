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
#include <chrono>
#include <iostream>
#include <thread>
#include <signal.h>
#include <sstream>

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
#define RF_CHANNEL 10

//-------------------------------------------------------------------------------------------------

class PingClient  {
   public:

      PingClient(Bt::Rf24::I_RfNetworkSocket& iSocket, uint8_t iPingId, int iDelay, std::string iMessage)
      : mSocket(iSocket), mPingId(iPingId), mDelay(iDelay), mMessage(iMessage), mCounter(0) {
         mSocket.startListening(std::bind(&PingClient::packetReceived,this,std::placeholders::_1));
      }

      ~PingClient() {
         mSocket.stopListening();
      }

      void start() {
         Bt::Rf24::I_RfNetworkSocket::Packet packet;
         packet.destination(mPingId);
         std::stringstream message;
         message << "M-" << mCounter;
         packet.writePayload(message.str().c_str(), message.str().size());
         printf("Send Packet %i => %i [%i] \n", mCounter , (int)packet.destination(), (int)packet.size());
         mStart = std::chrono::system_clock::now();
         mCounter++;
         mSocket.send(packet);
      }

      void packetReceived(Bt::Rf24::I_RfNetworkSocket::Packet& iPacket) {
         std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
         std::cout << "Packet from " << (int)iPacket.source() << " => " << (int)iPacket.destination() << std::endl;
         std::string message((char*)iPacket.payload(),(char*)iPacket.payload() + iPacket.size());
         std::cout << "Message: " << message << std::endl;

         std::cout << "Ping took "
                   << std::chrono::duration_cast<std::chrono::milliseconds>(end - mStart).count()
                   << "ms."
                   << std::endl << std::endl;


         std::this_thread::sleep_for(std::chrono::milliseconds(mDelay));
         start();
      }

   private:
      Bt::Rf24::I_RfNetworkSocket& mSocket;
      uint8_t mPingId;
      int mDelay;
      std::string mMessage;
      int mCounter;
      std::chrono::time_point<std::chrono::system_clock> mStart;
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
   if (argc < 5) {
      return printf("Usage %s SelfNodeId PingNodeId Delay Message \n", argv[0]);
   }
   signal(SIGINT, signalCallbackHandler);

   uint8_t nodeId = atoi(argv[1]);
   uint8_t pingId = atoi(argv[2]);
   int delay = atoi(argv[3]);
   std::string message = argv[4];

   Bt::CoreInitializer coreInitializer;

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
      Bt::Rf24::Rf24DeviceController controller(device,irq,RF_CHANNEL,executionContext);
      Bt::Rf24::RfNetworkSocket socket(nodeId, controller);

      PingClient pingClient(socket, pingId, delay, message);
      pingClient.start();


      printf("ping client form node %i to node %i\n",(int) nodeId, (int)pingId);
      printf("wait for stop latch (CTR-C)\n");

      sStopLatch.wait();
      printf("... stopping ...\n");
   }

   printf("power off\n");
   power.write(false);
   return 0;
}

//-------------------------------------------------------------------------------------------------

