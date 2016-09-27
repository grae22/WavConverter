#ifndef KSampleRateConverter_H
#define KSampleRateConverter_H

#include "../Exports.h"

#include <boost/cstdint.hpp>
#include <map>

//-----------------------------------------------------------------------------

class KWav;

//-----------------------------------------------------------------------------

class WAVCONVERTER_API KSampleRateConverter
{
  //-- Public methods.
public:
  static bool Convert( const KWav& source,
                       KWav*& destination,
                       std::map< std::string, std::string > options,
                       std::string& errorDescription );

  //-- Private types.
private:
  enum Method
  {
    LINEAR,
    CUBIC
  };

  //-- Private methods.
private:
  static boost::int8_t* UpsampleLinear( const KWav& input,
                                        const boost::uint32_t newSampleRate,
                                        boost::uint64_t& newDataSize );
};

//-----------------------------------------------------------------------------

#endif