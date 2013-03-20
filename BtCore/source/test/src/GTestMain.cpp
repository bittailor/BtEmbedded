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

enum JustAnEnumForSizeTesting {
   A,
   B
};


int main(int argc, char **argv) {

   std::cout << "bool    " << sizeof(bool) << std::endl;
   std::cout << "uint8_t " << sizeof(uint8_t) << std::endl;
   std::cout << "int     " << sizeof(int) << std::endl;
   std::cout << "enum    " << sizeof(JustAnEnumForSizeTesting) << std::endl;


   ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

