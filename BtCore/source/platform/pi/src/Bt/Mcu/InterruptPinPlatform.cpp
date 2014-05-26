//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mcu::InterruptPinPlatform
//  
//*************************************************************************************************

#include "Bt/Mcu/InterruptPinPlatform.hpp"

#include <string>
#include <fstream>
#include <thread>
#include <iostream>
#include <stdexcept>

#include <fcntl.h>
#include <errno.h>
#include <poll.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <Bt/Log/Logging.hpp>
#include <boost/lexical_cast.hpp>

#include <bcm2835.h>

#include "Bt/Mcu/GpioLibrary.hpp"
#include "Bt/Util/CollectionUtility.hpp"


namespace Bt {
namespace Mcu {

namespace {

const std::string GPIO_ROOT =  "/sys/class/gpio";

void writeToFile(const std::string& path, const std::string& value) {
   std::ofstream outputFile(path);
   outputFile << value;
   outputFile.close();
}

std::string edgeToString(I_InterruptPin::Edge iEdge) {
   switch(iEdge) {
      case I_InterruptPin::Edge::FALLING : return "falling";
      case I_InterruptPin::Edge::RISING : return "rising";
      case I_InterruptPin::Edge::BOTH : return "both";
   }
   throw std::invalid_argument("invalid interrupt edge " + boost::lexical_cast<std::string>(static_cast<int>(iEdge)));
}


#define CHECK_SUCCESS(call) \
   if (call < 0) {  \
      throw std::domain_error(#call);  \
   }

}


//-------------------------------------------------------------------------------------------------

InterruptPinPlatform::InterruptPinPlatform(uint8_t iPinId, I_InterruptPin::Edge iEdge)
: mPinId(iPinId), mEdge(iEdge), mEnabled(false) {

   GpioLibrary::ensureIsInitialized();
   bcm2835_gpio_set_pud(iPinId, BCM2835_GPIO_PUD_UP);

   std::string pin = boost::lexical_cast<std::string>(static_cast<int>(mPinId));

   writeToFile(GPIO_ROOT + "/export", pin);

   mPinPath = GPIO_ROOT + "/gpio" + pin;

   writeToFile(mPinPath + "/direction", "in");
   writeToFile(mPinPath + "/edge", edgeToString(iEdge));





}

//-------------------------------------------------------------------------------------------------

InterruptPinPlatform::~InterruptPinPlatform() {
   disable();
   std::string pin = boost::lexical_cast<std::string>(static_cast<int>(mPinId));
   writeToFile(GPIO_ROOT + "/unexport", pin);
}

//-------------------------------------------------------------------------------------------------

void InterruptPinPlatform::enable(std::function<void()> iInterruptHandler) {
   bool expected = false;
   if(!mEnabled.compare_exchange_strong(expected,true)) {
      throw std::domain_error("InterruptPin already enabled");
   }

   std::string pinValuePath = mPinPath + "/value";

   mPinFileHandle = open(pinValuePath.c_str(), O_RDWR);
   if(mPinFileHandle < 0) {
      auto message = std::string("could not open gpio file ") + pinValuePath + " " + strerror(errno);
      BT_LOG(WARNING) << message;
      throw std::domain_error(message);
   }

   CHECK_SUCCESS(pipe(mDisablePipe));

   int size;
   CHECK_SUCCESS(ioctl(mPinFileHandle, FIONREAD, &size));
   for (int i = 0 ; i < size ; ++i) {
      char c;
      read (mPinFileHandle, &c, 1);
   }

   mPollThread = std::thread(&InterruptPinPlatform::pollCycle, this, iInterruptHandler, mPinFileHandle, mDisablePipe[0]);
}

//-------------------------------------------------------------------------------------------------

void InterruptPinPlatform::disable() {
   bool expected = true;
   if(!mEnabled.compare_exchange_strong(expected,false)) {
      return;
   }

   if(mPollThread.joinable()) {
      const char* closeMessage = "close";
      write(mDisablePipe[1], closeMessage , strlen(closeMessage));
      CHECK_SUCCESS(close(mDisablePipe[1]));
      mPollThread.join();
      CHECK_SUCCESS(close(mDisablePipe[0]));
      CHECK_SUCCESS(close(mPinFileHandle));
   }
}

//-------------------------------------------------------------------------------------------------

void InterruptPinPlatform::pollCycle(std::function<void()> iInterruptHandler, int iPinFileHandle, int iPipeFileHandle) {
   while(mEnabled.load()) {
      if(waitForInterrupt(iPinFileHandle, iPipeFileHandle)) {
         iInterruptHandler();
      }
   }
}

//-------------------------------------------------------------------------------------------------

bool InterruptPinPlatform::waitForInterrupt(int iPinFileHandle, int iPipeFileHandle) {
   pollfd pollData[2] ;
   pollData[0].fd = iPinFileHandle;
   pollData[0].events = POLLPRI;
   pollData[1].fd = iPipeFileHandle;
   pollData[1].events = POLLIN;

   int result = poll(pollData, Util::sizeOfArray(pollData), -1);
   if(result > 0 && mEnabled.load()) {
      char c;
      read (iPinFileHandle, &c, 1);
      return true;
   }

   return false;
}

//-------------------------------------------------------------------------------------------------

} // namespace Mcu
} // namespace Bt
