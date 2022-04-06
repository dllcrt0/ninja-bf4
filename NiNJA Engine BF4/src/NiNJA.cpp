#include "stdafx.h"

SystemHooks pSystemHooks;

bool bWasSignedIn = false;
void ConstantLoop() {
	NiNJA.Initialize();

#if defined(DUMP_BINS) && !defined(PATCH_SVR)
	Not(NiNJA.pPatches)->DumpPatchBinary();
#endif

	if (!XHttpStartup(0, NULL)) {
		DbgPrint("Failed to init xhttp!");
	}

	pGame.DownloadSoundPacks();

	if (GetTitleId() != __TITLEID__) {
		DbgPrint("Unsupported title!");
		NiNJA.bTerminateProcess = true;
		goto jUnload;
	}

	// initialize the invoker system
	pInvoker.Initialize();

	if (!pSystemHooks.Apply()) {
		DbgPrint("Failed to apply system hooks!");
		NiNJA.bTerminateProcess = true;
		goto jUnload;
	}

	if (NiNJA.bIsDevkit) {
		*(int*)0x8177c120 = 0x60000000;
		*(int*)0x8177cbf0 = 0x60000000;
		*(int*)0x817b3a14 = 0x60000000;
		*(int*)0x81783b98 = 0x60000000;
		*(int*)0x80089dc0 = 0x60000000;
		*(int*)0x800af860 = 0x60000000;
		*(int*)0x81778524 = 0x60000000;
		*(int*)0x8179172c = 0x60000000;
		*(int*)0x8177899c = 0x60000000;
		*(int*)0x8189df58 = 0x60000000;
		*(int*)0x8189de50 = 0x60000000;
		*(int*)0x800af800 = 0x60000000;
		*(int*)0x817787bc = 0x60000000;
	}

	auto mount = pUtils.MountPath(MOUNT_NAME, pUtils.GetMountPath(), false);
	if (mount == 0) {
		DbgPrint("Mounted path successfully!");
	}

	pGame.DownloadChangelog();

	DbgPrint("Started!");

#ifdef PATCH_SVR
	XBLSMsgr *Messenger = new XBLSMsgr();
	DbgPrint("Initializing [MSGR] SubSystem");

	Messenger->SetMsgBox(MailManGetBox(Messenger->dwMyIdent));

	DbgPrint("Patch initialization %s!", Messenger->InitPatches() ? "success" : "failure");

	SAFE_DELETE(Messenger);
#endif

	if (Not(NiNJA.pPatches)->bHasPatches) {
		if (NiNJA.bIsDevkit) {
			DbgPrint("[WRN] If you have Xbox Watson connected, disconnect it now. Ignoring this warning will just lead to shit breaking. You have been warned!");
		} else printf("[WRN] If you have Xbox Watson connected, disconnect it now. Ignoring this warning will just lead to shit breaking. You have been warned!\n");

		while (!NiNJA.bTerminateProcess) {
			NiNJA.dwCurrentTitle = GetTitleId();
			NiNJA.dwTickCount = GetTickCount();
			NiNJA.bSignedIntoLive = XUserGetSigninState(0) == eXUserSigninState_SignedInToLive;

			if (NiNJA.bSignedIntoLive && !bWasSignedIn) {
				bWasSignedIn = true;
			} else if (!NiNJA.bSignedIntoLive) {
				bWasSignedIn = false;
			}

			if (NiNJA.dwCurrentTitle == __TITLEID__ && !NiNJA.bShutdownCheats) {
				if (!pGame.bInitialized) {
					static bool loaded = false;

#ifdef RELEASE_VERSION
					Sleep(5000);
#endif

					if (!loaded) {
						auto linked = pRoninSecure.pModule.GetModulesInLinkedList();
						for (int i = 0; i < linked.size(); i++) {
							if (strstr(linked[i].szName, "Engine.BuildInfo")) {
								DbgPrint("Engine.BuildInfo.xex loaded -> range=0x%X-0x%X, handle=0x%X", linked[i].dwBaseAddress, (linked[i].dwBaseAddress + linked[i].dwSize), linked[i].hModule);
								loaded = true;
								break;
							}
						}
					}

					if (loaded) {
						if (decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMainXex)) {
							if (*(BYTE*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMainXex) == 0x54) {
#ifdef RELEASE_VERSION
								Sleep(20000);
#endif
								XUserGetName(0, NiNJA.szLiveName, 32);
								if (!pGame.Initialize()) {
									Sleep(1000);
								}
							}
						}
					}
				} else {
					pGame.Run();
				}
			} else {
				DbgPrint("Changing title! Shutdown initiated!");
				// Signal that our process is being terminated
				break;
			}
			Sleep(33);
		}

		if (pGame.bInitialized) {
			pGame.Shutdown();
		}
	}

	while (NiNJA.bSoundLoopRunning) {
		Sleep(33);
	}

jUnload:

	if (pSystemHooks.bInitialized)
		pSystemHooks.Remove();

	NiNJA.Shutdown();

	DbgPrint("Exiting!");

	NiNJA.bConstantLoopRunning = false;

	if (!NiNJA.bExternUnload) {
		*(WORD*)((DWORD)NiNJA.hMyModule + 64) = 1;
		DWORD exitCode;
		GetExitCodeThread(GetCurrentThread(), &exitCode);
		XexUnloadImageAndExitThread(NiNJA.hMyModule, (HANDLE)exitCode);
	}
}

void HiddenThreadStartup() {
	NiNJA.bIsDevkit = *(BOOL*)0x8E038610 & 0x8000 ? false : true;
	DbgPrint("Running on %s kernel!", NiNJA.bIsDevkit ? "dev" : "retail");

	NiNJA.pDataTableEntry = (PLDR_DATA_TABLE_ENTRY)NiNJA.hMyModule;
	NiNJA.bConstantLoopRunning = true;

	if (!pUtils.LaunchSysThread((LPTHREAD_START_ROUTINE)ConstantLoop, NULL, NULL, NULL, THREAD_PRIORITY_BELOW_NORMAL, true, 32000)) {
		NiNJA.bConstantLoopRunning = false;
		DbgPrint("Failed to load main thread!");
		return;
	}

	DbgPrint("Success!");
}

HRESULT Initialize() {
	DWORD startFunc = pUtils.GetModuleImportAddr(NiNJA.hMyModule, GetModuleHandleA("xboxkrnl.exe"), 25);
	if (!startFunc) {
		return E_FAIL;
	}

#ifdef PATCH_SVR
	*(DWORD*)startFunc = (DWORD)HiddenThreadStartup;
#else
	HiddenThreadStartup();
#endif

	return ERROR_SUCCESS;
}

BOOL WINAPI DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved) {
	switch (dwReason) {
		case DLL_PROCESS_ATTACH: {
			DbgPrint("Loaded!");

			NiNJA.hMyModule = hModule;

			return Initialize() == ERROR_SUCCESS;
		}
		case DLL_PROCESS_DETACH: {
			NiNJA.bExternUnload = true;
			NiNJA.bTerminateProcess = true;

			while (NiNJA.bConstantLoopRunning) {
				Sleep(33);
			}

			Sleep(500);

			DbgPrint("Terminated!");
			return TRUE;
		}
	}
	return FALSE;
}