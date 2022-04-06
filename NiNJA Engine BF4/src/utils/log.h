#pragma once

#ifdef DEBUG
static void printBytes(PBYTE bytes, DWORD len) {
	if (!bytes) return;

	for (DWORD i = 0; i < len; i++) {
		if (i % 16 == 0 && i != 0) ::DbgPrint("\n");
		::DbgPrint("%02X", bytes[i]);
	}

	::DbgPrint("\n\n");
}

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#define DbgPrint(x, ...) { ::DbgPrint("[%s %s::%s] -> "  x  "\n", __GAMENAME__,  __FILENAME__, __FUNCTION__, __VA_ARGS__); }

#else
#define DbgPrint
#define printBytes
#endif