#ifndef KCHANNELCONVERTER_H
#define KCHANNELCONVERTER_H

#include "..\Exports.h"

#include <map>

//-----------------------------------------------------------------------------

class KWav;

//-----------------------------------------------------------------------------

class WAVCONVERTER_API KChannelConverter
{
public:
  static bool Convert( const KWav& source,
                       KWav& destination,
                       std::map< std::string, std::string > options );
};

//-----------------------------------------------------------------------------

#endif