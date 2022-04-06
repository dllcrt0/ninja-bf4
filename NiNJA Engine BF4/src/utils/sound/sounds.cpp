#include "stdafx.h"

#define STREAMING_BUFFER_SIZE 65536
#define MAX_BUFFER_COUNT 3

Sounds pSounds;

struct StreamingVoiceContext : public IXAudio2VoiceCallback {
	void OnVoiceProcessingPassStart(UINT32 BytesRequired) {}
	void OnVoiceProcessingPassEnd() {}
	void OnStreamEnd() {}
	void OnBufferStart(void*) {}
	void OnBufferEnd(void*) { SetEvent(hBufferEndEvent); }
	void OnLoopEnd(void*) {}
	void OnVoiceError(void*, HRESULT) {}

	HANDLE hBufferEndEvent;

	StreamingVoiceContext() : hBufferEndEvent(CreateEvent(NULL, FALSE, FALSE, NULL)) {}
	~StreamingVoiceContext() { CloseHandle(hBufferEndEvent); }
};

bool Sounds::Initialize(BOOL bFullInitialize) {
	if (bFullInitialize) {
		if (pUtils.MountPath(MOUNT_NAME, pUtils.GetMountPath(), FALSE) == 0) {
			DbgPrint("Mounted path successfully!");
		} else {
			DbgPrint("Failed to mount path!");
			return false;
		}
	}

	HRESULT hr;
	if (FAILED(hr = XAudio2Create(&pXAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR))) {
		DbgPrint("Failed to init xaudio2 object!");
		return false;
	}

	if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice, 6,
		48000, 0, 0, NULL))) {
		DbgPrint("Failed to init mastering voice object!");
		return false;
	}

	vSoundQueue.clear();

	bInitialized = true;
	return true;
}

HRESULT GetBlockBoundaries(DWORD nBlockCount, DWORD *pSeekTable, DWORD startSample, DWORD length, DWORD *pnStartingBlock, DWORD *pnStartingBlockPlayBegin, DWORD *pnEndingBlock, DWORD *pnEndingBlockPlayLength) {
	HRESULT hr = S_OK;

	if (FAILED(hr = GetXmaBlockContainingSample(nBlockCount, pSeekTable, startSample, pnStartingBlock, pnStartingBlockPlayBegin))) {
		return hr;
	}

	DWORD endSample = startSample + length - 1;
	DWORD endSampleOffset;

	if (FAILED(hr = GetXmaBlockContainingSample(nBlockCount, pSeekTable, endSample, pnEndingBlock, &endSampleOffset))) {
		return hr;
	}

	*pnEndingBlockPlayLength = endSampleOffset + 1;

	if (*pnStartingBlock == *pnEndingBlock) {
		*pnEndingBlockPlayLength = (endSampleOffset - *pnStartingBlockPlayBegin) + 1;
	}

	return hr;
}

VOID Sounds::Shutdown(BOOL bFullShutdown) {
	if (pMasteringVoice) {
		pMasteringVoice->DestroyVoice();
		pMasteringVoice = NULL;
	}

	SAFE_RELEASE(pXAudio2);
	if (bFullShutdown) {
		bInitialized = false;
	}
}

VOID Sounds::Queue(eSoundTypes soundType, DWORD dwSoundIndex) {
	vSoundQueue.push_back(SoundInfo(soundType, dwSoundIndex));
}

VOID Sounds::QueueBF(string name) {
	DbgPrint("Queued %s", name.c_str());
	vBFSoundQueue.push_back(name);
}

VOID Sounds::RunSync() {
	if (!bInitialized) {
		Initialize();
	}
}

