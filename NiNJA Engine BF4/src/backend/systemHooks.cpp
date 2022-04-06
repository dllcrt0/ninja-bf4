#include "stdafx.h"

typedef DWORD(*tXbdmCommand)(const char*, const char*, int, DWORD*);

SystemHooks::SystemHooks() {
	bInitialized = false;
}

Detour<void>* OriginalExTerminateTitleProcess = new Detour<void>();
void ExTerminateTitleProcessHook(int r3) {
	DbgPrint("ExTerminateTitleProcess!");
	
	Terminate();

	OriginalExTerminateTitleProcess->CallOriginal(0);
}

Detour<void> XamLoaderLaunchTitleDetour;
void XamLoaderLaunchTitleHook(PCHAR r3, int r4) {
	DbgPrint("XamLoaderLaunchTitle!");

	Terminate();

	XamLoaderLaunchTitle(r3, r4);
}

#pragma region xbdm
tXbdmCommand OriginalXbdmBreak;
DWORD XbdmBreakHook(const char* commandString, const char* outBuffer, int len, DWORD* unk) {
	// close the game, this would only be called if they were debugging.
	return 0x82DA000E;
}

tXbdmCommand OriginalXbdmContinue;
DWORD XbdmContinueHook(const char* commandString, const char* outBuffer, int len, DWORD* unk) {
	// close the game, this would only be called if they were debugging.
	return 0x82DA000E;
}

tXbdmCommand OriginalXbdmDebugger;
DWORD XbdmDebuggerHook(const char* commandString, const char* outBuffer, int len, DWORD* unk) {
	if (strstr(commandString, "Visual Studio")) {
		// close the game, this would only be called if they were debugging.
	} else {
		// watson or another type of debugger we haven't logged.
		return OriginalXbdmDebugger(commandString, outBuffer, len, unk);
	}

	return 0x82DA000E;
}

tXbdmCommand OriginalXbdmGetmemex;
DWORD XbdmGetmemexHook(const char* commandString, const char* outBuffer, int len, DWORD* unk) {
	string str = string(commandString);
	size_t commandOffset = str.find(' ');
	string command = str.substr(0, commandOffset);
	string next = str.substr(commandOffset + 1);

	size_t addrOffset = next.find(' ');
	string addr = next.substr(0, addrOffset);

	string addrValue = addr.substr(addr.find('=') + 1);

	transform(command.begin(), command.end(), command.begin(), ::tolower);

	DWORD address = stoul(addrValue);

	if (address >= (DWORD)NiNJA.pDataTableEntry->ImageBase && address <= ((DWORD)NiNJA.pDataTableEntry->ImageBase + NiNJA.pDataTableEntry->SizeOfFullImage)) {
		// trying to read inside my module.
		return 0x82DA000E;
	}

	return OriginalXbdmGetmemex(commandString, outBuffer, len, unk);
}

tXbdmCommand OriginalXbdmGetmem;
DWORD XbdmGetmemHook(const char* commandString, const char* outBuffer, int len, DWORD* unk) {
	DWORD original = 0;

	string str = string(commandString);
	size_t commandOffset = str.find(' ');
	string command = str.substr(0, commandOffset);
	string next = str.substr(commandOffset + 1);

	size_t addrOffset = next.find(' ');
	string addr = next.substr(0, addrOffset);

	string addrValue = addr.substr(addr.find('=') + 1);

	transform(command.begin(), command.end(), command.begin(), ::tolower);

	DWORD address = stoul(addrValue);

	if (address >= (DWORD)NiNJA.pDataTableEntry->ImageBase && address <= ((DWORD)NiNJA.pDataTableEntry->ImageBase + NiNJA.pDataTableEntry->SizeOfFullImage)) {
		// trying to read inside my module.
		return 0x82DA000E;
	}

	return OriginalXbdmGetmem(commandString, outBuffer, len, unk);
}

tXbdmCommand OriginalXbdmSetmem;
DWORD XbdmSetmemHook(const char* commandString, const char* outBuffer, int len, DWORD* unk) {
	DWORD original = 0;

	string str = string(commandString);
	size_t commandOffset = str.find(' ');
	string command = str.substr(0, commandOffset);
	string next = str.substr(commandOffset + 1);

	size_t addrOffset = next.find(' ');
	string addr = next.substr(0, addrOffset);

	string addrValue = addr.substr(addr.find('=') + 1);

	transform(command.begin(), command.end(), command.begin(), ::tolower);

	char *_ = 0;
	DWORD address = strtoul(addrValue.c_str(), &_, 0);

	if (address >= (DWORD)NiNJA.pDataTableEntry->ImageBase && address <= ((DWORD)NiNJA.pDataTableEntry->ImageBase + NiNJA.pDataTableEntry->SizeOfFullImage)) {
		// trying to write inside my module.
		return 0x82DA000E;
	}

	return OriginalXbdmSetmem(commandString, outBuffer, len, unk);
}
#pragma endregion

bool SystemHooks::Apply() {
	if (!bInitialized) {
		OriginalExTerminateTitleProcess->SetupDetour("xboxkrnl.exe", 0x1A, &ExTerminateTitleProcessHook);

		XamLoaderLaunchTitleDetour.SetupDetour(0x82000000, "xam.xex", 0x01A4, &XamLoaderLaunchTitleHook);

/*#ifndef DEBUG
		if (!NiNJA.bIsDevkit) {
			pRoninSecure.pXboxDebugManager.InitializeCommands();
			pRoninSecure.pXboxDebugManager.HookCommand("break", &XbdmBreakHook, &OriginalXbdmBreak);
			pRoninSecure.pXboxDebugManager.HookCommand("continue", &XbdmContinueHook, &OriginalXbdmContinue);
			pRoninSecure.pXboxDebugManager.HookCommand("debugger", &XbdmDebuggerHook, &OriginalXbdmDebugger);
			pRoninSecure.pXboxDebugManager.HookCommand("getmemex", &XbdmGetmemexHook, &OriginalXbdmGetmemex);
			pRoninSecure.pXboxDebugManager.HookCommand("getmem", &XbdmGetmemHook, &OriginalXbdmGetmem);
			pRoninSecure.pXboxDebugManager.HookCommand("setmem", &XbdmSetmemHook, &OriginalXbdmSetmem);
		}
#endif*/

		DbgPrint("Applied detours!");
		bInitialized = true;
		return true;
	} else {
		DbgPrint("System hooks already initialized!");
	}

	return false;
}

void SystemHooks::Remove() {
	if (bInitialized) {
		OriginalExTerminateTitleProcess->TakeDownDetour();

		bInitialized = false;
	} else {
		DbgPrint("TitleHooks have not been initialized!");
	}
}