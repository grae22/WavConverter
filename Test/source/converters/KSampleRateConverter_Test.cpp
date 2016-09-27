#include "Converters\KSampleRateConverter.h"
#include "KWav.h"

#include <gtest/gtest.h>
#include <map>

//-----------------------------------------------------------------------------

using namespace std;
using namespace boost;

//-----------------------------------------------------------------------------

TEST( KSampleRateConverter, MonoToStero8Bit )
{/*
  const int8_t data[] = "AB";
  KWav src( 1, 44100, 8, data, 2 );
  KWav* dest = nullptr;
  string errorDescription;

  map< string, string > options;
  options.insert( pair< string, string >( "-c", "2" ) );

  KSampleRateConverter::Convert( src, dest, options, errorDescription );

  ASSERT_NE( nullptr, dest );
  ASSERT_EQ( 4, dest->GetDataSize() );

  const int8_t* newData = dest->GetData();
  EXPECT_EQ( 'A', newData[ 0 ] );
  EXPECT_EQ( 'A', newData[ 1 ] );
  EXPECT_EQ( 'B', newData[ 2 ] );
  EXPECT_EQ( 'B', newData[ 3 ] );*/
}

//-----------------------------------------------------------------------------