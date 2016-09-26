#ifndef KCHANNELCONVERTER_H
#define KCHANNELCONVERTER_H

#include "..\Exports.h"

#include <boost/cstdint.hpp>
#include <map>

//-----------------------------------------------------------------------------

class KWav;

//-----------------------------------------------------------------------------

class WAVCONVERTER_API KChannelConverter
{
  //-- Public methods.
public:
  static bool Convert( const KWav& source,
                       KWav*& destination,
                       std::map< std::string, std::string > options,
                       std::string& errorDescription );

  //-- Private types.
private:
  enum StereoToMonoMode
  {
    COMBINE,
    SOURCE_LEFT,
    SOURCE_RIGHT
  };

  //-- Private methods.
private:
  static boost::int8_t* CreateStereoFromMono( const boost::int8_t* data,
                                              const boost::uint64_t dataSize,
                                              const boost::int16_t bytesPerSample,
                                              boost::uint64_t& newDataSize );

  static boost::int8_t* CreateMonoFromStereo( const StereoToMonoMode mode,
                                              const boost::int8_t* data,
                                              const boost::uint64_t dataSize,
                                              const boost::int16_t bytesPerSample,
                                              boost::uint64_t& newDataSize );
};

//-----------------------------------------------------------------------------

#endif