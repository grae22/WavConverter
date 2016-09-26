#include "..\..\source\KWav.h"

#include <gtest/gtest.h>
#include <stdio.h>

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

TEST( KWav, LoadHeader )
{
  char buffer[ 45 ] = "RIFF\0\0\0\0WAVEfmt\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0data\0\0\0\0";
  string errorDescription;
  KWav testOb;

  // Header is OK.
  EXPECT_TRUE( testOb.Load( buffer, 45, errorDescription ) );

  // Varients of header not OK.
  char buffer1[ 45 ] = "RIF \0\0\0\0WAVEfmt\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0data\0\0\0\0";
  EXPECT_FALSE( testOb.Load( buffer1, 45, errorDescription ) );

  char buffer2[ 45 ] = "RIFF\0\0\0\0WA Efmt\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0data\0\0\0\0";
  EXPECT_FALSE( testOb.Load( buffer2, 45, errorDescription ) );

  char buffer3[ 45 ] = "RIF \0\0\0\0WAVEfm \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0data\0\0\0\0";
  EXPECT_FALSE( testOb.Load( buffer3, 45, errorDescription ) );

  char buffer4[ 45 ] = "RIF \0\0\0\0WAVEfmt\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0 ata\0\0\0\0";
  EXPECT_FALSE( testOb.Load( buffer4, 45, errorDescription ) );
}

//-----------------------------------------------------------------------------

TEST( KWav, LoadFile )
{
  // Read file into buffer.
  FILE* file = fopen( "test.wav", "rb" );

  ASSERT_NE( 0, (int)file );

  fseek( file, 0, SEEK_END );
  const long fileLength = ftell( file );
  fseek( file, 0, SEEK_SET );

  char* buffer = new char[ fileLength ];
  const size_t sizeRead = fread( buffer, 1, fileLength, file );
  
  fclose( file );

  ASSERT_EQ( fileLength, sizeRead );

  // Initialise test object with buffer.
  string errorDescription;
  
  KWav testOb;
  bool result = testOb.Load( buffer, fileLength, errorDescription );
  
  EXPECT_TRUE( result );

  // Clean up.
  delete buffer;
}

//-----------------------------------------------------------------------------