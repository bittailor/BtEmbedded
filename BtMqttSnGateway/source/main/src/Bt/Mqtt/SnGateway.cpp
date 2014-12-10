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

SnGateway::SnGateway(const I_SnGatewaySettings& iSettings)
: mSettings(iSettings), mSocket(RfPacketSocketFactory().createPacketSocket(mSettings.rf24().chipEnablePin,
                                                                           mSettings.rf24().chipSelect,
                                                                           mSettings.rf24().irqPin,
                                                                           mSettings.rf24().channel,
                                                                           mSettings.socket().nodeId))
, mMqttFactory(std::make_shared<MqttFactory>(mSettings.broker().url,
                                             mSettings.broker().user,
                                             mSettings.broker().password)) {
   BT_LOG(DEBUG) << "SnGateway::SnGateway()";
}

//-------------------------------------------------------------------------------------------------

SnGateway::~SnGateway() {
   BT_LOG(DEBUG) << "SnGateway::~SnGateway()";
}

//-------------------------------------------------------------------------------------------------

int SnGateway::run() {
   mRunning.store(true);
   BT_LOG(DEBUG) << "SnGateway enter 'run' loop" ;
   while(mRunning.load()) {
      //BT_LOG(DEBUG) << "loop (" << mRunning << ")";
      uint8_t nodeId = 0;
      Net::MqttSn::MessageBuffer buffer(mSocket->payloadCapacity());
      int receivedSize = mSocket->receive(buffer.buffer(), buffer.bufferCapacity(), &nodeId);
      if (receivedSize > 0) {
         if (buffer.length() != receivedSize) {
            BT_LOG(ERROR) << "mismatching length (" << static_cast<int>(buffer.length()) <<") and received size (" << receivedSize << ") => drop message";
            continue;
         }

         auto message = buffer.parse();
         if (!message) {
            BT_LOG(WARNING) << "Could not parse message";
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

   BT_LOG(DEBUG) << "SnGateway end of 'run' loop" ;
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
