//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Util::Endianness
//  
//*************************************************************************************************

#ifndef INC__Bt_Util_Endianness__hpp
#define INC__Bt_Util_Endianness__hpp

namespace Bt {
namespace Util {

class Endianness 
{
   public:
      static bool sIsBigEndian;

      template <class T>
      static T hostToNetwork(T i_nValue)
      {
            if (sIsBigEndian)
            {
               return i_nValue;
            }
            else
            {
               T nBigEndian;
               for (int nIdx = sizeof(T); nIdx > 0; --nIdx)
               {
                  *(reinterpret_cast<char*>(&nBigEndian) + (sizeof(T)-nIdx)) = *(reinterpret_cast<char*>(&i_nValue) + (nIdx-1));
               }
               return nBigEndian;
            }
      }

      template <class T>
      static T networkToHost(T i_nValue)
      {
            if (sIsBigEndian)
            {
               return i_nValue;

            }
            else
            {
               T nLittleEndian;
               for (int nIdx = sizeof(T); nIdx > 0; --nIdx)
               {
                  *(reinterpret_cast<char*>(&nLittleEndian) + (sizeof(T)-nIdx)) = *(reinterpret_cast<char*>(&i_nValue) + (nIdx-1));
               }
               return nLittleEndian;
            }
      }

   private:
      Endianness();
      ~Endianness();

   	// Constructor to prohibit copy construction.
      Endianness(const Endianness&);

      // Operator= to prohibit copy assignment
      Endianness& operator=(const Endianness&);
};

} // namespace Util
} // namespace Bt

#endif // INC__Bt_Util_Endianness__hpp
