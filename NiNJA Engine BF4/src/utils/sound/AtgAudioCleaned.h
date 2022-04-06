#pragma once
#ifndef _ATG_AUDIO_CLEANED_H
#define _ATG_AUDIO_CLEANED_H

namespace AtgCleaned
{
//--------------------------------------------------------------------------------------
// FourCC definitions
//--------------------------------------------------------------------------------------
const DWORD ATG_FOURCC_RIFF = 'RIFF';
const DWORD ATG_FOURCC_WAVE = 'WAVE';
const DWORD ATG_FOURCC_XWMA = 'XWMA';
const DWORD ATG_FOURCC_DPDS = 'dpds';
const DWORD ATG_FOURCC_SEEK = 'seek';
const DWORD ATG_FOURCC_FORMAT = 'fmt ';
const DWORD ATG_FOURCC_DATA = 'data';
const DWORD ATG_FOURCC_WSMP = 'wsmp';
const DWORD ATG_FOURCC_SMPL = 'lsmp';

#define RIFFCHUNK_FLAGS_VALID   0x00000001

//--------------------------------------------------------------------------------------
// Misc type definitions
//--------------------------------------------------------------------------------------
typedef DWORD FOURCC, *PFOURCC, *LPFOURCC;

//--------------------------------------------------------------------------------------
// For parsing WAV files
//--------------------------------------------------------------------------------------
struct RIFFHEADER
{
    FOURCC fccChunkId;
    DWORD dwDataSize;
};

//--------------------------------------------------------------------------------------
// RIFF chunk type that contains loop point information
//--------------------------------------------------------------------------------------
struct WAVESAMPLE
{
    DWORD dwSize;
    WORD UnityNote;
    SHORT FineTune;
    LONG Gain;
    DWORD dwOptions;
    DWORD dwSampleLoops;
};


//--------------------------------------------------------------------------------------
// Loop point (contained in WSMP chunk)
//--------------------------------------------------------------------------------------
struct WAVESAMPLE_LOOP
{
    DWORD dwSize;
    DWORD dwLoopType;
    DWORD dwLoopStart;
    DWORD dwLoopLength;
};


//--------------------------------------------------------------------------------------
// RIFF chunk that may contain loop point information
//--------------------------------------------------------------------------------------
struct SAMPLER
{
    DWORD dwManufacturer;
    DWORD dwProduct;
    DWORD dwSamplePeriod;
    DWORD dwMIDIUnityNote;
    DWORD dwMIDIPitchFraction;
    DWORD dwSMPTEFormat;
    DWORD dwSMPTEOffset;
    DWORD dwNumSampleLoops;
    DWORD dwSamplerData;
};


//--------------------------------------------------------------------------------------
// Loop point contained in SMPL chunk
//--------------------------------------------------------------------------------------
struct SAMPLER_LOOP
{
    DWORD dwCuePointID;
    DWORD dwType;
    DWORD dwStart;
    DWORD dwEnd;
    DWORD dwFraction;
    DWORD dwPlayCount;
};

//--------------------------------------------------------------------------------------
// Name: class RiffChunk
// Desc: RIFF chunk utility class
//--------------------------------------------------------------------------------------
class RiffChunk
{
    FOURCC m_fccChunkId;       // Chunk identifier
    const RiffChunk* m_pParentChunk;     // Parent chunk
    HANDLE m_hFile;
    DWORD m_dwDataOffset;     // Chunk data offset
    DWORD m_dwDataSize;       // Chunk data size
    DWORD m_dwFlags;          // Chunk flags

public:
            RiffChunk();

    // Initialization
    VOID    Initialize( FOURCC fccChunkId, const RiffChunk* pParentChunk,
                        HANDLE hFile );
    HRESULT Open();
    BOOL    IsValid() const
    {
        return !!( m_dwFlags & RIFFCHUNK_FLAGS_VALID );
    }

    // Data
    HRESULT ReadData( LONG lOffset, VOID* pData, DWORD dwDataSize, OVERLAPPED* pOL ) const;

    // Chunk information
    FOURCC  GetChunkId() const
    {
        return m_fccChunkId;
    }
    DWORD   GetDataSize() const
    {
        return m_dwDataSize;
    }

private:
    // prevent copying so that we don't have to duplicate file handles
            RiffChunk( const RiffChunk& );
    RiffChunk& operator =( const RiffChunk& );
};

//--------------------------------------------------------------------------------------
// Name: class WaveFile
// Desc: Wave file utility class
//--------------------------------------------------------------------------------------
class WaveFile
{
    HANDLE m_hFile;            // File handle
    RiffChunk m_RiffChunk;        // RIFF chunk
    RiffChunk m_FormatChunk;      // Format chunk
    RiffChunk m_WaveSampleChunk;  // Wave Sample chunk
    RiffChunk m_SamplerChunk;     // Sampler chunk
    RiffChunk m_DpdsChunk;        // Packet cumulative bytes chunk (XWMA only)
    RiffChunk m_SeekChunk;        // Seek chunk (XMA only)

public:
	RiffChunk m_DataChunk;        // Data chunk
            WaveFile();
            ~WaveFile();

