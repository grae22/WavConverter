#ifndef KWAV_H
#define KWAV_H

#include "Exports.h"

#include <boost/cstdint.hpp>
#include <string>

//-----------------------------------------------------------------------------

class WAVCONVERTER_API KWav
{
public:
  KWav();
  virtual ~KWav();

  // Resets object state and releases any allocated memory.
  void Reset();

  // Loads wav data from a buffer.
  // Returns 'false' on error, sets 'errorDescription' accordingly.
  bool Load( const boost::int8_t* buffer,
             const boost::uint64_t bufferSize,
             std::string& errorDescription );

  // Returns a new buffer populated with this wav's data.
  // Caller is responible for deleting the pointer.
  boost::uint64_t CreateBuffer( boost::int8_t*& buffer ) const;

private:
  // Data struct representing a wav's header chunk.
  struct Header
  {
  public:
    boost::int8_t m_chunkId[ 4 ];
    boost::uint32_t m_size;
    boost::int8_t m_format[ 4 ];
  };

  // Data struct representing a wav's format chunk.
  struct FormatChunk
  {
  public:
    boost::int8_t m_chunkId[ 4 ];
    boost::uint32_t m_size;
    boost::uint16_t m_channelCount;
    boost::uint32_t m_sampleRate;
    boost::uint32_t m_byteRate;
    boost::uint16_t m_blockAlign;
    boost::uint16_t m_bitsPerSample;
  };

  // Data struct representing a wav's data chunk.
  struct DataChunk
  {
  public:
    boost::int8_t m_chunkId[ 4 ];
    boost::uint32_t m_dataSize;
  };

private:
  Header m_header;
  FormatChunk m_format;
  DataChunk m_data;
  boost::int8_t* m_wavData;
};

//-----------------------------------------------------------------------------

#endif