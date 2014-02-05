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

#include <Bt/Util/Timing.hpp>
#include <Bt/CoreInitializer.hpp>
#include "Bt/Mcu/Pin.hpp"
#include "Bt/Mcu/Spi.hpp"
#include "Bt/Rf24/Rf24Device.hpp"
#include "Bt/Rf24/Rf24Controller.hpp"
#include "Bt/Rf24/RfNetworkSocket.hpp"
#include "Bt/Rf24/I_RfNetworkSocket.hpp"


#include "Bt/Workcycle/MainWorkcycle.hpp"

using Bt::Workcycle::MainWorkcycle;

//-------------------------------------------------------------------------------------------------

class PingClient : public Bt::Rf24::I_RfNetworkSocket::I_Listener  {
   public:

      PingClient(Bt::Rf24::I_RfNetworkSocket& pSocket, uint8_t pPingId, std::string pMessage)
      : mSocket(&pSocket), mPingId(pPingId), mMessage(pMessage), mCounter(0) {
      }

      void start() {
         Bt::Rf24::I_RfNetworkSocket::Packet packet;
         packet.destination(mPingId);
         packet.writePayload(&mCounter, sizeof(mCounter));
         printf("Send Packet %i => %i [%i] \n", mCounter , (int)packet.destination(), (int)packet.size());
         mStart = std::chrono::system_clock::now();
         mCounter++;
         mSocket->send(packet);
      }

      virtual void packetReceived(Bt::Rf24::I_RfNetworkSocket::Packet& pPacket) {
         std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
         std::cout << "Packet from " << (int)pPacket.source() << " => " << (int)pPacket.destination() << std::endl;

         std::cout << "Id: " << *(int*)pPacket.payload() << std::endl;

         std::cout << "Ping took "
                   << std::chrono::duration_cast<std::chrono::milliseconds>(end - mStart).count()
                   << "ms."
                   << std::endl << std::endl;

         start();
      }


   private:
      Bt::Rf24::I_RfNetworkSocket* mSocket;
      uint8_t mPingId;
      std::string mMessage;
      int mCounter;
      std::chrono::time_point<std::chrono::system_clock> mStart;
};

//-------------------------------------------------------------------------------------------------

#define CHIP_ENABLE 17
#define CHIP_SELECT 8

int main(int argc, const char* argv[]) {
   if (argc < 4) {
      return printf("Usage %s SelfNodeId PingNodeId Message \n", argv[0]);
   }

   uint8_t nodeId = atoi(argv[1]);
   uint8_t pingId = atoi(argv[2]);
   std::string message = argv[3];

   Bt::CoreInitializer coreInitializer;

   Bt::Mcu::Pin power(4, Bt::Mcu::I_Pin::MODE_OUTPUT);

   std::cout << "Toggle power ...";
   power.write(false);
   Bt::Util::delayInMilliseconds(100);
   std::cout << "...";
   power.write(true);
   Bt::Util::delayInMilliseconds(10);
   std::cout << "...OK" << std::endl;


   Bt::Mcu::Pin chipEnable(CHIP_ENABLE, Bt::Mcu::I_Pin::MODE_OUTPUT);
   Bt::Mcu::Pin chipSelect(CHIP_SELECT, Bt::Mcu::I_Pin::MODE_OUTPUT);
   Bt::Mcu::Spi spi(Bt::Mcu::I_Spi::BIT_ORDER_MSBFIRST,
                    Bt::Mcu::I_Spi::MODE_0,
                    Bt::Mcu::I_Spi::SPEED_8_MHZ,
                    chipSelect);

   Bt::Rf24::Rf24Device device(spi,chipEnable);
   Bt::Rf24::Rf24Controller controller(device);
   Bt::Rf24::RfNetworkSocket socket(nodeId, controller);

   PingClient pingClient(socket, pingId, message);

   socket.startListening(pingClient);

   pingClient.start();

   MainWorkcycle workcycle;
   workcycle.add(socket);

   printf("Enter ping client workcycle for node %i => node %i\n",(int) nodeId, (int) pingId);

   workcycle.run();

   return 0;
}

//-------------------------------------------------------------------------------------------------

