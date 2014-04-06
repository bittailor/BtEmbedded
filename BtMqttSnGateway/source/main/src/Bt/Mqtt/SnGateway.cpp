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
#include <Bt/Net/MqttSn/Message.hpp>

#include "Bt/Mqtt/GatewayConnection.hpp"
#include "Bt/Mqtt/MqttFactory.hpp"

using Bt::Net::MqttSn::MessageBuffer;
using Bt::Rf24::RfPacketSocketFactory;


namespace Bt {
namespace Mqtt {


//-------------------------------------------------------------------------------------------------

SnGateway::SnGateway(const std::string& iAddress, const std::string& iUser, const std::string& iPassword)
: mSocket(RfPacketSocketFactory().createPacketSocket(17,8,0)), mMqttFactory(std::make_shared<MqttFactory>(iAddress, iUser, iPassword)) {
   std::cout << "SnGateway::SnGateway()" << std::endl;
}

//-------------------------------------------------------------------------------------------------

SnGateway::~SnGateway() {
   std::cout << "SnGateway::~SnGateway()" << std::endl;
}

//-------------------------------------------------------------------------------------------------

int SnGateway::run() {
   mRunning = true;
   std::cout << "enter loop " << std::endl;
   while(mRunning) {
      //std::cout << "loop (" << mRunning << ")"<< std::endl;
      uint8_t nodeId = 0;
      Net::MqttSn::MessageBuffer buffer(mSocket->payloadCapacity());
      int receivedSize = mSocket->receive(buffer.buffer(), buffer.bufferCapacity(), &nodeId);
      if (receivedSize > 0) {
         if (buffer.length() != receivedSize) {
            std::cout << "Missmatching length (" << buffer.length() <<") and received size (" << receivedSize << ")" <<  std::endl;
            continue;
         }

         //std::cout << "Raw message : " << buffer <<  std::endl;

         auto message = buffer.parse();
         if (!message) {
            std::cout << "Could not parse message" <<  std::endl;
            continue;
         }

         auto gatewayConnection = mConnections.lookup(nodeId);
         if(!gatewayConnection) {
            std::cout << "Create new gateway connection for node " << nodeId <<  std::endl;
            gatewayConnection = std::make_shared<GatewayConnection>(nodeId,
                                                                    mSocket,
                                                                    mMqttFactory,
                                                                    std::bind(&Bt::Util::Repository<GatewayConnection>::remove, &mConnections, std::placeholders::_1));
            mConnections.add(gatewayConnection);
         }
         gatewayConnection->handle(message);
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
