//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::Rf24::RfPipe
//  
//*************************************************************************************************

#ifndef INC__Bt_Rf24_RfPipe__hpp
#define INC__Bt_Rf24_RfPipe__hpp

#include "Bt/Util/CollectionUtility.hpp"

using Bt::Util::sizeOfArray;

namespace Bt {
namespace Rf24 {

enum class RfPipe
{
   PIPE_0,
   PIPE_1,
   PIPE_2,
   PIPE_3,
   PIPE_4,
   PIPE_5,
};


class RfPipes {
   public:

      static constexpr RfPipe ALL_PIPES[] = {
         RfPipe::PIPE_0,
         RfPipe::PIPE_1,
         RfPipe::PIPE_2,
         RfPipe::PIPE_3,
         RfPipe::PIPE_4,
         RfPipe::PIPE_5
      };

      enum { NUMBER_OF_PIPES = sizeOfArray(ALL_PIPES) };
};

} // namespace Rf24
} // namespace Bt

#endif // INC__Bt_Rf24_RfPipe__hpp
