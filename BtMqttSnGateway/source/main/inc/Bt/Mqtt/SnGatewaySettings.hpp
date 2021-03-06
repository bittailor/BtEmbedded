//*************************************************************************************************
//
//  BITTAILOR.CH - BtMqttSnGateway
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Mqtt::SnGatewaySettings
//  
//*************************************************************************************************

#ifndef INC__Bt_Mqtt_SnGatewaySettings__hpp
#define INC__Bt_Mqtt_SnGatewaySettings__hpp

#include <map>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include <Bt/Util/Demangling.hpp>


#include <Bt/Mqtt/I_SnGatewaySettings.hpp>

namespace Bt {
namespace Mqtt {

class SnGatewaySettings : public I_SnGatewaySettings
{
   public:
      SnGatewaySettings(const std::string& iFilename);
      ~SnGatewaySettings();

      virtual const Logging& logging() const { return mLogging; }
      virtual const Broker& broker() const { return mBroker; }
      virtual const Rf24& rf24() const { return mRf24; }
      virtual const Socket& socket() const { return mSocket; }

   private:
   	  // Constructor to prohibit copy construction.
      SnGatewaySettings(const SnGatewaySettings&);

      // Operator= to prohibit copy assignment
      SnGatewaySettings& operator=(const SnGatewaySettings&);

      template<typename T> T get(const std::string& iPath) const {
         return mProperties.get<T>(iPath);
      }

      template<typename T> boost::optional<T> getOptional(const std::string& iPath) const {
         try {
            return boost::optional<T>(mProperties.get<T>(iPath));
         } catch (boost::property_tree::ptree_bad_path) {
            return boost::optional<T>();
         } catch (boost::property_tree::ptree_bad_data) {
            throw boost::property_tree::ptree_error(std::string("Invalid value '") + mProperties.get<std::string>(iPath) + "' for "+ iPath + " must be a " + Util::demangle(typeid(T)));
         }
      }

      template<typename T> T get(const std::string& iPath, const T& iDefault) const {
            try {
               return mProperties.get<T>(iPath);
            } catch (boost::property_tree::ptree_bad_path) {
               return iDefault;
            } catch (boost::property_tree::ptree_bad_data) {
               throw boost::property_tree::ptree_error(std::string("Invalid value '") + mProperties.get<std::string>(iPath) + "' for "+ iPath + " must be a " + Util::demangle(typeid(T)));
            }
      }

      template<typename E, typename T> E getEnum(const std::string& iPath, const E& iDefault, const std::map<T,E> iMapping) const {
         try {
            auto key = mProperties.get<T>(iPath);
            auto iter = iMapping.find(key);
            if(iter == iMapping.end()) {
               std::stringstream message;
               message << "Invalid value '" << key << "' for " << iPath << " must be one off { ";
               for (auto entry : iMapping) {
                  message << "'" << entry.first << "' ";
               }
               message << "}";
               throw boost::property_tree::ptree_error(message.str());
            }

            return iter->second;
         } catch (boost::property_tree::ptree_bad_path) {
            return iDefault;
         } catch (boost::property_tree::ptree_bad_data) {
            throw boost::property_tree::ptree_error(std::string("Invalid value '") + mProperties.get<std::string>(iPath) + "' for "+ iPath + " must be a " + Util::demangle(typeid(T)));
         }
      }



      boost::property_tree::ptree mProperties;
      Logging mLogging;
      Broker mBroker;
      Rf24 mRf24;
      Socket mSocket;
};

} // namespace Mqtt
} // namespace Bt

#endif // INC__Bt_Mqtt_SnGatewaySettings__hpp
