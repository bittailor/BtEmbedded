//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Main of BtCore test suite
//  
//*************************************************************************************************

#include <iostream>
#include <gtest/gtest.h>

#include <sched.h>
#include <pthread.h>
#include <string.h>

enum JustAnEnumForSizeTesting {
   A,
   B
};


int main(int argc, char **argv) {

   std::cout << "bool    " << sizeof(bool) << std::endl;
   std::cout << "uint8_t " << sizeof(uint8_t) << std::endl;
   std::cout << "int     " << sizeof(int) << std::endl;
   std::cout << "enum    " << sizeof(JustAnEnumForSizeTesting) << std::endl;

   struct sched_param sched ;
   memset (&sched, 0, sizeof(sched)) ;
   std::cout << "sched_getscheduler " <<  sched_getscheduler (0) << std::endl;
   std::cout << "sched_getparam " << sched_getparam(0,&sched) << std::endl;
   std::cout << "sched.sched_priority " << sched.sched_priority << std::endl;

   memset (&sched, 0, sizeof(sched)) ;
   int pri = 99;
   std::cout << "sched_get_priority_max (SCHED_RR)    " << sched_get_priority_max (SCHED_RR) << std::endl;
   std::cout << "sched_get_priority_min (SCHED_RR)    " << sched_get_priority_min (SCHED_RR) << std::endl;

   if (pri > sched_get_priority_max (SCHED_RR))
      sched.sched_priority = sched_get_priority_max (SCHED_RR) ;
   else
      sched.sched_priority = pri ;

   std::cout << "sched_setscheduler " <<  sched_setscheduler (0, SCHED_RR, &sched) << std::endl;
   std::cout << "sched_getscheduler " <<  sched_getscheduler (0) << std::endl;

   memset (&sched, 0, sizeof(sched)) ;
   std::cout << "sched_getparam " << sched_getparam(0,&sched) << std::endl;
   std::cout << "sched.sched_priority " << sched.sched_priority << std::endl;



   ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

