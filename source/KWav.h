#ifndef KWAV_H
#define KWAV_H

#include "Exports.h"

#include <string>
#include <boost/cstdint.hpp>

//-----------------------------------------------------------------------------

class WAVCONVERTER_API KWav
{
public:
  KWav();
  virtual ~KWav();

  bool Load( const char* buffer,
             const int bufferSize,
             std::string& errorDescription );

private:
  struct Header
  {
  public:
    char m_chunkId[ 4 ];
    boost::uint32_t m_chunkSize;
    char m_format[ 4 ];
  };

private:
  Header m_header;
  int m_channelCount;
  int m_samplesPerSec;
  int m_bitsPerSample;
};

//-----------------------------------------------------------------------------

#endif