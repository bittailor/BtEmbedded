//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Singleton
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_Singleton__hpp
#define INC__Bt_Util_Singleton__hpp

namespace Bt {
namespace Util {

template<typename T>
class Singleton
{
   public:

      static bool valid() {
         return mInstance != 0 ;
      }

      static T* instance() {
         return mInstance;
      }

      class Instance
      {
         public:
            Instance(T& iInstance)
            : mPrevious(Singleton::mInstance)
            {
               Singleton::mInstance = &iInstance;
            }

            ~Instance()
            {
               Singleton::mInstance = mPrevious;
            }

         private:
            Instance(const Instance&);
            Instance& operator=(const Instance&);
            T* mPrevious;
      };

      friend class Singleton::Instance;

   private:
      // Only use the static methods
      Singleton();
      ~Singleton();
      Singleton(const Singleton&);
      Singleton& operator=(const Singleton&);

      static T* mInstance;
};

//-------------------------------------------------------------------------------------------------
//
//-------------------------------------------------------------------------------------------------

template <typename T>
T* Singleton<T>::mInstance;

//-------------------------------------------------------------------------------------------------

} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_Singleton__hpp
