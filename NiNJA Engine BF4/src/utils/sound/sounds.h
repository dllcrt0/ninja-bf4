#pragma once

enum eSoundTypes {
	SOUND_KILLSTREAK,
	SOUND_KILLSPREE
};

static PCHAR pSoundNames[] = {
	"streak",
	"spree"
};

struct SoundInfo {
	DWORD dwSoundType;
	DWORD dwSoundIndex;

	SoundInfo() {}

	SoundInfo(DWORD dwSoundType, DWORD dwSoundIndex) {
		this->dwSoundType = dwSoundType;
		this->dwSoundIndex = dwSoundIndex;
	}
};

class Sounds {
public:
	bool bInitialized;
	bool bShutdown;
	DWORD dwThreadId;
	HANDLE hThread;
	PBYTE pDataBuffer;

	IXAudio2* pXAudio2;
	IXAudio2MasteringVoice* pMasteringVoice;
	vector<SoundInfo> vSoundQueue;
	vector<string> vBFSoundQueue;

	bool Initialize(BOOL bFullInitialize = TRUE);
	void PlayXMA2Streamed(IXAudio2 * pXaudio2, const char * szFilename, DWORD dwPlayBeginOverride = 0);
	void PlayXMA2StreamedHandle(IXAudio2 * pXaudio2, HANDLE hFile, DWORD dwPlayBeginOverride = 0);
	void Shutdown(BOOL bFullShutdown = TRUE);
	void RunSync();
	void Queue(eSoundTypes soundType, DWORD dwSoundIndex);
	void QueueBF(string name);

	bool Start();
	Sounds();
	~Sounds();
};

extern Sounds pSounds;