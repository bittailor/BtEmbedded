//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::TopicStorageTest
//  
//*************************************************************************************************

#include <gtest/gtest.h>

#include "Bt/Mqtt/TopicStorage.hpp"

#include <string>

namespace Bt {
namespace Mqtt {

class TopicStorageTest : public ::testing::Test {
   
   protected:
      
      virtual void SetUp() {
      }

      virtual void TearDown() {
         
      }

      TopicStorage mTopicStorage;

};


TEST_F(TopicStorageTest, findTopicId) {
   std::string topic = "bt/topic/one";
   uint16_t topicIdFirst = mTopicStorage.getOrCreateTopicId(topic);
   uint16_t topicIdSecond = mTopicStorage.getTopicId(topic);

   EXPECT_EQ(topicIdFirst, topicIdSecond);
}

TEST_F(TopicStorageTest, findTopicIdNotFound) {
   std::string topic1 = "bt/topic/one";
   std::string topic2 = "bt/topic/two";
   mTopicStorage.getOrCreateTopicId(topic1);
   uint16_t topicIdSecond = mTopicStorage.getTopicId(topic2);

   EXPECT_EQ(TopicStorage::NO_TOPIC_ID, topicIdSecond);
}

TEST_F(TopicStorageTest, doNotCreateNewIdWhenPresent) {
   std::string topic = "bt/topic/one";
   uint16_t topicIdFirst = mTopicStorage.getOrCreateTopicId(topic);
   uint16_t topicIdSecond = mTopicStorage.getOrCreateTopicId(topic);

   EXPECT_EQ(topicIdFirst, topicIdSecond);
}

TEST_F(TopicStorageTest, findTopicName) {
   std::string topic = "bt/topic/one";
   uint16_t topicIdFirst = mTopicStorage.getOrCreateTopicId(topic);
   EXPECT_EQ(topic, mTopicStorage.getTopicName(topicIdFirst));
}

TEST_F(TopicStorageTest, newTopicIdOnOtherTopicName) {
   std::string topic1 = "bt/topic/one";
   std::string topic2 = "bt/topic/two";
   uint16_t topicIdFirst = mTopicStorage.getOrCreateTopicId(topic1);
   uint16_t topicIdSecond = mTopicStorage.getOrCreateTopicId(topic2);

   EXPECT_NE(topicIdFirst, topicIdSecond);
}


} // namespace Mqtt
} // namespace Bt
