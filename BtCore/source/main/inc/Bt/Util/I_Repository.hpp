//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::I_Repository
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_I_Repository__hpp
#define INC__Bt_Util_I_Repository__hpp

#include <map>
#include <memory>

namespace Bt {
namespace Util {

template <typename Entity, typename Id = int>
class I_Repository
{
   public:

      typedef std::map<Id,std::shared_ptr<Entity>> Container;
      typedef typename Container::iterator Iterator;
      typedef typename Container::size_type SizeType;

      virtual ~I_Repository(){}
      
      virtual bool add(std::shared_ptr<Entity> iEntity) = 0;
      virtual bool remove(Id iId) = 0;
      virtual std::shared_ptr<Entity> lookup(Id iId) = 0;
      virtual void clear() = 0;
      virtual SizeType size() =0;
      virtual Iterator begin()=0;
      virtual Iterator end()=0;

}; // end class I_Repository
} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_I_Repository__hpp
