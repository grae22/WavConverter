#include "KChannelConverter.h"
#include "../KWav.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

//-----------------------------------------------------------------------------

using namespace std;
using namespace boost;

//-----------------------------------------------------------------------------

bool KChannelConverter::Convert( const KWav& source,
                                 KWav*& destination,
                                 std::map< std::string, std::string > options,
                                 std::string& errorDescription )
{
  // Get the new channel count from the options.
  uint16_t newChannelCount = 0;

  if( options.find( "-c" ) == options.end() )
  {
    errorDescription = "Channels (-c) option not specified.";
    return false;
  }

  try
  {
    newChannelCount = lexical_cast< uint16_t >( options[ "-c" ].c_str() );
  }
  catch( bad_lexical_cast )
  {
    errorDescription = "Non-numeric channels option specified.";
    return false;
  }

  if( newChannelCount == 0 )
  {
    errorDescription = "Channel count cannot be zero.";
    return false;
  }
  else if( newChannelCount > 2 )
  {
    errorDescription = "Channel count cannot be greater than two.";
    return false;
  }

  // New channel count is two? Check if the mode has been specified.
  StereoToMonoMode mode = COMBINE;

  if( options.find( "-m" ) != options.end() )
  {
    string m = options[ "-m" ];
    to_upper< string >( m );

    if( m.length() == 0 )
    {
      errorDescription = "No stero-to-mono mode (-m) specified.";
      return false;
    }

    switch( m[ 0 ] )
    {
      case 'C':
        mode = COMBINE;
        break;

      case 'L':
        mode = SOURCE_LEFT;
        break;

      case 'R':
        mode = SOURCE_RIGHT;
        break;

      default:
        errorDescription = "Invalid stero-to-mono mode (-m) specified.";
        return false;
    }
  }

  // Channel count is already the required count?
  if( newChannelCount == source.GetChannelCount() )
  {
    return true;
  }

  // Get the wav data.
  const uint64_t dataSize = source.GetDataSize();
  const int8_t* data = source.GetData();

  // Choose conversion type.
  int8_t* newData = nullptr;
  uint64_t newDataSize = 0;

  if( source.GetChannelCount() == 1 &&
      newChannelCount == 2 )
  {
    newData =
      CreateStereoFromMono(
        data,
        dataSize,
        source.GetBitsPerSample() / 8,
        newDataSize );
  }
  else if( source.GetChannelCount() == 2 &&
           newChannelCount == 1 )
  {
    newData =
      CreateMonoFromStereo(
        mode,
        data,
        dataSize,
        source.GetBitsPerSample() / 8,
        newDataSize );
  }
  else
  {
    errorDescription = "Unknown conversion type.";
    return false;
  }

  // Create destination wav object.
  destination =
    new KWav(
      newChannelCount,
      source.GetSampleRate(),
      source.GetBitsPerSample(),
      newData,
      newDataSize );

  // Clean up.
  delete newData;
  newData = nullptr;

  return true;
}

//-----------------------------------------------------------------------------

// TODO: This is probably rather slow.

int8_t* KChannelConverter::CreateStereoFromMono( const int8_t* data,
                                                 const uint64_t dataSize,
                                                 const int16_t bytesPerSample,
                                                 uint64_t& newDataSize )
{
  newDataSize = dataSize * 2;

  int8_t* buffer = new int8_t[ static_cast< unsigned int >( newDataSize ) ];

  for( uint64_t i = 0; i < dataSize; i += bytesPerSample )
  {
    memcpy( &buffer[ i * 2 ],
            &data[ i ], 
            bytesPerSample );

    memcpy( &buffer[ ( i * 2 ) + bytesPerSample ],
            &data[ i ], 
            bytesPerSample );
  }

  return buffer;
}

//-----------------------------------------------------------------------------

// TODO: This is probably rather slow.

int8_t* KChannelConverter::CreateMonoFromStereo( const StereoToMonoMode mode,
                                                 const int8_t* data,
                                                 const uint64_t dataSize,
                                                 const int16_t bytesPerSample,
                                                 uint64_t& newDataSize )
{
  newDataSize = dataSize / 2;

  if( dataSize % 2 > 0 )    // Shouldn't happen...
  {
    newDataSize++;
  }

  int8_t* buffer = new int8_t[ static_cast< unsigned int >( newDataSize ) ];

  uint64_t i = 0;

  if( mode == SOURCE_RIGHT )
  {
    i += bytesPerSample;
  }

  for( i = i; i < newDataSize; i += bytesPerSample )
  {
    if( mode == COMBINE )
    {
      int64_t leftValue = 0;
      int64_t rightValue = 0;

      memcpy( &leftValue, &data[ i * 2 ], bytesPerSample );
      memcpy( &rightValue, &data[ ( i * 2 ) + bytesPerSample ], bytesPerSample );

      leftValue += rightValue;
      leftValue /= 2;

      memcpy( &buffer[ i ],
              &leftValue,
              bytesPerSample );
    }
    else
    {
      memcpy( &buffer[ i ],
              &data[ i * 2 ],
              bytesPerSample );
    }
  }

  return buffer;
}

//-----------------------------------------------------------------------------