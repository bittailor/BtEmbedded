//*************************************************************************************************
//
//  BITTAILOR.CH - ArduinoCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::IntrusiveList
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_IntrusiveList__hpp
#define INC__Bt_Util_IntrusiveList__hpp

namespace Bt {
namespace Util {



class IntrusiveListBase {
   protected:

      IntrusiveListBase();
      ~IntrusiveListBase();

      class ElementBase {
         protected:
            ElementBase(){
               reset();
            }
         private:
            friend class IntrusiveListBase;
            void reset() {
               mNext = 0;
               mPrevious = 0;
            }
            ElementBase* mNext;
            ElementBase* mPrevious;
      };

      class IteratorBase {
         protected:
            IteratorBase(ElementBase* iCurrent) : mCurrent(iCurrent) {
            }

            void next(){
               if (mCurrent == 0) {
                  return;
               }
               mCurrent = mCurrent->mNext;
            }

            ElementBase* current() const {
               return mCurrent;
            }

            bool equal(const IteratorBase& iOther) const {
               return mCurrent == iOther.current();
            }


         private:
            ElementBase* mCurrent;
      };

      ElementBase* begin() {
         return mHead;
      }

      ElementBase* end() {
         return 0;
      }

      void pushBack(ElementBase* iElement);
      void remove(ElementBase* iElement);


      ElementBase* mHead;
      ElementBase* mTail;

};

template<typename T>
class IntrusiveList : private IntrusiveListBase  {
   public:

      class Element : public ElementBase {
      };

      class Iterator : public IteratorBase {
         public:

            Iterator(ElementBase* iCurrent)
            : IteratorBase(iCurrent) {
            }

            Iterator& operator++() {
               next();
               return *this;
            }

            Iterator operator++(int) {
               Iterator tmp(*this);
               next();
               return tmp;
            }

            T& operator*() const {
               return *static_cast<T*>(current());
            }

            T* operator->() const {
               return static_cast<T*>(current());
            }

            bool operator==(const Iterator& iOther) const {
               return equal(iOther);
            }

            bool operator!=(const Iterator& iOther) const {
               return !equal(iOther);
            }
      };

      IntrusiveList() {

      }

      ~IntrusiveList() {

      }

      Iterator begin() {
         return Iterator(IntrusiveListBase::begin());
      }

      Iterator end() {
         return Iterator(IntrusiveListBase::end());
      }
   
      void pushBack(T& iElement) {
         IntrusiveListBase::pushBack(&iElement);
      }

      void remove(T& iElement) {
         IntrusiveListBase::remove(&iElement);
      }


   private:
   	  // Constructor to prohibit copy construction.
      IntrusiveList(const IntrusiveList&);

      // Operator= to prohibit copy assignment
      IntrusiveList& operator=(const IntrusiveList&);



};





} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_IntrusiveList__hpp
