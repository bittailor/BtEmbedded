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

#include <boost/log/trivial.hpp>

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
: mSocket(RfPacketSocketFactory().createPacketSocket(17,8,0))
//: mSocket(RfPacketSocketFactory().createPacketSocket(24,7,0))
, mMqttFactory(std::make_shared<MqttFactory>(iAddress, iUser, iPassword)) {
   BOOST_LOG_TRIVIAL(debug) << "SnGateway::SnGateway()";
}

//-------------------------------------------------------------------------------------------------

SnGateway::~SnGateway() {
   BOOST_LOG_TRIVIAL(debug) << "SnGateway::~SnGateway()";
}

//-------------------------------------------------------------------------------------------------

int SnGateway::run() {
   mRunning = true;
   BOOST_LOG_TRIVIAL(debug) << "enter loop " ;
   while(mRunning) {
      //BOOST_LOG_TRIVIAL(debug) << "loop (" << mRunning << ")";
      uint8_t nodeId = 0;
      Net::MqttSn::MessageBuffer buffer(mSocket->payloadCapacity());
      int receivedSize = mSocket->receive(buffer.buffer(), buffer.bufferCapacity(), &nodeId);
      if (receivedSize > 0) {
         if (buffer.length() != receivedSize) {
            BOOST_LOG_TRIVIAL(debug) << "Missmatching length (" << buffer.length() <<") and received size (" << receivedSize << ")" ;
            continue;
         }

         //BOOST_LOG_TRIVIAL(debug) << "Raw message : " << buffer ;

         auto message = buffer.parse();
         if (!message) {
            BOOST_LOG_TRIVIAL(debug) << "Could not parse message" ;
            continue;
         }

         auto gatewayConnection = mConnections.lookup(nodeId);
         if(!gatewayConnection) {
            BOOST_LOG_TRIVIAL(debug) << "Create new gateway connection for node " << nodeId ;
            gatewayConnection = std::make_shared<GatewayConnection>(nodeId,
                                                                    mSocket,
                                                                    mMqttFactory,
                                                                    std::bind(&Bt::Util::Repository<GatewayConnection>::remove, &mConnections, std::placeholders::_1));
            mConnections.add(gatewayConnection);
         }
         gatewayConnection->handle(message);
      }
   }

   BOOST_LOG_TRIVIAL(debug) << "SnGateway end of run loop" ;

   return 0;
}

//-------------------------------------------------------------------------------------------------

void SnGateway::stop() {
   BOOST_LOG_TRIVIAL(debug) << "SnGateway::stop" ;
   mRunning = false;
}

//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Bt