VOID Sounds::PlayXMA2StreamedHandle(IXAudio2 * pXaudio2, HANDLE hFile, DWORD dwPlayBeginOverride) {
	HRESULT hr = S_OK, hrWavOpen = S_OK;
	AtgCleaned::WaveFile WaveFile;
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XMA2WAVEFORMATEX xma2 = { 0 };
	DWORD dwSeekTableSize = 0;
	IXAudio2SourceVoice* pSourceVoice;
	StreamingVoiceContext voiceContext;

	if (FAILED(hrWavOpen = WaveFile.OpenWithHandle(hFile))) {
		DbgPrint("Error %#X opening WAV file", hr);
		return;
	}

	if (FAILED(hr = WaveFile.GetFormat(&wfx, &xma2)))
		DbgPrint("Error %#X reading WAV format", hr);

	if (wfx.Format.wFormatTag != WAVE_FORMAT_XMA2)
		DbgPrint("Error - Expected an XMA2 XAudio2 compatible file");

	if (FAILED(hr = WaveFile.GetSeekTableSize(&dwSeekTableSize)))
		DbgPrint("Error %#X reading seek table size", hr);

	DWORD* pSeekTable = (DWORD*)new BYTE[dwSeekTableSize];
	if (FAILED(hr = WaveFile.GetSeekTable(pSeekTable)))
		DbgPrint("Error %#X reading seek table", hr);

	if (FAILED(hr = pXaudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&xma2, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &voiceContext))) {
		DbgPrint("Error %#X creating source voice", hr);
		WaveFile.Close();
		goto jExit;
	}

	pSourceVoice->SetVolume(5.f);

	pSourceVoice->Start(0, 0);

	BYTE *buffers[MAX_BUFFER_COUNT];
	DWORD dwSize = xma2.BytesPerBlock;
	for (DWORD i = 0; i < MAX_BUFFER_COUNT; i++) {
		DWORD dwSize = xma2.BytesPerBlock;
		// XMA packets must be 2KB aligned
		buffers[i] = (BYTE*)XPhysicalAlloc(dwSize, MAXULONG_PTR, 2048, PAGE_READWRITE);
		if (!buffers[i]) {
			DbgPrint("Failed to allocate memory for sound buffer! Aborting!");

			for (DWORD b = 0; b < MAX_BUFFER_COUNT; b++) {
				if (buffers[b]) {
					XSAFE_FREE(buffers[b]);
				}
			}
			goto jExit;
		}
	}

	const DWORD dwRampUpSamples = XMA_SAMPLES_PER_SUBFRAME * 3;
	if (xma2.LoopBegin == dwRampUpSamples && (xma2.PlayLength + dwRampUpSamples) > xma2.LoopLength) {
		xma2.PlayBegin = xma2.LoopBegin;
		xma2.PlayLength = xma2.LoopLength;
	}

	DWORD playRegionStartingBlock;
	DWORD playRegionStartingBlockPlayBegin;
	DWORD playRegionEndingBlock;
	DWORD playRegionEndingBlockPlayLength;

	if (FAILED(hr = GetBlockBoundaries(xma2.BlockCount, pSeekTable, xma2.PlayBegin, xma2.PlayLength, &playRegionStartingBlock, &playRegionStartingBlockPlayBegin, &playRegionEndingBlock, &playRegionEndingBlockPlayLength))) {
		DbgPrint("Could not find block boundaries for play region\n");
	}

	DWORD loopRegionStartingBlock = 0;
	DWORD loopRegionStartingBlockPlayBegin = 0;
	DWORD loopRegionEndingBlock = xma2.BlockCount - 1;
	DWORD loopRegionEndingBlockPlayLength = 0;

	if (xma2.LoopBegin > 0 || xma2.LoopCount > 0) {
		if (FAILED(hr = GetBlockBoundaries(xma2.BlockCount, pSeekTable, xma2.LoopBegin, xma2.LoopLength, &loopRegionStartingBlock, &loopRegionStartingBlockPlayBegin, &loopRegionEndingBlock, &loopRegionEndingBlockPlayLength))) {
			DbgPrint("Could not find block boundaries for play region\n");
		}
	}

	DWORD currentDiskReadBuffer = 0;
	DWORD nextBlock = 0;
	DWORD currentPosition = playRegionStartingBlock * xma2.BytesPerBlock;
	DWORD endPosition = min(WaveFile.Duration(), (playRegionEndingBlock + 1) * xma2.BytesPerBlock);
	DWORD currentBlock = playRegionStartingBlock;
	DWORD loopsRemaining = xma2.LoopCount;

	if (dwPlayBeginOverride > 0) {
		DWORD overridePlayBeginBlock;
		DWORD overridePlayBeginOffset;
		if (FAILED(hr = GetXmaBlockContainingSample(xma2.BlockCount, pSeekTable, dwPlayBeginOverride, &overridePlayBeginBlock, &overridePlayBeginOffset))) {
			DbgPrint("Error %#X processing value for 'PlayBegin' override\n", hr);
		}
		currentPosition = overridePlayBeginBlock * xma2.BytesPerBlock;
		dwPlayBeginOverride = overridePlayBeginOffset;
	}

	while (currentPosition < endPosition) {

		if (bShutdown || NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
			goto jExit;
		}

		DWORD cbValid = min(xma2.BytesPerBlock, endPosition - currentPosition);
		DWORD cbBytesRead;

		if (FAILED(hr = WaveFile.ReadSample(currentPosition, buffers[currentDiskReadBuffer], cbValid, &cbBytesRead))) {
			DbgPrint("Error %#X reading XMA data\n", hr);
		} else if (cbBytesRead != cbValid) {
			DbgPrint("Unexpected number of bytes read (%d). Expected: %d.\n", cbBytesRead, cbValid);
		}

		currentPosition += cbValid;

		XAUDIO2_VOICE_STATE state;
		for (;;) {
			if (bShutdown || NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
				goto jExit;
			}

			pSourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
			if (state.BuffersQueued < MAX_BUFFER_COUNT - 1)
				break;

			WaitForSingleObject(voiceContext.hBufferEndEvent, INFINITE);
		}


		XAUDIO2_BUFFER buf = { 0 };
		buf.AudioBytes = cbValid;
		buf.pAudioData = buffers[currentDiskReadBuffer];
		if (currentPosition >= endPosition && loopsRemaining == 0)
			buf.Flags = XAUDIO2_END_OF_STREAM;

		nextBlock = currentBlock + 1;

		if (xma2.LoopCount > 0 && currentBlock == loopRegionStartingBlock && loopsRemaining != xma2.LoopCount) {
			buf.PlayBegin = loopRegionStartingBlockPlayBegin;

			if (xma2.LoopCount == XMA_INFINITE_LOOP) {
				loopsRemaining = XMA_INFINITE_LOOP;
			}
		}

		if (xma2.LoopCount > 0 && currentBlock == loopRegionEndingBlock && loopsRemaining > 0) {
			if (currentBlock == loopRegionStartingBlock && loopsRemaining == xma2.LoopCount) {
				buf.PlayLength = (loopRegionStartingBlockPlayBegin + loopRegionEndingBlockPlayLength) - buf.PlayBegin;
			} else {
				buf.PlayLength = loopRegionEndingBlockPlayLength;
			}

			currentPosition = loopRegionStartingBlock * xma2.BytesPerBlock;
			nextBlock = loopRegionStartingBlock;
			loopsRemaining--;
		}

		if (currentBlock == playRegionStartingBlock && buf.PlayBegin == 0) {
			buf.PlayBegin = playRegionStartingBlockPlayBegin;
		}

		if (currentBlock == playRegionEndingBlock && buf.PlayLength == 0 && (buf.PlayBegin + playRegionEndingBlockPlayLength < pSeekTable[playRegionEndingBlock])) {
			buf.PlayLength = (playRegionEndingBlockPlayLength % 128) ? 0 : playRegionEndingBlockPlayLength;
		}

		if (dwPlayBeginOverride > 0) {
			if (dwPlayBeginOverride > buf.PlayBegin + buf.PlayLength) {
				buf.PlayLength = 0;
				nextBlock = currentBlock + 1;
			} else if (buf.PlayLength > 0) {
				buf.PlayLength -= (dwPlayBeginOverride - buf.PlayBegin);
			}
			buf.PlayBegin = dwPlayBeginOverride;
			dwPlayBeginOverride = 0;
		}

		pSourceVoice->SubmitSourceBuffer(&buf);

		currentDiskReadBuffer++;
		currentDiskReadBuffer %= MAX_BUFFER_COUNT;

		currentBlock = nextBlock;
	}

	XAUDIO2_VOICE_STATE state;
	for (;;) {
		pSourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
		if (!state.BuffersQueued)
			break;

		WaitForSingleObject(voiceContext.hBufferEndEvent, INFINITE);
	}

	// Clean up
	if (pSourceVoice) {
		pSourceVoice->Stop(0);
		pSourceVoice->DestroyVoice();
	}
