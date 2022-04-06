#pragma once

#define docryptoBypass(a,b)  ((QWORD)((QWORD)( ((QWORD)0x120b7a1372b75012) * ((QWORD)(b)) ) ^ (~(QWORD)(a))))

#if defined(PATCH_SVR)
#define decryptDWORD(a) ((DWORD)(docryptoBypass(a, (QWORD)((((DWORD*)&a - (DWORD*)&Not(NiNJA.pPatches)->BF4))))))
#else
#define	decryptDWORD(a) (a)
#endif

struct CheatPatches {
	DWORD dwRenderPatches[3];
	DWORD dwMessageBox[7];
	DWORD dwAntiCheatPatches[3];
	DWORD dwUnlimitedAmmo[4];
	DWORD dwTeleportPatches[14];
	DWORD dwStwCrashPatches[16];
	DWORD dwTeleportPatchValues[18];
	
	DWORD dwNop;
	DWORD dwBLR;
	DWORD dwMainXex;
	DWORD dwClientGameContext;
	DWORD dwGameRenderer;
	DWORD dwButtonSetup;
	DWORD dwUIEngine;
	DWORD dwSyncedBFSettings;
	DWORD dwBranchHookJumper;
	DWORD dwFormatString;
	DWORD dwDrawRect;
	DWORD dwRectPtr;
	DWORD dwRenderingCreateText;
	DWORD dwExecuteMessage;
	DWORD dwPrimaryFireCallbackExecuteMessage;
	DWORD dwGetDispersion;
	DWORD dwDrawHudInventory;
	DWORD dwGetRecoil;
	DWORD dwAddDamageData;
	DWORD dwSendClientDamageMessage;
	DWORD dwPhysicsUpdater;
	DWORD dwHandleTextGlow;
	DWORD dwXamInputGetState;
	DWORD dwGetPlayerScore;
	DWORD dwPhysicsRunner;
	DWORD dwDrawRectHk;
	DWORD dwSendTextMessage;
	DWORD dwServerPeerManager;
	DWORD dwExecuteNetworkableMessage;
	DWORD dwClientDamageInfoVtable;
	DWORD dwClientPlayerVtable;
	DWORD dwClientSoldierEntityVtable;
	DWORD dwDrawLine;
	DWORD dwSetText;
	DWORD dwDisplay;
	DWORD dwSetTextAlign;
	DWORD dwConstructTextGlow;
	DWORD dwConstructFontDefinition;
	DWORD dwFontStyleVtable;
	DWORD dwReleaseText;
	DWORD dwPhysicsRay;
	DWORD dwTimeGetTime;
	DWORD dwSendOrderPatch;
	DWORD dwSendOrder;
	DWORD dwViewBobbing;
	DWORD dwMessageConstruct;
	DWORD dwCrashHookFix;
	DWORD dwCreateKillMessage;
	DWORD dwCreateKillMessageGetInfo;
	DWORD dwClientConnectionSendMessage;
	DWORD dwUpdatePosition;
	DWORD dwOnShoot;
	DWORD dwStwPatch;
	DWORD dwGlobalNodeSupply;
	DWORD dwGlobalNodeExplosives;
	DWORD dwGlobalNodeGrenade;
	DWORD dwGlobalNodePickups;
	DWORD dwGlobalNodeVehicles;
	DWORD dwAimbotHook;
	DWORD dwTextAntiSpam1;
	DWORD dwTextAntiSpam2;
	DWORD dwClientGameWorld;
	DWORD dwServerInfoData;
	DWORD dwUnknownCrash4Hook;
};

class Cheats {
public:
	bool bHasPatches;
	CheatPatches BF4;

