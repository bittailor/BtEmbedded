//*************************************************************************************************
//
//  BITTAILOR.CH - BtCore
//
//-------------------------------------------------------------------------------------------------
//
//  Bt::CoreInitializerPlatform
//  
//*************************************************************************************************

#include "Bt/CoreInitializerPlatform.hpp"
#include <Arduino.h>

namespace Bt {

namespace {

int serial_putc( char c, FILE * )
{
  Serial.write( c );

  return c;
}

}

//-------------------------------------------------------------------------------------------------

CoreInitializerPlatform::CoreInitializerPlatform() {
   init();
   Serial.begin(9600);
   fdevopen( &serial_putc, 0 );
}

//-------------------------------------------------------------------------------------------------

CoreInitializerPlatform::~CoreInitializerPlatform() {

}

//-------------------------------------------------------------------------------------------------

} // namespace Bt
