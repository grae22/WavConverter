#include "KSampleRateConverter.h"
#include "../KWav.h"

#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

//-----------------------------------------------------------------------------

using namespace std;
using namespace boost;

//-----------------------------------------------------------------------------

bool KSampleRateConverter::Convert( const KWav& source,
                                    KWav*& destination,
                                    std::map< std::string, std::string > options,
                                    std::string& errorDescription )
{
  // Get the new sample rate from the options.
  uint32_t newRate = 0;

  if( options.find( "-r" ) == options.end() )
  {
    errorDescription = "Sample rate (-r) option not specified.";
    return false;
  }

  try
  {
    newRate = lexical_cast< uint32_t >( options[ "-r" ].c_str() );
  }
  catch( bad_lexical_cast )
  {
    errorDescription = "Non-numeric sample rate option specified.";
    return false;
  }

  if( newRate == 0 )
  {
    errorDescription = "Sample rate cannot be zero.";
    return false;
  }
  else if( newRate > 96000 )
  {
    errorDescription = "Sample rate cannot exceed 96000.";
    return false;
  }
  
  // Get the method to use.
  Method method = LINEAR;

  if( options.find( "-m" ) != options.end() )
  {
    string m = options[ "-m" ];
    to_upper< string >( m );

    if( m.length() == 0 )
    {
      errorDescription = "No re-sampling method (-m) specified.";
      return false;
    }

    switch( m[ 0 ] )
    {
      case 'L':
        method = LINEAR;
        break;

      case 'C':
        method = CUBIC;
        break;

      default:
        errorDescription = "Invalid re-sampling method (-m) specified.";
        return false;
    }
  }

  // Already at specified rate?
  if( source.GetSampleRate() == newRate )
  {
    return true;
  }
  
  // Get the wav data.
  const uint64_t dataSize = source.GetDataSize();
  const int8_t* data = source.GetData();

  // Choose conversion type.
  int8_t* newData = nullptr;
  uint64_t newDataSize = 0;

  switch( method )
  {
    case LINEAR:
      if( source.GetSampleRate() < newRate )
      {
        newData =
          UpsampleLinear( source,
                          newRate,
                          newDataSize );
      }
      break;

    case CUBIC:
      break;

    default:
      errorDescription = "Unknown re-sampling method.";
      return false;
  }

  // Create destination wav object.
  destination =
    new KWav(
      source.GetChannelCount(),
      newRate,
      source.GetBitsPerSample(),
      newData,
      newDataSize );

  return true;
}

//-----------------------------------------------------------------------------

int8_t* KSampleRateConverter::UpsampleLinear( const KWav& input,
                                              const uint32_t newSampleRate,
                                              uint64_t& newDataSize )
{
  // Calculate new buffer size.
  if( input.GetBytesPerSample() == 0 ||
      input.GetChannelCount() == 0 )
  {
    return nullptr;
  }

  newDataSize = newSampleRate / input.GetBytesPerSample() / input.GetChannelCount();

  // Allocate the buffer.
  int8_t* buffer = new int8_t[ newDataSize ];
  fill( buffer, buffer + newDataSize, 0 );

  // Calculate the ratio between sample rates.
  const float ratio = static_cast< float >( input.GetSampleRate() ) / newSampleRate;

  //
  for( uint64_t i = 0; i < input.GetDataSize(); i++ )
  {

  }

  return buffer;
}

//-----------------------------------------------------------------------------