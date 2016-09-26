#include "KWav.h"

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

KWav::KWav()
:
  m_header(),
  m_format(),
  m_data(),
  m_wavData( nullptr )
{
  Reset();
}

//-----------------------------------------------------------------------------

KWav::~KWav()
{
  Reset();
}

//-----------------------------------------------------------------------------

void KWav::Reset()
{
  delete m_wavData;
  m_wavData = nullptr;

  memset( &m_header, 0, sizeof( Header ) );
  memset( &m_format, 0, sizeof( FormatChunk ) );
  memset( &m_data, 0, sizeof( DataChunk ) );
}

//-----------------------------------------------------------------------------

bool KWav::Load( const char* buffer,
                 const unsigned int bufferSize,
                 string& errorDescription )
{
  // Reset state before attempting to load.
  Reset();

  //-- Header.
  
  // Big enough?
  if( bufferSize < sizeof( Header ) )
  {
    errorDescription = "Buffer too small to contain header.";
    return false;
  }

  // Copy header out of the buffer.
  memcpy( &m_header, buffer, sizeof( Header ) );

  // Check the id.
  if( m_header.m_chunkId[ 0 ] != 'R' ||
      m_header.m_chunkId[ 1 ] != 'I' ||
      m_header.m_chunkId[ 2 ] != 'F' ||
      m_header.m_chunkId[ 3 ] != 'F' )
  {
    errorDescription = "Chunk ID unknown - expected 'RIFF'.";
    return false;
  }

  // Check the format.
  if( m_header.m_format[ 0 ] != 'W' ||
      m_header.m_format[ 1 ] != 'A' ||
      m_header.m_format[ 2 ] != 'V' ||
      m_header.m_format[ 3 ] != 'E' )
  {
    errorDescription = "Chunk format unknown - expected 'WAVE'.";
    return false;
  }

  //-- Format chunk.

  // Calculate the buffer offset and check the buffer is big enough.
  uint64_t formatChunkOffset = sizeof( Header );

  if( bufferSize < formatChunkOffset + sizeof( FormatChunk ) )
  {
    errorDescription = "Buffer too small to contain the format chunk.";
    return false;
  }

  // Copy the format chunk from the buffer.
  memcpy( &m_format, &buffer[ formatChunkOffset ], sizeof( FormatChunk ) );

  // Check the id.
  if( m_format.m_chunkId[ 0 ] != 'f' ||
      m_format.m_chunkId[ 1 ] != 'm' ||
      m_format.m_chunkId[ 2 ] != 't' )
  {
    errorDescription = "Format chunk ID unknown - expected 'fmt'.";
    return false;
  }

  //-- Data chunk.

  // Calculate the buffer offset and check the buffer is big enough.
  uint64_t dataChunkOffset = formatChunkOffset + sizeof( FormatChunk );

  if( bufferSize < dataChunkOffset + sizeof( DataChunk ) )
  {
    errorDescription = "Buffer too small to contain the data chunk.";
    return false;
  }

  // Copy the format chunk from the buffer.
  memcpy( &m_data, &buffer[ dataChunkOffset ], sizeof( DataChunk ) );

  // Check the id.
  if( m_data.m_chunkId[ 0 ] != 'd' ||
      m_data.m_chunkId[ 1 ] != 'a' ||
      m_data.m_chunkId[ 2 ] != 't' ||
      m_data.m_chunkId[ 3 ] != 'a' )
  {
    errorDescription = "Data chunk ID unknown - expected 'data'.";
    return false;
  }

  //-- Data.
  
  // Calculate the buffer offset and check the buffer is big enough.
  uint64_t dataOffset = dataChunkOffset + sizeof( DataChunk );

  if( bufferSize < dataOffset + m_data.m_dataSize )
  {
    errorDescription = "Buffer too small to contain the data.";
    return false;
  }

  // Allocate buffer for data.
  m_wavData = new char[ m_data.m_dataSize ];
  memcpy( m_wavData, &buffer[ dataOffset ], m_data.m_dataSize );

  return true;
}

//-----------------------------------------------------------------------------

unsigned int KWav::CreateBuffer( char*& buffer ) const
{
  const unsigned int bufferSize =
    sizeof( Header ) +
    sizeof( FormatChunk ) +
    sizeof( DataChunk ) +
    m_data.m_dataSize;

  const unsigned int offsetFormatChunk = sizeof( Header );
  const unsigned int offsetDataChunk = offsetFormatChunk + sizeof( FormatChunk );
  const unsigned int offsetData = offsetDataChunk + sizeof( DataChunk );

  buffer = new char[ bufferSize ];

  memcpy( buffer, &m_header, sizeof( Header ) );
  memcpy( &buffer[ offsetFormatChunk ], &m_format, sizeof( FormatChunk ) );
  memcpy( &buffer[ offsetDataChunk ], &m_data, sizeof( DataChunk ) );
  memcpy( &buffer[ offsetData ], m_wavData, m_data.m_dataSize );

  return bufferSize;
}

//-----------------------------------------------------------------------------