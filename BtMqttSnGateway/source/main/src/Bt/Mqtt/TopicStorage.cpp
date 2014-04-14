//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::TopicStorage
//  
//*************************************************************************************************

#include "Bt/Mqtt/TopicStorage.hpp"

namespace Bt {
namespace Mqtt {


//-------------------------------------------------------------------------------------------------

TopicStorage::TopicStorage() {
   mIdToName.push_back("0x0000 == RESERVED");
}

//-------------------------------------------------------------------------------------------------

uint16_t TopicStorage::getOrCreateTopicId(const std::string& iTopicName) {
   std::lock_guard<std::mutex> lock(mMutex);
   auto iter = mNameToId.find(iTopicName);

   if(iter != mNameToId.end()) {
      return iter->second;
   }

   uint16_t newId = mIdToName.size();
   mIdToName.push_back(iTopicName);
   mNameToId[iTopicName] = newId;
   return newId;
}



//-------------------------------------------------------------------------------------------------

uint16_t TopicStorage::getTopicId(const std::string& iTopicName) {
   std::lock_guard<std::mutex> lock(mMutex);
   auto iter = mNameToId.find(iTopicName);

   if(iter != mNameToId.end()) {
      return iter->second;
   }
   return NO_TOPIC_ID;
}

//-------------------------------------------------------------------------------------------------

std::string TopicStorage::getTopicName(uint16_t iTopicId) {
   std::lock_guard<std::mutex> lock(mMutex);
   if (iTopicId >= mIdToName.size()) {
      return std::string();
   }
   return mIdToName[iTopicId];
}

//-------------------------------------------------------------------------------------------------

void TopicStorage::clear() {
   std::lock_guard<std::mutex> lock(mMutex);
   mIdToName.resize(1);
   mNameToId.clear();

}

//-------------------------------------------------------------------------------------------------

} // namespace Mqtt
} // namespace Bt