    // Initialization
			HRESULT Open(const CHAR* strFileName);
			HRESULT OpenWithHandle(HANDLE hFile);
    VOID    Close();

    // File format
    HRESULT GetFormat( WAVEFORMATEXTENSIBLE* pwfxFormat, XMA2WAVEFORMATEX* pXma2Format = NULL ) const;

    // File data
    HRESULT ReadSample( DWORD dwPosition, VOID* pBuffer, DWORD dwBufferSize,
                        DWORD* pdwRead ) const;

    // File data - no endianness conversion is performed
    HRESULT ReadSampleRaw( DWORD dwPosition, VOID* pBuffer, DWORD dwBufferSize,
                        DWORD* pdwRead ) const;

    // Loop region
    HRESULT GetLoopRegion( DWORD* pdwStart, DWORD* pdwLength ) const;
    HRESULT GetLoopRegionBytes( DWORD* pdwStart, DWORD* pdwLength ) const;

    // File properties
    VOID    GetDuration( DWORD* pdwDuration ) const
    {
        *pdwDuration = m_DataChunk.GetDataSize();
    }
    DWORD   Duration() const
    {
        return m_DataChunk.GetDataSize();
    }

    // The 'dpds' chunk - packet cumulative bytes. Returns the packet count and the total structure size, in bytes.
    // The packet count is for the client to index the structure returned by GetPacketCumulativeBytes.
    // The buffer size is the total size, in bytes, of the structure. The client can use this value to allocate a pointer to
    // pass to GetPacketCumulativeBytes.
    // This function should be used with XWMA files only.
    HRESULT GetPacketCumulativeBytesSize( DWORD* pdwPacketCount, DWORD* pdwBufferSize );

    // The 'dpds' chunk - packet cumulative bytes. Returns the structure data.
    // 'pdwData' should point to a memory block large enough to hold all the data. The exact number of bytes
    // required can be obtained by calling GetPacketCumulativeBytes.
    // This function should be used with XWMA files only.
    HRESULT GetPacketCumulativeBytes( DWORD* pdwData );

    // The 'seek' chunk - seek table. Returns the total structure size, in bytes.
    // The buffer size is the total size, in bytes, of the structure. The client can use this value to allocate a pointer to
    // pass to GetSeekTable.
    // This function should be used with XMA files only.
    HRESULT GetSeekTableSize( DWORD* pdwBufferSize );

    // The 'seek' chunk - seek table. Returns the structure data.
    // 'pdwData' should point to a memory block large enough to hold all the data. The exact number of bytes
    // required can be obtained by calling GetSeekTableSize.
    // This function should be used with XMA files only.
    HRESULT GetSeekTable( DWORD* pdwData );

private:
    // prevent copying so that we don't have to duplicate file handles
            WaveFile( const WaveFile& );
    WaveFile& operator =( const WaveFile& );
};


//--------------------------------------------------------------------------------------
// Name: CWavebank
// Desc: Class to handle the Wavebank file
//--------------------------------------------------------------------------------------
class CWavebank {
public:
	CWavebank();
	~CWavebank();

	// Initialization
	// Open a wavebank. The function allocates buffer for a wavebank
	HRESULT Open(const CHAR* strFileName);
	// Close the wavebank
	VOID    Close();
	// Get # of wave entries in the wavebank
	DWORD   GetEntryCount(void) const;
	// Get a wave format of the wave entry
	HRESULT GetEntryFormat(const DWORD dwEntry, WAVEFORMATEX* pFormat) const;
	// Get a duration of the wave entry
	DWORD   GetEntryLengthInBytes(const DWORD dwEntry) const;
	// Get offset of the wave entry in the bank file
	DWORD   GetEntryOffset(const DWORD dwEntry) const;
	// Get samples of the wave entry
	HRESULT GetEntryData(const DWORD dwEntry, void** pData) const;
private:
	DWORD m_dwFileSize;
	WAVEBANKHEADER m_wavebankHeader;
	WAVEBANKDATA m_wavebankData;
	WAVEBANKENTRY* m_pDataEntry;
	BYTE* m_pSeekData;
	HANDLE m_hFile;
};

};


#endif