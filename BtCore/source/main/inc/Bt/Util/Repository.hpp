//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Repository
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_Repository__hpp
#define INC__Bt_Util_Repository__hpp

#include <mutex>

#include <Bt/Util/I_Repository.hpp>


namespace Bt {
namespace Util {

template <typename Entity, typename Id = int,  Id (Entity::*IdFunction)() = &Entity::id>
class Repository : public I_Repository<Entity,Id>
{

   public:

      Repository() = default;

      typedef typename I_Repository<Entity,Id>::Container Container;
      typedef typename I_Repository<Entity,Id>::Iterator Iterator;
      typedef typename I_Repository<Entity,Id>::SizeType SizeType;

      virtual bool add(std::shared_ptr<Entity> iEntity);
      virtual bool remove(Id iId);
      virtual std::shared_ptr<Entity> lookup(Id iId);
      virtual void clear();
      virtual SizeType size();
      virtual Iterator begin();
      virtual Iterator end();

   private:
      Repository(const Repository&);

      Repository& operator=(const Repository&);

      std::mutex mMutex;
      Container mEntities;

}; // end class Repository


//-------------------------------------------------------------------------------------------------

template <typename Entity, typename Id, Id (Entity::*IdFunction)()>
bool Repository<Entity,Id,IdFunction>::add(std::shared_ptr<Entity> iEntity)
{
   std::lock_guard<std::mutex> lock(mMutex);
   int id = ((*iEntity).*IdFunction)();
   if(mEntities.find(id) != mEntities.end())
   {
      return false;
   }
   mEntities[id] = iEntity;
   return true;
}

//-------------------------------------------------------------------------------------------------

template <typename Entity, typename Id, Id (Entity::*IdFunction)()>
bool Repository<Entity,Id,IdFunction>::remove(Id iId)
{
   std::lock_guard<std::mutex> lock(mMutex);
   typename Container::iterator iter = mEntities.find(iId);
   if(iter!=mEntities.end())
   {
      mEntities.erase(iter);
      return true;
   }
   return false;
}

//-------------------------------------------------------------------------------------------------

template <typename Entity, typename Id, Id (Entity::*IdFunction)()>
std::shared_ptr<Entity> Repository<Entity,Id,IdFunction>::lookup(Id iId)
{
   std::lock_guard<std::mutex> lock(mMutex);
   typename Container::iterator iter = mEntities.find(iId);
   if(iter!=mEntities.end())
   {
      return iter->second;
   }
   return 0;
}

//-------------------------------------------------------------------------------------------------

template <typename Entity, typename Id, Id (Entity::*IdFunction)()>
typename Repository<Entity,Id,IdFunction>::Iterator Repository<Entity,Id,IdFunction>::begin()
{
   return mEntities.begin();
}

//-------------------------------------------------------------------------------------------------

template <typename Entity, typename Id, Id (Entity::*IdFunction)()>
typename Repository<Entity,Id,IdFunction>::Iterator Repository<Entity,Id,IdFunction>::end()
{
   return mEntities.end();
}

//-------------------------------------------------------------------------------------------------

template <typename Entity, typename Id, Id (Entity::*IdFunction)()>
void Repository<Entity,Id,IdFunction>::clear()
{
   std::lock_guard<std::mutex> lock(mMutex);
   mEntities.clear();
}

//-------------------------------------------------------------------------------------------------

template <typename Entity, typename Id, Id (Entity::*IdFunction)()>
typename Repository<Entity,Id,IdFunction>::SizeType Repository<Entity,Id,IdFunction>::size()
{
   return mEntities.size();
}

//-------------------------------------------------------------------------------------------------


} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_Repository__hpp
