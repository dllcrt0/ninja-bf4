#include "stdafx.h"

namespace AtgCleaned
{
//--------------------------------------------------------------------------------------
// Name: RiffChunk()
// Desc: Constructor
//--------------------------------------------------------------------------------------
RiffChunk::RiffChunk()
{
    // Initialize defaults
    m_fccChunkId = 0;
    m_pParentChunk = NULL;
    m_hFile = INVALID_HANDLE_VALUE;
    m_dwDataOffset = 0;
    m_dwDataSize = 0;
    m_dwFlags = 0;
}


//--------------------------------------------------------------------------------------
// Name: Initialize()
// Desc: Initializes the Riff chunk for use
//--------------------------------------------------------------------------------------
VOID RiffChunk::Initialize( FOURCC fccChunkId, const RiffChunk* pParentChunk,
                            HANDLE hFile )
{
    m_fccChunkId = fccChunkId;
    m_pParentChunk = pParentChunk;
    m_hFile = hFile;
}


//--------------------------------------------------------------------------------------
// Name: Open()
// Desc: Opens an existing chunk
//--------------------------------------------------------------------------------------
HRESULT RiffChunk::Open()
{
    LONG lOffset = 0;

    // Seek to the first byte of the parent chunk's data section
    if( m_pParentChunk )
    {
        lOffset = m_pParentChunk->m_dwDataOffset;

        // Special case the RIFF chunk
        if( ATG_FOURCC_RIFF == m_pParentChunk->m_fccChunkId )
            lOffset += sizeof( FOURCC );
    }

    // Read each child chunk header until we find the one we're looking for
    for(; ; )
    {
        if( INVALID_SET_FILE_POINTER == SetFilePointer( m_hFile, lOffset, NULL, FILE_BEGIN ) )
            return HRESULT_FROM_WIN32( GetLastError() );

        RIFFHEADER rhRiffHeader;
        DWORD dwRead;
        if( 0 == ReadFile( m_hFile, &rhRiffHeader, sizeof( rhRiffHeader ), &dwRead, NULL ) )
            return HRESULT_FROM_WIN32( GetLastError() );
        rhRiffHeader.dwDataSize = __loadwordbytereverse( 0, &rhRiffHeader.dwDataSize );

        // Hit EOF without finding it
        if( 0 == dwRead )
            return E_FAIL;

        // Check if we found the one we're looking for
        if( m_fccChunkId == rhRiffHeader.fccChunkId )
        {
            // Save the chunk size and data offset
            m_dwDataOffset = lOffset + sizeof( rhRiffHeader );
            m_dwDataSize = rhRiffHeader.dwDataSize;

            // Success
            m_dwFlags |= RIFFCHUNK_FLAGS_VALID;

            return S_OK;
        }

        lOffset += sizeof( rhRiffHeader ) + rhRiffHeader.dwDataSize;
    }
}


//--------------------------------------------------------------------------------------
// Name: ReadData()
// Desc: Reads from the file
//--------------------------------------------------------------------------------------
HRESULT RiffChunk::ReadData( LONG lOffset, VOID* pData, DWORD dwDataSize, OVERLAPPED* pOL ) const
{
    HRESULT hr = S_OK;

    OVERLAPPED defaultOL = {0};
    OVERLAPPED* pOverlapped = pOL;
    if( !pOL )
    {
        pOverlapped = &defaultOL;
    }

    // Seek to the offset
    pOverlapped->Offset = m_dwDataOffset + lOffset;

    // Read from the file
    DWORD dwRead;
    if( SUCCEEDED( hr ) && 0 == ReadFile( m_hFile, pData, dwDataSize, &dwRead, pOverlapped ) )
        hr = HRESULT_FROM_WIN32( GetLastError() );

    if( SUCCEEDED( hr ) && !pOL )
    {
        // we're using the default overlapped structure, which means that even if the
        // read was async, we need to act like it was synchronous.
        if( !GetOverlappedResult( m_hFile, pOverlapped, &dwRead, TRUE ) )
            hr = HRESULT_FROM_WIN32( GetLastError() );
    }
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Name: WaveFile()
// Desc: Constructor
//--------------------------------------------------------------------------------------
WaveFile::WaveFile()
{
    m_hFile = INVALID_HANDLE_VALUE;
}


//--------------------------------------------------------------------------------------
// Name: ~WaveFile()
// Desc: Denstructor
//--------------------------------------------------------------------------------------
WaveFile::~WaveFile()
{
    Close();
}

HRESULT WaveFile::OpenWithHandle(HANDLE hFile) {
	Close();

	// Open the file
	m_hFile = hFile;

	DbgPrint("File Handle: %X", m_hFile);

	if (INVALID_HANDLE_VALUE == m_hFile) {
		DbgPrint("error: %X", GetLastError());
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// Initialize the chunk objects
	m_RiffChunk.Initialize(ATG_FOURCC_RIFF, NULL, m_hFile);
	m_FormatChunk.Initialize(ATG_FOURCC_FORMAT, &m_RiffChunk, m_hFile);
	m_DataChunk.Initialize(ATG_FOURCC_DATA, &m_RiffChunk, m_hFile);
	m_WaveSampleChunk.Initialize(ATG_FOURCC_WSMP, &m_RiffChunk, m_hFile);
	m_SamplerChunk.Initialize(ATG_FOURCC_SMPL, &m_RiffChunk, m_hFile);
	m_DpdsChunk.Initialize(ATG_FOURCC_DPDS, &m_RiffChunk, m_hFile);
	m_SeekChunk.Initialize(ATG_FOURCC_SEEK, &m_RiffChunk, m_hFile);

	HRESULT hr = m_RiffChunk.Open();
	if (FAILED(hr)) {
		DbgPrint("ERR: 2");
		return hr;
	}

	hr = m_FormatChunk.Open();
	if (FAILED(hr)) {
		DbgPrint("ERR: 3");
		return hr;
	}

	hr = m_DataChunk.Open();
	if (FAILED(hr)) {
		DbgPrint("ERR: 4");
		return hr;
	}

	// Wave Sample and Sampler chunks are not required
	m_WaveSampleChunk.Open();
	m_SamplerChunk.Open();

	// Dpds chunk is valid only for XWMA files
	m_DpdsChunk.Open();

	// Seek chunk is valid only for XMA files
	m_SeekChunk.Open();

	// Validate the file type
	FOURCC fccType;
	hr = m_RiffChunk.ReadData(0, &fccType, sizeof(fccType), NULL);
	if (FAILED(hr)) {
		DbgPrint("ERR: 5");
		return hr;
	}

	if (ATG_FOURCC_WAVE != fccType && ATG_FOURCC_XWMA != fccType) {
		DbgPrint("ERR: 6");
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: Open()
// Desc: Initializes the object
//--------------------------------------------------------------------------------------
HRESULT WaveFile::Open( const CHAR* strFileName )
{
    // If we're already open, close
    Close();

    // Open the file
    m_hFile = CreateFile(
        strFileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
        NULL );

	DbgPrint("File Handle: %X", m_hFile);

	if (INVALID_HANDLE_VALUE == m_hFile) {
		DbgPrint("error: %X", GetLastError());
		return HRESULT_FROM_WIN32(GetLastError());
	}

    // Initialize the chunk objects
    m_RiffChunk.Initialize( ATG_FOURCC_RIFF, NULL, m_hFile );
    m_FormatChunk.Initialize( ATG_FOURCC_FORMAT, &m_RiffChunk, m_hFile );
    m_DataChunk.Initialize( ATG_FOURCC_DATA, &m_RiffChunk, m_hFile );
    m_WaveSampleChunk.Initialize( ATG_FOURCC_WSMP, &m_RiffChunk, m_hFile );
    m_SamplerChunk.Initialize( ATG_FOURCC_SMPL, &m_RiffChunk, m_hFile );
    m_DpdsChunk.Initialize( ATG_FOURCC_DPDS, &m_RiffChunk, m_hFile );
    m_SeekChunk.Initialize( ATG_FOURCC_SEEK, &m_RiffChunk, m_hFile );

    HRESULT hr = m_RiffChunk.Open();
	if (FAILED(hr)) {
		DbgPrint("ERR: 2");
		return hr;
	}

    hr = m_FormatChunk.Open();
	if (FAILED(hr)) {
		DbgPrint("ERR: 3");
		return hr;
	}

    hr = m_DataChunk.Open();
	if (FAILED(hr)) {
		DbgPrint("ERR: 4");
		return hr;
	}

    // Wave Sample and Sampler chunks are not required
    m_WaveSampleChunk.Open();
    m_SamplerChunk.Open();

    // Dpds chunk is valid only for XWMA files
    m_DpdsChunk.Open();

    // Seek chunk is valid only for XMA files
    m_SeekChunk.Open();

    // Validate the file type
    FOURCC fccType;
    hr = m_RiffChunk.ReadData( 0, &fccType, sizeof( fccType ), NULL );
	if (FAILED(hr)) {
		DbgPrint("ERR: 5");
		return hr;
	}

	if (ATG_FOURCC_WAVE != fccType && ATG_FOURCC_XWMA != fccType) {
		DbgPrint("ERR: 6");
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	}

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Name: GetFormat()
// Desc: Gets the wave file format.
//--------------------------------------------------------------------------------------
HRESULT WaveFile::GetFormat( WAVEFORMATEXTENSIBLE* pwfxFormat, XMA2WAVEFORMATEX* pXma2Format ) const
{
    assert( pwfxFormat );

    DWORD dwValidSize = m_FormatChunk.GetDataSize();

    // Must be at least as large as a WAVEFORMAT to be valid
    if( dwValidSize < sizeof( WAVEFORMAT ) )
        return HRESULT_FROM_WIN32( ERROR_INVALID_DATA );

    // Read the format chunk into the buffer
    HRESULT hr = m_FormatChunk.ReadData( 0, pwfxFormat, min( dwValidSize, sizeof( WAVEFORMATEXTENSIBLE ) ), NULL );
    if( FAILED( hr ) )
        return hr;

    // Endian conversion for WAVEFORMAT base structure
    pwfxFormat->Format.wFormatTag = __loadshortbytereverse( 0, &pwfxFormat->Format.wFormatTag );
    pwfxFormat->Format.nChannels = __loadshortbytereverse( 0, &pwfxFormat->Format.nChannels );
    pwfxFormat->Format.nSamplesPerSec = __loadwordbytereverse( 0, &pwfxFormat->Format.nSamplesPerSec );
    pwfxFormat->Format.nAvgBytesPerSec = __loadwordbytereverse( 0, &pwfxFormat->Format.nAvgBytesPerSec );
    pwfxFormat->Format.nBlockAlign = __loadshortbytereverse( 0, &pwfxFormat->Format.nBlockAlign );

    switch( pwfxFormat->Format.wFormatTag )
    {
        case WAVE_FORMAT_PCM:
        case WAVE_FORMAT_WMAUDIO3:
        case WAVE_FORMAT_WMAUDIO2:
            if( dwValidSize < sizeof( PCMWAVEFORMAT ) )
                return HRESULT_FROM_WIN32( ERROR_INVALID_DATA );

            pwfxFormat->Format.wBitsPerSample = __loadshortbytereverse( 0, &pwfxFormat->Format.wBitsPerSample );
            break;

        case WAVE_FORMAT_EXTENSIBLE:
            if( dwValidSize < sizeof( WAVEFORMATEXTENSIBLE ) )
                return HRESULT_FROM_WIN32( ERROR_INVALID_DATA );

            // Endianness conversion
            pwfxFormat->Format.wBitsPerSample = __loadshortbytereverse( 0, &pwfxFormat->Format.wBitsPerSample );
            pwfxFormat->Format.cbSize = __loadshortbytereverse( 0, &pwfxFormat->Format.cbSize );

            pwfxFormat->Samples.wReserved = __loadshortbytereverse( 0, &pwfxFormat->Samples.wReserved );
            pwfxFormat->dwChannelMask = __loadwordbytereverse( 0, &pwfxFormat->dwChannelMask );
            pwfxFormat->SubFormat.Data1 = __loadwordbytereverse( 0, &pwfxFormat->SubFormat.Data1 );
            pwfxFormat->SubFormat.Data2 = __loadshortbytereverse( 0, &pwfxFormat->SubFormat.Data2 );
            pwfxFormat->SubFormat.Data3 = __loadshortbytereverse( 0, &pwfxFormat->SubFormat.Data3 );
            // Data4 is a array of char, not needed to convert
            break;

        case WAVE_FORMAT_XMA2:
            if( dwValidSize != sizeof( XMA2WAVEFORMATEX ) )
                return HRESULT_FROM_WIN32( ERROR_INVALID_DATA );

            // Need second structure to store the result
            if( !pXma2Format )
                return HRESULT_FROM_WIN32( ERROR_INVALID_PARAMETER );

            hr = m_FormatChunk.ReadData( 0, pXma2Format, sizeof( XMA2WAVEFORMATEX ), NULL );
            if( FAILED( hr ) )
                return hr;

            // Endian conversion
            {
                pwfxFormat->Format.wBitsPerSample = __loadshortbytereverse( 0, &pwfxFormat->Format.wBitsPerSample );
                pwfxFormat->Format.cbSize = __loadshortbytereverse( 0, &pwfxFormat->Format.cbSize );

                pXma2Format->wfx = pwfxFormat->Format;

                pXma2Format->NumStreams = __loadshortbytereverse( 0, &pXma2Format->NumStreams );
                pXma2Format->ChannelMask = __loadwordbytereverse( 0, &pXma2Format->ChannelMask );
                pXma2Format->SamplesEncoded = __loadwordbytereverse( 0, &pXma2Format->SamplesEncoded );
                pXma2Format->BytesPerBlock = __loadwordbytereverse( 0, &pXma2Format->BytesPerBlock );
                pXma2Format->PlayBegin = __loadwordbytereverse( 0, &pXma2Format->PlayBegin );
                pXma2Format->PlayLength = __loadwordbytereverse( 0, &pXma2Format->PlayLength );
                pXma2Format->LoopBegin = __loadwordbytereverse( 0, &pXma2Format->LoopBegin );
                pXma2Format->LoopLength = __loadwordbytereverse( 0, &pXma2Format->LoopLength );
                pXma2Format->BlockCount = __loadshortbytereverse( 0, &pXma2Format->BlockCount );
            }
            break;

        default:
            // Unsupported!
            return HRESULT_FROM_WIN32( ERROR_NOT_SUPPORTED );
    }

    // Zero out remaining bytes, in case enough bytes were not read
    if( dwValidSize < sizeof( WAVEFORMATEXTENSIBLE ) )
        ZeroMemory( ( BYTE* )pwfxFormat + dwValidSize, sizeof( WAVEFORMATEXTENSIBLE ) - dwValidSize );

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: ReadSampleRaw()
// Desc: Reads data from the audio file. No endianness conversion is performed.
//--------------------------------------------------------------------------------------
HRESULT WaveFile::ReadSampleRaw( DWORD dwPosition, VOID* pBuffer,
                              DWORD dwBufferSize, DWORD* pdwRead ) const
{
    // Don't read past the end of the data chunk
    DWORD dwDuration;
    GetDuration( &dwDuration );

    if( dwPosition + dwBufferSize > dwDuration )
        dwBufferSize = dwDuration - dwPosition;

    HRESULT hr = S_OK;
    if( dwBufferSize )
        hr = m_DataChunk.ReadData( ( LONG )dwPosition, pBuffer, dwBufferSize, NULL );

    if( pdwRead )
        *pdwRead = dwBufferSize;

    return hr;
}


//--------------------------------------------------------------------------------------
// Name: ReadSample()
// Desc: Reads data from the audio file.
//--------------------------------------------------------------------------------------
HRESULT WaveFile::ReadSample( DWORD dwPosition, VOID* pBuffer,
                              DWORD dwBufferSize, DWORD* pdwRead ) const
{
    HRESULT hr = S_OK;
    hr = ReadSampleRaw(dwPosition, pBuffer, dwBufferSize, pdwRead);

    if (FAILED( hr ))
    {
        return hr;
    }

    // Check bit size for endianness conversion.
    WAVEFORMATEXTENSIBLE wfxFormat;
    GetFormat( &wfxFormat );

    //Endianness conversion
    switch( wfxFormat.Format.wFormatTag )
    {
        case WAVE_FORMAT_PCM:
            if( wfxFormat.Format.wBitsPerSample == 16 )
            {
                SHORT* pBufferShort = ( SHORT* )pBuffer;
                for( DWORD i = 0; i < dwBufferSize / sizeof( SHORT ); i++ )
                    pBufferShort[i] = __loadshortbytereverse( 0, &pBufferShort[i] );
            }
            break;

        case WAVE_FORMAT_EXTENSIBLE:
            if( wfxFormat.Format.wBitsPerSample == 16 )
            {
                SHORT* pBufferShort = ( SHORT* )pBuffer;
                for( DWORD i = 0; i < dwBufferSize / sizeof( SHORT ); i++ )
                    pBufferShort[i] = __loadshortbytereverse( 0, &pBufferShort[i] );
            }
            else if( wfxFormat.Format.wBitsPerSample == 32 )
            {
                DWORD* pBufferDWord = ( DWORD* )pBuffer;
                for( DWORD i = 0; i < dwBufferSize / sizeof( DWORD ); i++ )
                    pBufferDWord[i] = __loadwordbytereverse( 0, &pBufferDWord[i] );
            }
            // 24 doesn't need converted...
            break;
    }

    return hr;
}


//--------------------------------------------------------------------------------------
// Name: GetLoopRegion()
// Desc: Gets the loop region, in terms of samples
//--------------------------------------------------------------------------------------
HRESULT WaveFile::GetLoopRegion( DWORD* pdwStart, DWORD* pdwLength ) const
{
    assert( pdwStart != NULL );
    assert( pdwLength != NULL );
    HRESULT hr = S_OK;

    *pdwStart = 0;
    *pdwLength = 0;

    // First, look for a MIDI-style SMPL chunk, then for a DLS-style WSMP chunk.
    BOOL bGotLoopRegion = FALSE;
    if( !bGotLoopRegion && m_SamplerChunk.IsValid() )
    {
        // Read the SAMPLER struct from the chunk
        SAMPLER smpl;
        hr = m_SamplerChunk.ReadData( 0, &smpl, sizeof( SAMPLER ), NULL );
        if( FAILED( hr ) )
            return hr;

        //Endianness conversion
        LONG* pLong = ( LONG* )&smpl;
        for( INT i = 0; i < sizeof( SAMPLER ) / sizeof( LONG ); i++ )
            *pLong++ = __loadwordbytereverse( i, &smpl );


        // Check if the chunk contains any loop regions
        if( smpl.dwNumSampleLoops > 0 )
        {
            SAMPLER_LOOP smpl_loop;
            hr = m_SamplerChunk.ReadData( sizeof( SAMPLER ), &smpl_loop, sizeof( SAMPLER_LOOP ), NULL );
            if( FAILED( hr ) )
                return E_FAIL;

            //Endianness conversion
            pLong = ( LONG* )&smpl_loop;
            for( INT i = 0; i < sizeof( SAMPLER_LOOP ) / sizeof( LONG ); i++ )
                *pLong++ = __loadwordbytereverse( i, &smpl_loop );

            // Documentation on the SMPL chunk indicates that dwStart and
            // dwEnd are stored as byte-offsets, rather than sample counts,
            // but SoundForge stores sample counts, so we'll go with that.
            *pdwStart = smpl_loop.dwStart;
            *pdwLength = smpl_loop.dwEnd - smpl_loop.dwStart + 1;
            bGotLoopRegion = TRUE;
        }
    }

    if( !bGotLoopRegion && m_WaveSampleChunk.IsValid() )
    {
        // Read the WAVESAMPLE struct from the chunk
        WAVESAMPLE ws;
        hr = m_WaveSampleChunk.ReadData( 0, &ws, sizeof( WAVESAMPLE ), NULL );
        if( FAILED( hr ) )
            return hr;

        // Endianness conversion
        ws.dwSize = __loadwordbytereverse( 0, &ws.dwSize );
        ws.UnityNote = __loadshortbytereverse( 0, &ws.UnityNote );
        ws.FineTune = __loadshortbytereverse( 0, &ws.FineTune );
        ws.Gain = __loadwordbytereverse( 0, &ws.Gain );
        ws.dwOptions = __loadwordbytereverse( 0, &ws.dwOptions );
        ws.dwSampleLoops = __loadwordbytereverse( 0, &ws.dwSampleLoops );

        // Check if the chunk contains any loop regions
        if( ws.dwSampleLoops > 0 )
        {
            // Read the loop region
            WAVESAMPLE_LOOP wsl;
            hr = m_WaveSampleChunk.ReadData( ws.dwSize, &wsl, sizeof( WAVESAMPLE_LOOP ), NULL );
            if( FAILED( hr ) )
                return hr;

            //Endianness conversion
            LONG* l = ( LONG* )&wsl;
            for( INT i = 0; i < sizeof( WAVESAMPLE_LOOP ) / sizeof( LONG ); i++ )
                *l++ = __loadwordbytereverse( i, &wsl );

            // Fill output vars with the loop region
            *pdwStart = wsl.dwLoopStart;
            *pdwLength = wsl.dwLoopLength;
            bGotLoopRegion = TRUE;
        }
    }

    // Couldn't find either chunk, or they didn't contain loop points
    if( !bGotLoopRegion )
        return E_FAIL;

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Name: GetLoopRegionBytes()
// Desc: Gets the loop region, in terms of bytes
//--------------------------------------------------------------------------------------
HRESULT WaveFile::GetLoopRegionBytes( DWORD* pdwStart, DWORD* pdwLength ) const
{
    assert( pdwStart != NULL );
    assert( pdwLength != NULL );

    // If no loop region is explicitly specified, loop the entire file
    *pdwStart = 0;
    GetDuration( pdwLength );

    // We'll need the wave format to convert from samples to bytes
    WAVEFORMATEXTENSIBLE wfx;
    if( FAILED( GetFormat( &wfx ) ) )
        return E_FAIL;

    // See if the file contains an embedded loop region
    DWORD dwLoopStartSamples;
    DWORD dwLoopLengthSamples;
    if( FAILED( GetLoopRegion( &dwLoopStartSamples, &dwLoopLengthSamples ) ) )
        return S_FALSE;

    // For PCM, multiply by bytes per sample
    DWORD cbBytesPerSample = wfx.Format.nChannels * wfx.Format.wBitsPerSample / 8;
    *pdwStart = dwLoopStartSamples * cbBytesPerSample;
    *pdwLength = dwLoopLengthSamples * cbBytesPerSample;

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: GetPacketCumulativeBytesSize()
// Desc: Gets the packet count and the total byte size of the 'dpds' chunk.
//--------------------------------------------------------------------------------------
HRESULT WaveFile::GetPacketCumulativeBytesSize( DWORD* pdwPacketCount, DWORD* pdwBufferSize )
{
    assert( m_DpdsChunk.IsValid() );

    if (!m_DpdsChunk.IsValid())
        return E_HANDLE;

    WAVEFORMATEXTENSIBLE wfxFormat;
    GetFormat( &wfxFormat );
    
    // Packet count: length of the 'data' chunk divided by number of bytes per packet
    *pdwPacketCount = Duration() / wfxFormat.Format.nBlockAlign;

    *pdwBufferSize = m_DpdsChunk.GetDataSize();

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: GetPacketCumulativeBytes()
// Desc: Gets the dpds structure data.
//--------------------------------------------------------------------------------------
HRESULT WaveFile::GetPacketCumulativeBytes( DWORD* pdwData )
{
    assert( m_DpdsChunk.IsValid() );
    assert( pdwData != NULL );

    HRESULT hr = S_OK;

    if (!m_DpdsChunk.IsValid())
        return E_HANDLE;

    DWORD dwPacketCount;
    DWORD dwBufferSize;

    hr = GetPacketCumulativeBytesSize( &dwPacketCount, &dwBufferSize );
    if( FAILED( hr ) )
        return hr;

    hr = m_DpdsChunk.ReadData( 0, pdwData, dwBufferSize, NULL );
    if( FAILED( hr ) )
        return hr;

    // Endianness conversion
    #if defined(_XBOX)
        for (DWORD dw=0; dw<dwPacketCount; dw++) {
            pdwData[dw] = _byteswap_ulong(pdwData[dw]);
        }
    #endif

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: GetSeekTableSize()
// Desc: Gets the total byte size of the 'seek' chunk.
//--------------------------------------------------------------------------------------
HRESULT WaveFile::GetSeekTableSize( DWORD* pdwBufferSize )
{
    assert( m_SeekChunk.IsValid() );

    if ( !m_SeekChunk.IsValid() )
        return E_HANDLE;

    *pdwBufferSize = m_SeekChunk.GetDataSize();

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: GetSeekTable()
// Desc: Gets the seek table.
//--------------------------------------------------------------------------------------
HRESULT WaveFile::GetSeekTable( DWORD* pdwData )
{
    assert( m_SeekChunk.IsValid() );
    assert( pdwData != NULL );

    HRESULT hr = S_OK;

    if ( !m_SeekChunk.IsValid() )
        return E_HANDLE;

    DWORD dwBufferSize;

    hr = GetSeekTableSize( &dwBufferSize );
    if( FAILED( hr ) )
        return hr;

    hr = m_SeekChunk.ReadData( 0, pdwData, dwBufferSize, NULL );
    if( FAILED( hr ) )
        return hr;

    return S_OK;
}

//--------------------------------------------------------------------------------------
// Name: Close()
// Desc: Closes the object
//--------------------------------------------------------------------------------------
VOID WaveFile::Close()
{
    if( m_hFile != INVALID_HANDLE_VALUE )
    {
        CloseHandle( m_hFile );
        m_hFile = INVALID_HANDLE_VALUE;
    }
}



// WAVEBANK FUNCTIONS

//--------------------------------------------------------------------------------------
// CWavebank.cpp
//
// Xbox Advanced Technology Group.
// Copyright (C) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
// Name: CWavebank::Ctor
// Desc: 
//--------------------------------------------------------------------------------------
CWavebank::CWavebank() :
	m_dwFileSize(0),
	m_pDataEntry(NULL),
	m_pSeekData(NULL),
	m_hFile(INVALID_HANDLE_VALUE)
{
}


//--------------------------------------------------------------------------------------
// Name: CWavebank::Dtor
// Desc: 
//--------------------------------------------------------------------------------------
CWavebank::~CWavebank()
{
	Close();
}


//--------------------------------------------------------------------------------------
// Name: CWavebank::Open
// Desc: Open the wave bank file and verify
//--------------------------------------------------------------------------------------
HRESULT CWavebank::Open(const CHAR* strFileName)
{
	assert(strFileName != NULL);

	HRESULT hr = S_OK;

	// Open the file
	m_hFile = CreateFile(
		strFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0L,
		NULL);
	if (m_hFile == INVALID_HANDLE_VALUE) {
		DbgPrint("Error opening Wavebank file\n");
		return E_FAIL;
	}

	//
	// Load header
	//
	DWORD cbBytesRead;
	if (!ReadFile(m_hFile, &m_wavebankHeader, sizeof(WAVEBANKHEADER), &cbBytesRead, NULL)) {
		DbgPrint("Couldn't read wavebank header");
		return E_FAIL;
	}

	// Verify header
	if (WAVEBANK_HEADER_SIGNATURE != m_wavebankHeader.dwSignature)
	{
		DbgPrint("Invalid Wavebank format [0x%X]", m_wavebankHeader.dwSignature);
		return E_FAIL;
	}
	if (44 < m_wavebankHeader.dwHeaderVersion)
	{
		DbgPrint(
			"Wave bank version (%i) more recent than this tool supports (%i).",
			m_wavebankHeader.dwHeaderVersion, 46);
		return E_FAIL;
	}

	//
	// Load WAVEBANKDATA
	//
	SetFilePointer(m_hFile, m_wavebankHeader.Segments[WAVEBANK_SEGIDX_BANKDATA].dwOffset, 0, SEEK_SET);
	if (!ReadFile(m_hFile, &m_wavebankData, sizeof(WAVEBANKDATA), &cbBytesRead, NULL)) {
		DbgPrint("Couldn't read wavebank data block");
		return E_FAIL;
	}


	//
	// Load entries
	//
	DWORD cbEntries = m_wavebankHeader.Segments[WAVEBANK_SEGIDX_ENTRYMETADATA].dwLength;
	m_pDataEntry = new WAVEBANKENTRY[cbEntries / sizeof(WAVEBANKENTRY)];
	SetFilePointer(m_hFile, m_wavebankHeader.Segments[WAVEBANK_SEGIDX_ENTRYMETADATA].dwOffset, 0, SEEK_SET);
	if (!ReadFile(m_hFile, m_pDataEntry, cbEntries, &cbBytesRead, NULL)) {
		DbgPrint("Couldn't read wavebank entry metadata\n");
		return E_FAIL;
	}

	//
	// Load seek tables (if any)
	//
	DWORD cbSeekTables = m_wavebankHeader.Segments[WAVEBANK_SEGIDX_SEEKTABLES].dwLength;
	if (cbSeekTables > 0)
	{
		m_pSeekData = new BYTE[cbSeekTables];
		SetFilePointer(m_hFile, m_wavebankHeader.Segments[WAVEBANK_SEGIDX_SEEKTABLES].dwOffset, 0, SEEK_SET);
		if (!ReadFile(m_hFile, m_pSeekData, cbSeekTables, &cbBytesRead, NULL)) {
			DbgPrint("Couldn't read wavebank seek tables\n");
			return E_FAIL;
		}
	}

	return hr;
}


//--------------------------------------------------------------------------------------
// Name: CWavebank::Close
// Desc: 
//--------------------------------------------------------------------------------------
VOID CWavebank::Close()
{
	delete[] m_pDataEntry;
	m_pDataEntry = NULL;

	delete[] m_pSeekData;
	m_pSeekData = NULL;

	CloseHandle(m_hFile);
	m_hFile = INVALID_HANDLE_VALUE;
}


//--------------------------------------------------------------------------------------
// Name: CWavebank::GetEntries
// Desc: Retrieve a number of entries in the wavebank
//--------------------------------------------------------------------------------------
DWORD CWavebank::GetEntryCount(void) const
{
	return m_wavebankData.dwEntryCount;
}


//--------------------------------------------------------------------------------------
// Name: CWavebank::GetFormat
// Desc: Get a wave format
//--------------------------------------------------------------------------------------
HRESULT CWavebank::GetEntryFormat(const DWORD dwEntry, WAVEFORMATEX* pFormat) const
{
	assert(m_pDataEntry != NULL);
	assert(pFormat != NULL);

	// Check entry #
	if (dwEntry >= m_wavebankData.dwEntryCount)
		return S_FALSE;

	const WAVEBANKMINIWAVEFORMAT& miniFmt =
		m_wavebankData.dwFlags & WAVEBANK_FLAGS_COMPACT
		? m_wavebankData.CompactFormat
		: m_pDataEntry[dwEntry].Format;

	switch (miniFmt.wFormatTag)
	{
	case WAVEBANKMINIFORMAT_TAG_PCM:
		pFormat->wFormatTag = WAVE_FORMAT_PCM;
		pFormat->cbSize = 0;
		break;

	case WAVEBANKMINIFORMAT_TAG_XMA:
		pFormat->wFormatTag = WAVE_FORMAT_XMA2;
		pFormat->cbSize = sizeof(XMA2WAVEFORMATEX) - sizeof(WAVEFORMATEX);
		{
			XMA2WAVEFORMATEX* xma2Fmt = reinterpret_cast<XMA2WAVEFORMATEX*>(pFormat);

			WORD wBlockCount = 0;

			// See if we have a seek table and use it for the block count
			if (m_pSeekData != NULL)
			{
				WAVEBANKOFFSET offset = ((WAVEBANKOFFSET*)m_pSeekData)[dwEntry];
				if (offset != 0xffffffff) /* XACTOFFSET_INVALID */
				{
					const DWORD* pEntrySeekTable = reinterpret_cast<const DWORD*>(m_pSeekData + sizeof(WAVEBANKOFFSET)*m_wavebankData.dwEntryCount + offset);
					wBlockCount = (WORD)pEntrySeekTable[0];
				}
			}

			miniFmt.XMA2FillFormatEx(xma2Fmt, wBlockCount, &m_pDataEntry[dwEntry]);
		}
		break;

	case WAVEBANKMINIFORMAT_TAG_WMA:
		pFormat->wFormatTag = (miniFmt.wBitsPerSample & 0x1) ? WAVE_FORMAT_WMAUDIO3 : WAVE_FORMAT_WMAUDIO2;
		pFormat->cbSize = 0;
		break;

	default:
		// WAVEBANKMINIFORMAT_TAG_ADPCM is only valid for Windows
		return E_FAIL;
	}

	pFormat->nChannels = miniFmt.nChannels;
	pFormat->wBitsPerSample = miniFmt.BitsPerSample();
	pFormat->nBlockAlign = (WORD)miniFmt.BlockAlign();
	pFormat->nSamplesPerSec = miniFmt.nSamplesPerSec;
	pFormat->nAvgBytesPerSec = miniFmt.nSamplesPerSec * miniFmt.wBlockAlign;

	return S_OK;
}


//--------------------------------------------------------------------------------------
// Name: CWavebank::GetDuration
// Desc: Get a duration of a wave entry
//--------------------------------------------------------------------------------------
DWORD CWavebank::GetEntryLengthInBytes(const DWORD dwEntry) const
{
	assert(m_pDataEntry != NULL);

	DWORD result = 0;

	// Check entry #
	if (dwEntry < m_wavebankData.dwEntryCount)
	{
		// Fill out format data
		result = m_pDataEntry[dwEntry].PlayRegion.dwLength;
	}

	return result;
}


//--------------------------------------------------------------------------------------
// Name: CWavebank::GetEntryData
// Desc: Retrieve a pointer of sample data
//--------------------------------------------------------------------------------------
HRESULT CWavebank::GetEntryData(const DWORD dwEntry, void** pSample) const
{
	assert(m_pDataEntry != NULL);
	assert(pSample);
	*pSample = NULL;

	HRESULT hr = S_OK;

	// Check entry #
	if (dwEntry >= m_wavebankData.dwEntryCount)
		hr = S_FALSE;

	*pSample = new BYTE[m_pDataEntry[dwEntry].PlayRegion.dwLength];

	DWORD cbRead;
	SetFilePointer(m_hFile, m_pDataEntry[dwEntry].PlayRegion.dwOffset, 0L, SEEK_SET);
	if (!ReadFile(m_hFile, (*pSample), m_pDataEntry[dwEntry].PlayRegion.dwLength, &cbRead, NULL))
		hr = E_FAIL;

	if (FAILED(hr))
	{
		delete *pSample;
		pSample = NULL;
	}
	return hr;
}

//--------------------------------------------------------------------------------------
// Name: CWavebank::GetEntryOffset
// Desc: Retrieve a pointer of sample data
//--------------------------------------------------------------------------------------
DWORD CWavebank::GetEntryOffset(const DWORD dwEntry) const
{
	assert(m_pDataEntry != NULL);

	DWORD result = 0;

	// Check entry #
	if (dwEntry < m_wavebankData.dwEntryCount)
	{
		result =
			0//m_wavebankHeader.Segments[WAVEBANK_SEGIDX_ENTRYWAVEDATA].dwOffset
			+ m_pDataEntry[dwEntry].PlayRegion.dwOffset;
	}
	return result;
}


};
