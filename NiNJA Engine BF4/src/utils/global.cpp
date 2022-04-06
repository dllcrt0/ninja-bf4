#include "stdafx.h"

Globals NiNJA;

void Globals::Shutdown() {
	XSAFE_FREE(Not(pPatches));
}

void Globals::Initialize() {
	dwCurrentTitle = 0;
	bSoundLoopRunning = false;
	bExternUnload = false;
	bGameInitialized = false;
	bShutdownCheats = false;
	bTerminateProcess = false;
	bHasReadConfig = false;

	pPatches = Not((Cheats*)XPhysicalAlloc(sizeof(Cheats), MAXULONG_PTR, NULL, PAGE_READWRITE));
	Not(pPatches)->Initialize();

	DbgPrint("Initialized!");
}

bool bTerminating = false;
void Terminate() {
	if (NiNJA.bGameInitialized && !bTerminating) {
		bTerminating = true;
		DbgPrint("Title is terminating!");
		DbgPrint("No more web requests are busy!");

		NiNJA.bShutdownCheats = TRUE;
		while (NiNJA.bShutdownCheats) {
			Sleep(33);
		}

		DbgPrint("Cheats are fully shutdown!");

		while (NiNJA.bSoundLoopRunning) {
			Sleep(33);
		}

		DbgPrint("Our extra thread has exited!");
	}
}