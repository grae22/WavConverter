#ifndef KSampleRateConverter_H
#define KSampleRateConverter_H

#include "..\Exports.h"

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

  //-- Private methods.
private:

};

//-----------------------------------------------------------------------------

#endif