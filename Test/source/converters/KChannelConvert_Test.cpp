#include "Converters\KChannelConverter.h"
#include "KWav.h"

#include <gtest/gtest.h>
#include <map>

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

TEST( KChannelConverter, General )
{
  KWav src( 1, 44100, 16, nullptr, 0 );
  KWav dest;

  map< string, string > options;
  options.insert( pair< string, string >( "-c", "2" ) );

  // TODO: Continue here.

  KChannelConverter::Convert( src, dest, options );
}

//-----------------------------------------------------------------------------