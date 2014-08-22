//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::DemanglingTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Util/Demangling.hpp"

namespace Bt {
namespace Util {

namespace {
   enum JustAnEnum {
      JustAValue
   };

   enum class JustAnEnumClass {
      JustAValue
   };

   class JustAnClass {
   };
}


TEST(DemanglingTest, intType) {
   EXPECT_EQ("int",demangle(typeid(int)));
}

TEST(DemanglingTest, floatType) {
   EXPECT_EQ("float",demangle(typeid(float)));
}

TEST(DemanglingTest, doubleType) {
   EXPECT_EQ("double",demangle(typeid(double)));
}

TEST(DemanglingTest, enumType) {
   EXPECT_EQ("Bt::Util::(anonymous namespace)::JustAnEnum",demangle(typeid(JustAnEnum)));
}

TEST(DemanglingTest, enumClassType) {
   EXPECT_EQ("Bt::Util::(anonymous namespace)::JustAnEnumClass",demangle(typeid(JustAnEnumClass)));
}

TEST(DemanglingTest, classType) {
   EXPECT_EQ("Bt::Util::(anonymous namespace)::JustAnClass",demangle(typeid(JustAnClass)));
}


} // namespace Util
} // namespace Bt