	HRESULT SetAddresses(PVOID data) {
#ifndef PATCH_SVR

		BF4.dwUnknownCrash4Hook = 0x88D10620;
		BF4.dwServerInfoData = 0x898536F4;
		BF4.dwClientGameWorld = 0x89916E74;
		BF4.dwTextAntiSpam1 = 0x8841D530;
		BF4.dwTextAntiSpam2 = 0x8841D534;
		BF4.dwAimbotHook = 0x88A832A0;
		BF4.dwGlobalNodeVehicles = 0x89A10B10;
		BF4.dwGlobalNodePickups = 0x89A38D38;
		BF4.dwGlobalNodeGrenade = 0x89A39DE8;
		BF4.dwGlobalNodeExplosives = 0x89A39D48;
		BF4.dwGlobalNodeSupply = 0x89A39F28;
		BF4.dwStwPatch = 0x888EC458;
		BF4.dwOnShoot = 0x8856BF28;
		BF4.dwUpdatePosition = 0x88D0D6A8;
		BF4.dwClientConnectionSendMessage = 0x88B277B0;
		BF4.dwCreateKillMessageGetInfo = 0x88D0F800;
		BF4.dwCreateKillMessage = 0x88767A48;
		BF4.dwCrashHookFix = 0x8845E890;
		BF4.dwMessageConstruct = 0x888EC9E8;
		BF4.dwViewBobbing = 0x88C0D9E0;
		BF4.dwSendOrder = 0x8856D380;
		BF4.dwSendOrderPatch = 0x88655120;
		BF4.dwTimeGetTime = 0x8921A648;
		BF4.dwPhysicsRay = 0x88BD35D8;
		BF4.dwReleaseText = 0x88FC31B8;
		BF4.dwFontStyleVtable = 0x880FFC00;
		BF4.dwConstructFontDefinition = 0x897591F8;
		BF4.dwConstructTextGlow = 0x89756B60;
		BF4.dwSetTextAlign = 0x88A9F9B8;
		BF4.dwSetText = 0x88AA5E40;
		BF4.dwDisplay = 0x88AA3798;
		BF4.dwDrawLine = 0x88AA40A8;
		BF4.dwClientPlayerVtable = 0x880C6FC0;
		BF4.dwClientSoldierEntityVtable = 0x88158720;
		BF4.dwClientDamageInfoVtable = 0x880330E8;
		BF4.dwExecuteNetworkableMessage = 0x88B277B0;
		BF4.dwServerPeerManager = 0x89975554;
		BF4.dwSendTextMessage = 0x8856A310;
		BF4.dwDrawRectHk = 0x88763360;
		BF4.dwPhysicsRunner = 0x88D55340;
		BF4.dwGetPlayerScore = 0x88583598;
		BF4.dwHandleTextGlow = 0x88AA8098;
		BF4.dwMainXex = 0x8814CF78;
		BF4.dwClientGameContext = 0x89916E78;
		BF4.dwUIEngine = 0x8998ACCC;
		BF4.dwSyncedBFSettings = 0x89846BB0;
		BF4.dwXamInputGetState = 0x896402CC;
		BF4.dwBranchHookJumper = 0x89735B68;
		BF4.dwFormatString = 0x88E60798;
		BF4.dwDrawRect = 0x88AA2C80;
		BF4.dwRectPtr = 0x8991B90C;
		BF4.dwRenderingCreateText = 0x8852B0A0;
		BF4.dwPrimaryFireCallbackExecuteMessage = 0x8915B7B8;
		BF4.dwExecuteMessage = 0x888F0150;
		BF4.dwGetDispersion = 0x884813D0;
		BF4.dwDrawHudInventory = 0x88712A90;
		BF4.dwGetRecoil = 0x88481448;
		BF4.dwAddDamageData = 0x88BEF318;
		BF4.dwSendClientDamageMessage = 0x885A8A70;
		BF4.dwPhysicsUpdater = 0x88D55340;
		BF4.dwGameRenderer = 0x8991B790;
		BF4.dwButtonSetup = 0x8991BB40;
		BF4.dwNop = 0x60000000;
		BF4.dwBLR = 0x4e800020;

		BF4.dwRenderPatches[0] = 0x88762008;
		BF4.dwRenderPatches[1] = 0x88759DBC;
		BF4.dwRenderPatches[2] = 0x887620C0;

		BF4.dwMessageBox[0] = 0x886ADEC7;
		BF4.dwMessageBox[1] = 0x886ADEDC;
		BF4.dwMessageBox[2] = 0x886ADDD8;
		BF4.dwMessageBox[3] = 0x899B6F50;
		BF4.dwMessageBox[4] = 0x886ADE08;
		BF4.dwMessageBox[5] = 0x886ADE10;
		BF4.dwMessageBox[6] = 0x886ADE0F;

		BF4.dwAntiCheatPatches[0] = 0x888C8EC4;
		BF4.dwAntiCheatPatches[1] = 0x88460AA8;
		BF4.dwAntiCheatPatches[2] = 0x881C62F4;

		BF4.dwUnlimitedAmmo[0] = 0x88A94758;
		BF4.dwUnlimitedAmmo[1] = 0x8991B78C;
		BF4.dwUnlimitedAmmo[2] = 0x88534498;
		BF4.dwUnlimitedAmmo[3] = 0x88548C90;

		BF4.dwTeleportPatches[0] = 0x89158D00;
		BF4.dwTeleportPatches[1] = 0x885F4EEC;
		BF4.dwTeleportPatches[2] = 0x88623830;
		BF4.dwTeleportPatches[3] = 0x88623838;
		BF4.dwTeleportPatches[4] = 0x88623748;
		BF4.dwTeleportPatches[5] = 0x88623750;
		BF4.dwTeleportPatches[6] = 0x885F410C;
		BF4.dwTeleportPatches[7] = 0x885F4DB8;
		BF4.dwTeleportPatches[8] = 0x885F4E1C;
		BF4.dwTeleportPatches[9] = 0x885F4E04;
		BF4.dwTeleportPatches[10] = 0x885F4E10;
		BF4.dwTeleportPatches[11] = 0x885F51E4;
		BF4.dwTeleportPatches[12] = 0x885F4E54;
		BF4.dwTeleportPatches[13] = 0x89158CD0;

		BF4.dwTeleportPatchValues[0] = 0x4800004C;
		BF4.dwTeleportPatchValues[1] = 0x480002FC;
		BF4.dwTeleportPatchValues[2] = 0x48000034;
		BF4.dwTeleportPatchValues[3] = 0x4800007C;

		BF4.dwTeleportPatchValues[4] = 0x4099004C;
		BF4.dwTeleportPatchValues[5] = 0x1000038C;
		BF4.dwTeleportPatchValues[6] = 0x11BFD9C3;
		BF4.dwTeleportPatchValues[7] = 0x915F0040;
		BF4.dwTeleportPatchValues[8] = 0x119FD9C3;
		BF4.dwTeleportPatchValues[9] = 0x913F0040;
		BF4.dwTeleportPatchValues[10] = 0xD3350020;
		BF4.dwTeleportPatchValues[11] = 0x419A0430;
		BF4.dwTeleportPatchValues[12] = 0x409900D0;
		BF4.dwTeleportPatchValues[13] = 0x419A00E8;
		BF4.dwTeleportPatchValues[14] = 0x419A00DC;
		BF4.dwTeleportPatchValues[15] = 0x939F003C;
		BF4.dwTeleportPatchValues[16] = 0x409A0034;
		BF4.dwTeleportPatchValues[17] = 0x419A007C;

		BF4.dwStwCrashPatches[0] = 0x888EAFF8;
		BF4.dwStwCrashPatches[1] = 0x888EB078;
		BF4.dwStwCrashPatches[2] = 0x888EB088;
		BF4.dwStwCrashPatches[3] = 0x888EB0E4;
		BF4.dwStwCrashPatches[4] = 0x888EB104;
		BF4.dwStwCrashPatches[5] = 0x888EB1A8;
		BF4.dwStwCrashPatches[6] = 0x888EBB14;
		BF4.dwStwCrashPatches[7] = 0x888EBB2C;
		BF4.dwStwCrashPatches[8] = 0x888EBB70;
		BF4.dwStwCrashPatches[9] = 0x888EBE78;
		BF4.dwStwCrashPatches[10] = 0x888EBE88;
		BF4.dwStwCrashPatches[11] = 0x888EBE98;
		BF4.dwStwCrashPatches[12] = 0x888EBEDC;
		BF4.dwStwCrashPatches[13] = 0x888EC410;
		BF4.dwStwCrashPatches[14] = 0x888EC458;
		BF4.dwStwCrashPatches[15] = 0x888EC924;

		bHasPatches = true;
#else
		if (data) {
			memcpy(&BF4, data, sizeof(CheatPatches));
			bHasPatches = true;
			return ERROR_SUCCESS;
		} else {
			bHasPatches = FALSE;
			return E_FAIL;
		}
#endif
		return ERROR_SUCCESS;
	}

	void DissolveAddresses() {
		if (bHasPatches) {
			XeCryptRandom((PBYTE)&BF4, sizeof(CheatPatches));
			bHasPatches = false;
		}
	}

	void Initialize() {
		bHasPatches = false;
#ifndef PATCH_SVR
		ZeroMemory(&BF4, sizeof(CheatPatches));
		SetAddresses(NULL);
		bHasPatches = true;
#endif
	}

#ifdef DUMP_BINS
	void DumpPatchBinary(char* pFile = NULL);
#endif
};