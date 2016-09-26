#include "..\..\source\KWav.h"

#include <gtest/gtest.h>
#include <stdio.h>

//-----------------------------------------------------------------------------

using namespace std;
using namespace boost;

//-----------------------------------------------------------------------------

TEST( KWav, LoadHeader )
{
  const int8_t buffer[ 45 ] = "RIFF\0\0\0\0WAVEfmt\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0data\0\0\0\0";
  string errorDescription;
  KWav testOb;

  // Header is OK.
  EXPECT_TRUE( testOb.Load( buffer, 45, errorDescription ) );

  // Varients of header not OK.
  const int8_t buffer1[ 45 ] = "RIF \0\0\0\0WAVEfmt\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0data\0\0\0\0";
  EXPECT_FALSE( testOb.Load( buffer1, 45, errorDescription ) );

  const int8_t buffer2[ 45 ] = "RIFF\0\0\0\0WA Efmt\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0data\0\0\0\0";
  EXPECT_FALSE( testOb.Load( buffer2, 45, errorDescription ) );

  const int8_t buffer3[ 45 ] = "RIF \0\0\0\0WAVEfm \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0data\0\0\0\0";
  EXPECT_FALSE( testOb.Load( buffer3, 45, errorDescription ) );

  const int8_t buffer4[ 45 ] = "RIF \0\0\0\0WAVEfmt\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0 ata\0\0\0\0";
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

  int8_t* buffer = new int8_t[ fileLength ];
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

TEST( KWav, WriteFile )
{
  // Read file into buffer.
  FILE* file = fopen( "test.wav", "rb" );

  ASSERT_NE( 0, (int)file );

  fseek( file, 0, SEEK_END );
  const long fileLength = ftell( file );
  fseek( file, 0, SEEK_SET );

  int8_t* buffer = new int8_t[ fileLength ];
  const size_t sizeRead = fread( buffer, 1, fileLength, file );
  
  fclose( file );

  ASSERT_EQ( fileLength, sizeRead );

  // Initialise test object with buffer.
  string errorDescription;
  
  KWav testOb;
  bool result = testOb.Load( buffer, fileLength, errorDescription );

  delete buffer;
  
  ASSERT_TRUE( result );

  // Write back to file.
  FILE* outFile = fopen( "test_out.wav", "wb" );

  ASSERT_NE( 0, (int)outFile );

  const uint64_t bufferSize = testOb.CreateBuffer( buffer );

  size_t sizeWrote = fwrite( buffer, 1, static_cast< size_t >( bufferSize ), outFile );
  fclose( outFile );

  delete buffer;

  ASSERT_EQ( bufferSize, sizeWrote );
}

//-----------------------------------------------------------------------------