jExit:

	WaveFile.Close();

	SAFE_DELETE(pSeekTable);

	for (int i = 0; i < MAX_BUFFER_COUNT; i++) {
		XSAFE_FREE(buffers[i]);
	}
}

VOID Sounds::PlayXMA2Streamed(IXAudio2* pXaudio2, const char* szFilename, DWORD dwPlayBeginOverride) {
	DbgPrint("PLAYING %s", szFilename);
	HRESULT hr = S_OK, hrWavOpen = S_OK;
	AtgCleaned::WaveFile WaveFile;
	WAVEFORMATEXTENSIBLE wfx = { 0 };
	XMA2WAVEFORMATEX xma2 = { 0 };
	DWORD dwSeekTableSize = 0;
	IXAudio2SourceVoice* pSourceVoice;
	StreamingVoiceContext voiceContext;

	if (FAILED(hrWavOpen = WaveFile.Open(szFilename))) {
		DbgPrint("Error %#X opening WAV file", hr);
		return;
	}

	if (FAILED(hr = WaveFile.GetFormat(&wfx, &xma2)))
		DbgPrint("Error %#X reading WAV format", hr);

	if (wfx.Format.wFormatTag != WAVE_FORMAT_XMA2)
		DbgPrint("Error - Expected an XMA2 XAudio2 compatible file");

	if (FAILED(hr = WaveFile.GetSeekTableSize(&dwSeekTableSize)))
		DbgPrint("Error %#X reading seek table size", hr);

	DWORD* pSeekTable = (DWORD*)new BYTE[dwSeekTableSize];
	if (FAILED(hr = WaveFile.GetSeekTable(pSeekTable)))
		DbgPrint("Error %#X reading seek table", hr);

	if (FAILED(hr = pXaudio2->CreateSourceVoice(&pSourceVoice, (WAVEFORMATEX*)&xma2, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &voiceContext))) {
		DbgPrint("Error %#X creating source voice", hr);
		WaveFile.Close();
		goto jExit;
	}

	pSourceVoice->SetVolume(5.f);

	pSourceVoice->Start(0, 0);

	BYTE *buffers[MAX_BUFFER_COUNT];
	DWORD dwSize = xma2.BytesPerBlock;
	for (DWORD i = 0; i < MAX_BUFFER_COUNT; i++) {
		DWORD dwSize = xma2.BytesPerBlock;
		// XMA packets must be 2KB aligned
		buffers[i] = (BYTE*)XPhysicalAlloc(dwSize, MAXULONG_PTR, 2048, PAGE_READWRITE);
		if (!buffers[i]) {
			DbgPrint("Failed to allocate memory for sound buffer! Aborting!");

			for (DWORD b = 0; b < MAX_BUFFER_COUNT; b++) {
				if (buffers[b]) {
					XSAFE_FREE(buffers[b]);
				}
			}
			goto jExit;
		}
	}

	const DWORD dwRampUpSamples = XMA_SAMPLES_PER_SUBFRAME * 3;
	if (xma2.LoopBegin == dwRampUpSamples && (xma2.PlayLength + dwRampUpSamples) > xma2.LoopLength) {
		xma2.PlayBegin = xma2.LoopBegin;
		xma2.PlayLength = xma2.LoopLength;
	}

	DWORD playRegionStartingBlock;
	DWORD playRegionStartingBlockPlayBegin;
	DWORD playRegionEndingBlock;
	DWORD playRegionEndingBlockPlayLength;

	if (FAILED(hr = GetBlockBoundaries(xma2.BlockCount, pSeekTable, xma2.PlayBegin, xma2.PlayLength, &playRegionStartingBlock, &playRegionStartingBlockPlayBegin, &playRegionEndingBlock, &playRegionEndingBlockPlayLength))) {
		DbgPrint("Could not find block boundaries for play region\n");
	}

	DWORD loopRegionStartingBlock = 0;
	DWORD loopRegionStartingBlockPlayBegin = 0;
	DWORD loopRegionEndingBlock = xma2.BlockCount - 1;
	DWORD loopRegionEndingBlockPlayLength = 0;

	if (xma2.LoopBegin > 0 || xma2.LoopCount > 0) {
		if (FAILED(hr = GetBlockBoundaries(xma2.BlockCount, pSeekTable, xma2.LoopBegin, xma2.LoopLength, &loopRegionStartingBlock, &loopRegionStartingBlockPlayBegin, &loopRegionEndingBlock, &loopRegionEndingBlockPlayLength))) {
			DbgPrint("Could not find block boundaries for play region\n");
		}
	}

	DWORD currentDiskReadBuffer = 0;
	DWORD nextBlock = 0;
	DWORD currentPosition = playRegionStartingBlock * xma2.BytesPerBlock;
	DWORD endPosition = min(WaveFile.Duration(), (playRegionEndingBlock + 1) * xma2.BytesPerBlock);
	DWORD currentBlock = playRegionStartingBlock;
	DWORD loopsRemaining = xma2.LoopCount;

	if (dwPlayBeginOverride > 0) {
		DWORD overridePlayBeginBlock;
		DWORD overridePlayBeginOffset;
		if (FAILED(hr = GetXmaBlockContainingSample(xma2.BlockCount, pSeekTable, dwPlayBeginOverride, &overridePlayBeginBlock, &overridePlayBeginOffset))) {
			DbgPrint("Error %#X processing value for 'PlayBegin' override\n", hr);
		}
		currentPosition = overridePlayBeginBlock * xma2.BytesPerBlock;
		dwPlayBeginOverride = overridePlayBeginOffset;
	}

	while (currentPosition < endPosition) {

		if (bShutdown || NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
			goto jExit;
		}

		DWORD cbValid = min(xma2.BytesPerBlock, endPosition - currentPosition);
		DWORD cbBytesRead;

		if (FAILED(hr = WaveFile.ReadSample(currentPosition, buffers[currentDiskReadBuffer], cbValid, &cbBytesRead))) {
			DbgPrint("Error %#X reading XMA data\n", hr);
		} else if (cbBytesRead != cbValid) {
			DbgPrint("Unexpected number of bytes read (%d). Expected: %d.\n", cbBytesRead, cbValid);
		}

		currentPosition += cbValid;

		XAUDIO2_VOICE_STATE state;
		for (;;) {
			if (bShutdown || NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload) {
				goto jExit;
			}

			pSourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
			if (state.BuffersQueued < MAX_BUFFER_COUNT - 1)
				break;

			WaitForSingleObject(voiceContext.hBufferEndEvent, INFINITE);
		}


		XAUDIO2_BUFFER buf = { 0 };
		buf.AudioBytes = cbValid;
		buf.pAudioData = buffers[currentDiskReadBuffer];
		if (currentPosition >= endPosition && loopsRemaining == 0)
			buf.Flags = XAUDIO2_END_OF_STREAM;

		nextBlock = currentBlock + 1;

		if (xma2.LoopCount > 0 && currentBlock == loopRegionStartingBlock && loopsRemaining != xma2.LoopCount) {
			buf.PlayBegin = loopRegionStartingBlockPlayBegin;

			if (xma2.LoopCount == XMA_INFINITE_LOOP) {
				loopsRemaining = XMA_INFINITE_LOOP;
			}
		}

		if (xma2.LoopCount > 0 && currentBlock == loopRegionEndingBlock && loopsRemaining > 0) {
			if (currentBlock == loopRegionStartingBlock && loopsRemaining == xma2.LoopCount) {
				buf.PlayLength = (loopRegionStartingBlockPlayBegin + loopRegionEndingBlockPlayLength) - buf.PlayBegin;
			} else {
				buf.PlayLength = loopRegionEndingBlockPlayLength;
			}

			currentPosition = loopRegionStartingBlock * xma2.BytesPerBlock;
			nextBlock = loopRegionStartingBlock;
			loopsRemaining--;
		}

		if (currentBlock == playRegionStartingBlock && buf.PlayBegin == 0) {
			buf.PlayBegin = playRegionStartingBlockPlayBegin;
		}

		if (currentBlock == playRegionEndingBlock && buf.PlayLength == 0 && (buf.PlayBegin + playRegionEndingBlockPlayLength < pSeekTable[playRegionEndingBlock])) {
			buf.PlayLength = (playRegionEndingBlockPlayLength % 128) ? 0 : playRegionEndingBlockPlayLength;
		}

		if (dwPlayBeginOverride > 0) {
			if (dwPlayBeginOverride > buf.PlayBegin + buf.PlayLength) {
				buf.PlayLength = 0;
				nextBlock = currentBlock + 1;
			} else if (buf.PlayLength > 0) {
				buf.PlayLength -= (dwPlayBeginOverride - buf.PlayBegin);
			}
			buf.PlayBegin = dwPlayBeginOverride;
			dwPlayBeginOverride = 0;
		}

		pSourceVoice->SubmitSourceBuffer(&buf);

		currentDiskReadBuffer++;
		currentDiskReadBuffer %= MAX_BUFFER_COUNT;

		currentBlock = nextBlock;
	}

	XAUDIO2_VOICE_STATE state;
	for (;;) {
		pSourceVoice->GetState(&state, XAUDIO2_VOICE_NOSAMPLESPLAYED);
		if (!state.BuffersQueued)
			break;

		WaitForSingleObject(voiceContext.hBufferEndEvent, INFINITE);
	}

	// Clean up
	if (pSourceVoice) {
		pSourceVoice->Stop(0);
		pSourceVoice->DestroyVoice();
	}
jExit:

	WaveFile.Close();

	SAFE_DELETE(pSeekTable);

	for (int i = 0; i < MAX_BUFFER_COUNT; i++) {
		XSAFE_FREE(buffers[i]);
	}
	//DbgPrint("Exited");
}

