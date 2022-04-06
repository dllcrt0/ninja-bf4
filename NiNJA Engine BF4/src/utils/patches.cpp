#include "stdafx.h"

BYTE dumpBuf[sizeof(CheatPatches)];
#define dumpBF4Patch(a) ((DWORD)(docryptoBypass(a, (QWORD)((((DWORD*)&a - (DWORD*)&BF4))))))
#define DumpName "NINJA:\\ninjacache\\" __GAMENAME__ ".bin"

#ifdef DUMP_BINS
void Cheats::DumpPatchBinary(char* file) {
	if (!bHasPatches)
		return;

	DbgPrint("Dumping!");

	memset(dumpBuf, 0, sizeof(CheatPatches));

	DWORD *fileBuff = (DWORD*)&dumpBuf[0];
	for (int i = 0; i * 4 < sizeof(CheatPatches); i += 1) {
		fileBuff[i] = (DWORD)dumpBF4Patch(*(DWORD*)((DWORD)&BF4 + ((DWORD)i * 4)));
	}

	DbgPrint("Writing!");

	pUtils.BinToFile(file != NULL ? file : DumpName, dumpBuf, sizeof(CheatPatches));
}
#endif