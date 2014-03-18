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

class PingServer : public Bt::Rf24::I_RfNetworkSocket::I_Listener  {
   public:

      PingServer(Bt::Rf24::I_RfNetworkSocket& pSocket) : mSocket(&pSocket) {

      }

      virtual void packetReceived(Bt::Rf24::I_RfNetworkSocket::Packet& pPacket) {
         printf("Packet from %i => %i [%i]\n", (int)pPacket.source(), (int)pPacket.destination(), (int) pPacket.size());
         Bt::Rf24::I_RfNetworkSocket::Packet packet;
         packet.destination(pPacket.source());
         packet.writePayload(pPacket.payload(),pPacket.size());
         mSocket->send(packet);
      }


   private:
      Bt::Rf24::I_RfNetworkSocket* mSocket;
};

//-------------------------------------------------------------------------------------------------

#ifdef BT_PF_AVR

#include <EEPROM.h>
#include <Arduino.h>

#define CHIP_ENABLE 9
#define CHIP_SELECT 10

//void setNodeId() {
//   printf("Enter node id \n");
//   while(Serial.available() <= 0){}
//   int size = Serial.available();
//   char idString[size + 1];
//   Serial.readBytes(idString,size);
//   idString[size] = 0;
//   uint8_t nodeId = atoi(idString);
//   printf("Enter node id set to %i \n",(int)nodeId);
//   EEPROM.write(0,nodeId);
//}


int main() {
   Bt::CoreInitializer coreInitializer;

//   if(EEPROM.read(0) == 255) {
//      setNodeId();
//   }

   uint8_t nodeId = 12;

#else
#define CHIP_ENABLE 17
#define CHIP_SELECT 8

int main(int argc, const char* argv[]) {
   Bt::CoreInitializer coreInitializer;

   if (argc < 2) {
      return printf("Usage %s NodeId \n", argv[0]);
   }

   uint8_t nodeId = atoi(argv[1]);


#endif



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

   socket.setListener(pingServer);

   MainWorkcycle workcycle;
   workcycle.add(socket);


   printf("Enter ping server workcycle for node %i\n",(int) nodeId);
   workcycle.run();
   return 0;
}

//-------------------------------------------------------------------------------------------------

