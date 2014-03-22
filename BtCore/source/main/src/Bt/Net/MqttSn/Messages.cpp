//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Net::Mqtt::MqttSn::Messages
//  
//*************************************************************************************************

#include <type_traits>

#include "Bt/Net/Mqtt/MqttSn/Messages.hpp"

namespace Bt {
namespace Net {
namespace Mqtt {
namespace MqttSn {

static_assert(std::is_standard_layout<Header>::value,"Header must be standard_layout");
static_assert(std::is_standard_layout<Connect>::value,"Connack must be standard_layout");
static_assert(std::is_standard_layout<Connack>::value,"Connack must be standard_layout");

} // namespace MqttSn
} // namespace Mqtt
} // namespace Net
} // namespace Bt
