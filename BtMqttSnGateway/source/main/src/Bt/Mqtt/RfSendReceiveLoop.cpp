//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::RfSendReceiveLoop
//  
//*************************************************************************************************

#include "Bt/Mqtt/RfSendReceiveLoop.hpp"

#include <thread>

namespace Bt {
namespace Mqtt {


//-------------------------------------------------------------------------------------------------

RfSendReceiveLoop::RfSendReceiveLoop() : mRunning(false) {

}

//-------------------------------------------------------------------------------------------------

RfSendReceiveLoop::~RfSendReceiveLoop() {

}

//-------------------------------------------------------------------------------------------------

bool RfSendReceiveLoop::start() {
   bool expected = false;
   if(!mRunning.compare_exchange_strong(expected,true)) {
      return false;
   }
   mThread = std::thread(&RfSendReceiveLoop::run, this);
   return true;
}

//-------------------------------------------------------------------------------------------------

void RfSendReceiveLoop::run() {
   while(mRunning.load()){
      workcycle();
   }
}

//-------------------------------------------------------------------------------------------------

void RfSendReceiveLoop::workcycle() {
   std::function<void()> action;
   //if (mQueue.pop())

}

//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Bt
