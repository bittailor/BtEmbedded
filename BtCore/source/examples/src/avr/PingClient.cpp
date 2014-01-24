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

#include <Bt/Util/Delay.hpp>
#include <Bt/CoreInitializer.hpp>

#include "Bt/Mcu/Pin.hpp"
#include "Bt/Mcu/Spi.hpp"
#include "Bt/Rf24/Rf24Device.hpp"
#include "Bt/Rf24/Rf24Controller.hpp"
#include "Bt/Rf24/RfNetworkSocket.hpp"
#include "Bt/Rf24/I_RfNetworkSocket.hpp"

//-------------------------------------------------------------------------------------------------

class PingServer : public Bt::Rf24::I_RfNetworkSocket::I_Listener  {
   public:

      PingServer(Bt::Rf24::I_RfNetworkSocket& pSocket) : mSocket(&pSocket) {

      }

      virtual void packetReceived(Bt::Rf24::I_RfNetworkSocket::Packet& pPacket) {
         printf("Packet from %i => %i \n", (int)pPacket.source(), (int)pPacket.destination());
         Bt::Rf24::I_RfNetworkSocket::Packet packet;
         packet.destination(pPacket.source());
         mSocket->send(packet);
      }


   private:
      Bt::Rf24::I_RfNetworkSocket* mSocket;
};

//-------------------------------------------------------------------------------------------------

#ifdef BT_PF_AVR

#define CHIP_ENABLE 9
#define CHIP_SELECT 10

int main() {
   uint8_t nodeId = 2;

#else
#define CHIP_ENABLE 17
#define CHIP_SELECT 8

int main(int argc, const char* argv[]) {
   if (argc < 2) {
      return printf("Usage %s NodeId \n", argv[0]);
   }

   uint8_t nodeId = atoi(argv[1]);


#endif



   Bt::CoreInitializer coreInitializer;

   Bt::Mcu::Pin chipEnable(CHIP_ENABLE, Bt::Mcu::I_Pin::MODE_OUTPUT);
   Bt::Mcu::Pin chipSelect(CHIP_SELECT, Bt::Mcu::I_Pin::MODE_OUTPUT);
   Bt::Mcu::Spi spi(Bt::Mcu::I_Spi::BIT_ORDER_MSBFIRST,
                    Bt::Mcu::I_Spi::MODE_0,
                    Bt::Mcu::I_Spi::SPEED_8_MHZ,
                    chipSelect);

   Bt::Rf24::Rf24Device device(spi,chipEnable);
   Bt::Rf24::Rf24Controller controller(device);
   Bt::Rf24::RfNetworkSocket socket(nodeId, controller);

   PingServer pingServer(socket);

   socket.startListening(pingServer);

   printf("Enter ping server workcycle for node %i\n",(int) nodeId);
   while(true) {
      socket.workcycle();
   }
   return 0;
}

//-------------------------------------------------------------------------------------------------

