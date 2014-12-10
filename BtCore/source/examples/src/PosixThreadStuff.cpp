//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  MqttClientPublish
//  
//*************************************************************************************************

#include <iostream>
#include <pthread.h>

int main(int argc, const char* argv[]) {
   std::cout << "hello" << std::endl;

   std::cout << "PTHREAD_MUTEX_NORMAL = " << PTHREAD_MUTEX_NORMAL << std::endl;
   std::cout << "PTHREAD_MUTEX_ERRORCHECK = " << PTHREAD_MUTEX_ERRORCHECK << std::endl;
   std::cout << "PTHREAD_MUTEX_RECURSIVE = " << PTHREAD_MUTEX_RECURSIVE << std::endl;
   std::cout << "PTHREAD_MUTEX_DEFAULT = " << PTHREAD_MUTEX_DEFAULT << std::endl;


   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
   pthread_mutexattr_t attr;
   int rc;

   pthread_mutexattr_init(&attr);
   pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
   if ((rc = pthread_mutex_init(&mutex, &attr)) != 0) {
      std::cout <<"error"  <<  rc << "initializing client_mutex" << std::endl;
   }


   std::cout <<"First Lock "  << pthread_mutex_lock(&mutex) << std::endl;
   std::cout <<"Second Lock "  << pthread_mutex_lock(&mutex) << std::endl;

   std::cout <<"First Unlock "  << pthread_mutex_unlock(&mutex) << std::endl;
   std::cout <<"Second Unlock "  << pthread_mutex_unlock(&mutex) << std::endl;

   std::cout << "bye" << std::endl;
   return 0;
}


//-------------------------------------------------------------------------------------------------

