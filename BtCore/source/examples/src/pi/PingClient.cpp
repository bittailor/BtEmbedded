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


#include <Bt/Util/Delay.hpp>
#include <Bt/CoreInitializer.hpp>

#include "Bt/Mcu/Pin.hpp"
#include "Bt/Mcu/Spi.hpp"
#include "Bt/Rf24/Rf24Device.hpp"
#include "Bt/Rf24/Rf24Controller.hpp"
#include "Bt/Rf24/RfNetworkSocket.hpp"
#include "Bt/Rf24/I_RfNetworkSocket.hpp"
#include <iostream>

//-------------------------------------------------------------------------------------------------

class PingClient : public Bt::Rf24::I_RfNetworkSocket::I_Listener  {
   public:

      PingClient(Bt::Rf24::I_RfNetworkSocket& pSocket, uint8_t pPingId)
      : mSocket(&pSocket), mPingId(pPingId) {

      }

      void start() {
         Bt::Rf24::I_RfNetworkSocket::Packet packet;
         packet.destination(mPingId);
         std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
         packet.writePayload((uint8_t*)&start, sizeof(start));
         printf("Send Packet => %i [%i] \n", (int)packet.destination(), (int)packet.size());
         mSocket->send(packet);
      }

      virtual void packetReceived(Bt::Rf24::I_RfNetworkSocket::Packet& pPacket) {
         printf("Packet from %i => %i \n", (int)pPacket.source(), (int)pPacket.destination());
         std::chrono::time_point<std::chrono::system_clock> start;
         std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
         memcpy(&end,pPacket.payload(),pPacket.size());
         std::cout << "Printing took "
                   << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                   << "ms."
                   << std::endl;
      }


   private:
      Bt::Rf24::I_RfNetworkSocket* mSocket;
      uint8_t mPingId;
};

//-------------------------------------------------------------------------------------------------

#define CHIP_ENABLE 17
#define CHIP_SELECT 8

int main(int argc, const char* argv[]) {
   if (argc < 3) {
      return printf("Usage %s SelfNodeId PingNodeId \n", argv[0]);
   }

   uint8_t nodeId = atoi(argv[1]);
   uint8_t pingId = atoi(argv[2]);

   Bt::CoreInitializer coreInitializer;

   Bt::Mcu::Pin power(4, Bt::Mcu::I_Pin::MODE_OUTPUT);

   power.write(true);

   Bt::Mcu::Pin chipEnable(CHIP_ENABLE, Bt::Mcu::I_Pin::MODE_OUTPUT);
   Bt::Mcu::Pin chipSelect(CHIP_SELECT, Bt::Mcu::I_Pin::MODE_OUTPUT);
   Bt::Mcu::Spi spi(Bt::Mcu::I_Spi::BIT_ORDER_MSBFIRST,
                    Bt::Mcu::I_Spi::MODE_0,
                    Bt::Mcu::I_Spi::SPEED_8_MHZ,
                    chipSelect);

   Bt::Rf24::Rf24Device device(spi,chipEnable);
   Bt::Rf24::Rf24Controller controller(device);
   Bt::Rf24::RfNetworkSocket socket(nodeId, controller);

   PingClient pingClient(socket,pingId);

   socket.startListening(pingClient);

   pingClient.start();

   printf("Enter ping client workcycle for node %i => node %i\n",(int) nodeId, (int) pingId);

   while(true) {
      socket.workcycle();
   }

   return 0;
}

//-------------------------------------------------------------------------------------------------

