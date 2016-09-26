#include "KWav.h"

//-----------------------------------------------------------------------------

using namespace std;

//-----------------------------------------------------------------------------

KWav::KWav()
:
  m_header(),
  m_channelCount( 0 ),
  m_samplesPerSec( 0 ),
  m_bitsPerSample( 0 )
{
  memset( &m_header, 0, sizeof( Header ) );
}

//-----------------------------------------------------------------------------

KWav::~KWav()
{

}

//-----------------------------------------------------------------------------

bool KWav::Load( const char* buffer,
                 const int bufferSize,
                 string& errorDescription )
{
  //-- Header.
  
  // Big enough?
  if( bufferSize < sizeof( Header ) )
  {
    errorDescription = "Buffer too small to container header.";
    return false;
  }

  // Copy header out of the buffer.
  memcpy( &m_header, buffer, sizeof( Header ) );

  // Check the id.
  if( ( m_header.m_chunkId[ 0 ] == 'R' &&
        m_header.m_chunkId[ 1 ] == 'I' &&
        m_header.m_chunkId[ 2 ] == 'F' &&
        m_header.m_chunkId[ 3 ] == 'F' ) == false )
  {
    errorDescription = "Chunk ID unknown - expected 'RIFF'.";
    return false;
  }

  // Check the format.
  if( ( m_header.m_format[ 0 ] == 'W' ) &&
      ( m_header.m_format[ 1 ] == 'A' ) &&
      ( m_header.m_format[ 2 ] == 'V' ) &&
      ( m_header.m_format[ 3 ] == 'E' ) == false )
  {
    errorDescription = "Chunk format unknown - expected 'WAVE'.";
    return false;
  }

  return true;
}

//-----------------------------------------------------------------------------