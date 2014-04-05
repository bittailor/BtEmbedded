//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::TopicStorage
//  
//*************************************************************************************************

#ifndef INC__Bt_Mqtt_TopicStorage__hpp
#define INC__Bt_Mqtt_TopicStorage__hpp

#include <string>
#include <map>
#include <mutex>
#include <vector>

namespace Bt {
namespace Mqtt {

class TopicStorage 
{
   public:

      TopicStorage() = default;

      uint16_t getOrCreateTopicId(const std::string& iTopicName);
      std::string getTopicName(uint16_t iTopicId);
   
   private:
   	  // Constructor to prohibit copy construction.
      TopicStorage(const TopicStorage&);

      // Operator= to prohibit copy assignment
      TopicStorage& operator=(const TopicStorage&);

      std::mutex mMutex;
      std::vector<std::string> mIdToName;
      std::map<std::string,uint16_t> mNameToId;
};

} // namespace Mqtt
} // namespace Bt

#endif // INC__Bt_Mqtt_TopicStorage__hpp