struct OverlapData {
	HANDLE hFile;
	Sounds* pSounds;
};

static DWORD WINAPI RunOverlap(void* pData) {
	OverlapData* lap = (OverlapData*)pData;
	lap->pSounds->PlayXMA2StreamedHandle(lap->pSounds->pXAudio2, lap->hFile);
	delete lap;
	return 0;
}

static DWORD WINAPI Run(void *pParent) {
	Sounds *pSounds = (Sounds*)pParent;
	NiNJA.bSoundLoopRunning = TRUE;
	// Initialize our sounds
	if (pSounds->Initialize()) {
		DbgPrint("Sounds initialized!");
		// Run our loop
		while (!(NiNJA.bTerminateProcess || NiNJA.bShutdownCheats || NiNJA.bExternUnload)) {
			// If we enter the game, and our audio has been marked as "shutdown" (this means we partially uninitialized before)
			if (pSounds->bShutdown) {
				if (pSounds->Initialize(FALSE)) {
					DbgPrint("Sounds reinitialized!");
					pSounds->bShutdown = false;
				} else {
					DbgPrint("Failed to reinitialize sounds! Aborting sound thread!");
					pSounds->bInitialized = false;
					break;
				}
			}
			// Otherwise, if we aren't in the game and our sounds have been initialized, and the shutdown flag hasnt been set
			else  {
				if (pSounds->vBFSoundQueue.size() > 0) {
					// Setup our sound path
					CHAR pSoundPath[MAX_PATH];
					RtlSprintf(pSoundPath, "NINJA:\\ninjacache\\media\\sounds\\%s.xma", pSounds->vBFSoundQueue[0].c_str());
					if (pUtils.FileExists(pSoundPath)) {
						// Play it back
						pSounds->PlayXMA2Streamed(pSounds->pXAudio2, pSoundPath);

						/*OverlapData* data = new OverlapData();
						data->pSounds = pSounds;
						data->hFile = CreateFile(pSoundPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						
						CreateThread(0, 0, RunOverlap, data, 0, 0);*/
					} else {
						DbgPrint("%s doesn't exist", pSoundPath);
					}

					pSounds->vBFSoundQueue.erase(pSounds->vBFSoundQueue.begin());
				}

				if (pSounds->vSoundQueue.size() > 0) {
					// Setup our sound path
					CHAR pSoundPath[MAX_PATH];
					RtlSprintf(pSoundPath, "NINJA:\\ninjacache\\media\\sounds\\%s\\%i.xma", pSoundNames[pSounds->vSoundQueue[0].dwSoundType], pSounds->vSoundQueue[0].dwSoundIndex);

					if (pUtils.FileExists(pSoundPath)) {
						// Play it back
						pSounds->PlayXMA2Streamed(pSounds->pXAudio2, pSoundPath);
					} else {
						DbgPrint("%s doesn't exist", pSoundPath);
					}

					pSounds->vSoundQueue.erase(pSounds->vSoundQueue.begin());
				}
			}
			Sleep(100);
		}

		// Shutdown our sounds
		pSounds->Shutdown(TRUE);
	}

	DbgPrint("Audio thread has exited!");
	NiNJA.bSoundLoopRunning = FALSE;
	return 0;
}

bool Sounds::Start() {
	if (!bInitialized) {
		hThread = CreateThread(0, 16000, Run, this, CREATE_SUSPENDED, &dwThreadId);

		if (hThread) {
			XSetThreadProcessor(hThread, 4);
			SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL);
			ResumeThread(hThread);
			CloseHandle(hThread);
		}

		// Try to launch a thread to handle our audio thread
		return hThread > 0 ? true : false;
	} else {
		return dwThreadId != 0;
	}
}

Sounds::Sounds() {
	pXAudio2 = NULL;
	hThread = NULL;
	bInitialized = false;
	bShutdown = false;
	dwThreadId = 0;
}

Sounds::~Sounds() {

}