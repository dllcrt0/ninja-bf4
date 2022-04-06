#pragma once

void Terminate();

class Globals {
public:
	DWORD dwCurrentTitle; // Cached copy of the current title ID
	DWORD dwTickCount;
	HANDLE hMyModule;

	PLDR_DATA_TABLE_ENTRY pDataTableEntry;

	bool bSoundLoopRunning;
	bool bConstantLoopRunning;
	bool bExternUnload;
	bool bGameInitialized;
	bool bIsDevkit;
	bool bShutdownCheats;
	bool bTerminateProcess;
	bool bSignedIntoLive;
	bool bHasReadConfig;

	char szLiveName[0x20];

	Cheats* pPatches;

	void Initialize();
	void Shutdown();
};

extern Globals NiNJA;