#include "..\..\source\KWav.h"

#include <gtest/gtest.h>
#include <stdio.h>

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

TEST( KWav, LoadHeader )
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