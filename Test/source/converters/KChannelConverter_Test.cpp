#include "Converters\KChannelConverter.h"
#include "KWav.h"

#include <gtest/gtest.h>
#include <map>

//-----------------------------------------------------------------------------

using namespace std;
using namespace boost;

//-----------------------------------------------------------------------------

TEST( KChannelConverter, MonoToStero8Bit )
{
  const int8_t data[] = "AB";
  KWav src( 1, 44100, 8, data, 2 );
  KWav* dest = nullptr;
  string errorDescription;

  map< string, string > options;
  options.insert( pair< string, string >( "-c", "2" ) );

  KChannelConverter::Convert( src, dest, options, errorDescription );

  ASSERT_NE( nullptr, dest );
  ASSERT_EQ( 4, dest->GetDataSize() );

  const int8_t* newData = dest->GetData();
  EXPECT_EQ( 'A', newData[ 0 ] );
  EXPECT_EQ( 'A', newData[ 1 ] );
  EXPECT_EQ( 'B', newData[ 2 ] );
  EXPECT_EQ( 'B', newData[ 3 ] );
}

//-----------------------------------------------------------------------------

TEST( KChannelConverter, MonoToStero16Bit )
{
  const int8_t data[] = "ABCD";
  KWav src( 1, 44100, 16, data, 4 );
  KWav* dest = nullptr;
  string errorDescription;

  map< string, string > options;
  options.insert( pair< string, string >( "-c", "2" ) );

  KChannelConverter::Convert( src, dest, options, errorDescription );

  ASSERT_NE( nullptr, dest );
  ASSERT_EQ( 8, dest->GetDataSize() );

  const int8_t* newData = dest->GetData();
  EXPECT_EQ( 'A', newData[ 0 ] );
  EXPECT_EQ( 'B', newData[ 1 ] );
  EXPECT_EQ( 'A', newData[ 2 ] );
  EXPECT_EQ( 'B', newData[ 3 ] );
  EXPECT_EQ( 'C', newData[ 4 ] );
  EXPECT_EQ( 'D', newData[ 5 ] );
  EXPECT_EQ( 'C', newData[ 6 ] );
  EXPECT_EQ( 'D', newData[ 7 ] );
}

//-----------------------------------------------------------------------------

TEST( KChannelConverter, FileMonoToStero16Bit )
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
  buffer = nullptr;
  
  ASSERT_TRUE( result );

  // Convert.
  KWav* newTestOb = nullptr;

  map< string, string > options;
  options.insert( pair< string, string >( "-c", "2" ) );

  result =
    KChannelConverter::Convert(
      testOb,
      newTestOb,
      options,
      errorDescription );

  // Write back to file.
  FILE* outFile = fopen( "test_stero_out.wav", "wb" );

  ASSERT_NE( 0, (int)outFile );

  const uint64_t bufferSize = newTestOb->CreateBuffer( buffer );

  size_t sizeWrote = fwrite( buffer, 1, static_cast< size_t >( bufferSize ), outFile );
  fclose( outFile );

  delete buffer;
  buffer = nullptr;

  ASSERT_EQ( bufferSize, sizeWrote );
}

//-----------------------------------------------------------------------------

TEST( KChannelConverter, FileSteroToMono16Bit_SourceLeft )
{
  // Read file into buffer.
  FILE* file = fopen( "StereoTest.wav", "rb" );

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
  buffer = nullptr;
  
  ASSERT_TRUE( result );

  // Convert.
  KWav* newTestOb = nullptr;

  map< string, string > options;
  options.insert( pair< string, string >( "-c", "1" ) );
  options.insert( pair< string, string >( "-m", "L" ) );

  result =
    KChannelConverter::Convert(
      testOb,
      newTestOb,
      options,
      errorDescription );

  // Write back to file.
  FILE* outFile = fopen( "test_mono_left_out.wav", "wb" );

  ASSERT_NE( 0, (int)outFile );

  const uint64_t bufferSize = newTestOb->CreateBuffer( buffer );

  size_t sizeWrote = fwrite( buffer, 1, static_cast< size_t >( bufferSize ), outFile );
  fclose( outFile );

  delete buffer;
  buffer = nullptr;

  ASSERT_EQ( bufferSize, sizeWrote );
}

//-----------------------------------------------------------------------------

TEST( KChannelConverter, FileSteroToMono16Bit_SourceCombine )
{
  // Read file into buffer.
  FILE* file = fopen( "StereoTest.wav", "rb" );

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
  buffer = nullptr;
  
  ASSERT_TRUE( result );

  // Convert.
  KWav* newTestOb = nullptr;

  map< string, string > options;
  options.insert( pair< string, string >( "-c", "1" ) );
  options.insert( pair< string, string >( "-m", "C" ) );

  result =
    KChannelConverter::Convert(
      testOb,
      newTestOb,
      options,
      errorDescription );

  // Write back to file.
  FILE* outFile = fopen( "test_mono_combined_out.wav", "wb" );

  ASSERT_NE( 0, (int)outFile );

  const uint64_t bufferSize = newTestOb->CreateBuffer( buffer );

  size_t sizeWrote = fwrite( buffer, 1, static_cast< size_t >( bufferSize ), outFile );
  fclose( outFile );

  delete buffer;
  buffer = nullptr;

  ASSERT_EQ( bufferSize, sizeWrote );
}

//-----------------------------------------------------------------------------