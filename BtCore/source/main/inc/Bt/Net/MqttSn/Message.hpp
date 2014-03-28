//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::MqttSn::Message
//  
//*************************************************************************************************

#ifndef INC__Bt_Net_MqttSn_Message__hpp
#define INC__Bt_Net_MqttSn_Message__hpp

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <memory>
#include <vector>

#include <Bt/Net/MqttSn/Messages.hpp>

namespace Bt {
namespace Net {
namespace MqttSn {

class MessageBuffer 
{
   public:
      MessageBuffer(size_t iCapacity);
      MessageBuffer(std::vector<uint8_t> iRawMessage);
      ~MessageBuffer();

      uint8_t* buffer() { return mBuffer.data(); }
      const uint8_t* buffer() const  { return mBuffer.data(); }
      size_t bufferCapacity() const { return mBuffer.size() - 1;}

      uint8_t length() const { return mBuffer.data()[0]; }

      std::shared_ptr<I_Message> parse();

      friend std::ostream& operator<<(std::ostream& iStream, const MessageBuffer& iMessage);

   private:
   	  // Constructor to prohibit copy construction.
      MessageBuffer(const MessageBuffer&);

      // Operator= to prohibit copy assignment
      MessageBuffer& operator=(const MessageBuffer&);

      std::vector<uint8_t> mBuffer;
};

} // namespace MqttSn
} // namespace Net
} // namespace Bt

#endif // INC__Bt_Net_MqttSn_Message__hpp
