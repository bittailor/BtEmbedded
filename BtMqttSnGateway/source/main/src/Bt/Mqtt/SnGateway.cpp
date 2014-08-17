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

#include <Bt/Log/Logging.hpp>

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
: mSocket(RfPacketSocketFactory().createPacketSocket(25,Mcu::I_Spi::CHIP_SELECT_0,24,0))
//: mSocket(RfPacketSocketFactory().createPacketSocket(24,7,0))
, mMqttFactory(std::make_shared<MqttFactory>(iAddress, iUser, iPassword)) {
   BT_LOG(DEBUG) << "SnGateway::SnGateway()";
}

//-------------------------------------------------------------------------------------------------

SnGateway::~SnGateway() {
   BT_LOG(DEBUG) << "SnGateway::~SnGateway()";
}

//-------------------------------------------------------------------------------------------------

int SnGateway::run() {
   mRunning.store(true);
   BT_LOG(DEBUG) << "enter loop " ;
   while(mRunning.load()) {
      //BT_LOG(DEBUG) << "loop (" << mRunning << ")";
      uint8_t nodeId = 0;
      Net::MqttSn::MessageBuffer buffer(mSocket->payloadCapacity());
      int receivedSize = mSocket->receive(buffer.buffer(), buffer.bufferCapacity(), &nodeId);
      if (receivedSize > 0) {
         if (buffer.length() != receivedSize) {
            BT_LOG(DEBUG) << "Missmatching length (" << buffer.length() <<") and received size (" << receivedSize << ")" ;
            continue;
         }

         //BT_LOG(DEBUG) << "Raw message : " << buffer ;

         auto message = buffer.parse();
         if (!message) {
            BT_LOG(DEBUG) << "Could not parse message" ;
            continue;
         }

         auto gatewayConnection = mConnections.lookup(nodeId);
         if(!gatewayConnection) {
            BT_LOG(DEBUG) << "Create new gateway connection for node " << nodeId ;
            gatewayConnection = std::make_shared<GatewayConnection>(nodeId,
                                                                    mSocket,
                                                                    mMqttFactory,
                                                                    [=](int id){
               this->mWorker.invoke([=](){
                  this->mConnections.remove(id);
               });
            });



            mConnections.add(gatewayConnection);
         }
         gatewayConnection->handle(message);
      }
   }

   BT_LOG(DEBUG) << "SnGateway end of run loop" ;

   return 0;
}

//-------------------------------------------------------------------------------------------------

void SnGateway::stop() {
   BT_LOG(DEBUG) << "SnGateway::stop" ;
   mRunning.store(false);
   mSocket->close();
}

//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Bt
