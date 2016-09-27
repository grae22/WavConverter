#include "Converters\KSampleRateConverter.h"
#include "KWav.h"

#include <gtest/gtest.h>
#include <map>

//-----------------------------------------------------------------------------

using namespace std;
using namespace boost;

//-----------------------------------------------------------------------------

TEST( KSampleRateConverter, LinearUpSample )
{
  const int8_t data[] = "AC";
  KWav src( 1, 22050, 8, data, 2 );
  KWav* dest = nullptr;
  string errorDescription;

  map< string, string > options;
  options.insert( pair< string, string >( "-r", "44100" ) );
  options.insert( pair< string, string >( "-m", "L" ) );

  KSampleRateConverter::Convert( src, dest, options, errorDescription );

  ASSERT_NE( nullptr, dest );
  ASSERT_EQ( 3, dest->GetDataSize() );

  const int8_t* newData = dest->GetData();
  EXPECT_EQ( 'A', newData[ 0 ] );
  EXPECT_EQ( 'B', newData[ 1 ] );
  EXPECT_EQ( 'C', newData[ 2 ] );
}

//-----------------------------------------------------------------------------