#pragma once

#include <xtl.h>
#include <xboxmath.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <time.h>
#include <vector>
#include <array>
#include <ppcintrinsics.h>
#include <math.h>
#include <fstream>
#include <cstdio>
#include <tchar.h>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <xhttp.h>
#include <string>
#include <xaudio2.h>
#include <xact3wb.h>

#include "xkelib.h"

#define M_PI 3.14159265358979323846264338327950288

#pragma warning(disable : 4244)
#pragma warning(disable : 4172)
#pragma warning(disable : 4800)
#pragma warning(disable : 4018)

using namespace std;
#define NUMOF(x) (sizeof(x)/sizeof(x[0]))

template<typename T>
__declspec(noinline) static bool ValidPtr(T* ptr) {
	DWORD address = (DWORD)ptr;
	if (ptr && (address > 0x30000000 && address < 0x80000000) && HIWORD(ptr) && MmIsAddressValid((void*)ptr)) {
		return true;
	}

	return false;
}

/*Utils*/
#include "utils/enums.h"
#include "utils/settings.h"
#include "utils/patches.h"
#include "utils/structs.h"
#include "utils/global.h"
#include "utils/utilities.h"
#include "utils/log.h"
#include "utils/heapManager.h"
#include "utils/sound/AtgAudioCleaned.h"
#include "utils/sound/sounds.h"
#include "utils/config.h"
#include "utils/memory/memory.h"
#include "utils/rtti.h"

/*Network*/
#include "network/HttpRequest.h"

/*Security*/
#include "roninSecure/security/module.h"
#include "roninSecure/security/functionRemoval.h"
#include "roninSecure/security/runtime.h"
#include "roninSecure/security/xbdm.h"
#include "roninSecure/roninSecure.h"
#include "roninSecure/XBLSMsgr.h"
#include "roninSecure/mailMan.h"

/*Backend*/
#include "backend/hooking.h"
#include "backend/systemHooks.h"
#include "backend/titleHooks.h"

/*Invoker*/
#include "roninSecure/invoker/invoker.h"
#include "roninSecure/invoker/natives.h"

/*Menu*/
#include "menu/game.h"
#include "menu/messages.h"
#include "menu/aimbot.h"

/*Input*/
#include "utils/input/controller.h"

#include "menu/weapon.h"
#include "menu/player.h"
#include "menu/renderer.h"

/*Options*/
#include "menu/handler/options/option.h"
#include "menu/handler/options/buttonOption.h"
#include "menu/handler/options/toggleOption.h"
#include "menu/handler/options/scrollOption.h"
#include "menu/handler/options/numberOption.h"
#include "menu/handler/options/submenuOption.h"
#include "menu/handler/options/colorOption.h"
#include "menu/handler/options/breakOption.h"

/*Submenu*/
#include "menu/handler/submenu.h"
#include "menu/handler/submenuHandler.h"

/*Menu*/
#include "menu/handler/menu.h"
#include "menu/handler/utils/menuFunctions.h"