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

  void Reset();

private:
  struct Header
  {
  public:
    char m_chunkId[ 4 ];
    boost::uint32_t m_size;
    char m_format[ 4 ];
  };

  struct FormatChunk
  {
  public:
    char m_chunkId[ 4 ];
    boost::uint32_t m_size;
    boost::uint16_t m_channelCount;
    boost::uint32_t m_sampleRate;
    boost::uint32_t m_byteRate;
    boost::uint16_t m_blockAlign;
    boost::uint16_t m_bitsPerSample;
  };

  struct DataChunk
  {
  public:
    char m_chunkId[ 4 ];
    boost::uint32_t m_dataSize;
  };

private:
  Header m_header;
  FormatChunk m_format;
  DataChunk m_data;
  char* m_wavData;
};

//-----------------------------------------------------------------------------

#endif