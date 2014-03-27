//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::SnGateway
//  
//*************************************************************************************************

#include "Bt/Mqtt/SnGateway.hpp"

#include <iostream>
#include <vector>

#include <Bt/Rf24/RfPacketSocketFactory.hpp>
#include <Bt/Net/Mqtt/MqttSn/Message.hpp>

#include "Bt/Mqtt/Client.hpp"

using Bt::Net::Mqtt::MqttSn::MessageBuffer;
using Bt::Rf24::RfPacketSocketFactory;


namespace Bt {
namespace Mqtt {


//-------------------------------------------------------------------------------------------------

SnGateway::SnGateway() : mSocket(RfPacketSocketFactory().createPacketSocket(17,8,0)) {
   std::cout << "SnGateway::SnGateway()" << std::endl;
}

//-------------------------------------------------------------------------------------------------

SnGateway::~SnGateway() {
   std::cout << "SnGateway::~SnGateway()" << std::endl;
}

//-------------------------------------------------------------------------------------------------

int SnGateway::run() {
   mRunning = true;

   while(mRunning) {
      std::cout << "Run Loop" << mRunning << std::endl;
      uint8_t nodeId = 0;
      //std::cout << "mSocket->payloadCapacity() = " << mSocket->payloadCapacity() << std::endl;
      Net::Mqtt::MqttSn::MessageBuffer buffer(mSocket->payloadCapacity());
      //std::cout << "message.buffer() = 0x" <<  std::hex << reinterpret_cast<uint64_t>(buffer.buffer()) << std::dec << std::endl;
      //std::cout << "message.bufferCapacity() ="  << buffer.bufferCapacity() << std::endl;


      int receivedSize = mSocket->receive(buffer.buffer(), buffer.bufferCapacity(), &nodeId);
      if (receivedSize > 0) {
         if (buffer.length() != receivedSize) {
            std::cout << "Missmatching length (" << buffer.length() <<") and received size (" << receivedSize << ")" <<  std::endl;
            continue;
         }

         std::cout << "raw message : " << buffer <<  std::endl;

         auto message = buffer.parse();
         if (!message) {
            std::cout << "Could not parse message" <<  std::endl;
            continue;
         }
         std::shared_ptr<Client> client(new Client(nodeId,mSocket));
                     client->handle(message);

      }
   }

   std::cout << "SnGateway end of run loop" << std::endl;

   return 0;
}

//-------------------------------------------------------------------------------------------------

void SnGateway::stop() {
   std::cout << "SnGateway::stop" << std::endl;
   mRunning = false;
}

//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Bt
