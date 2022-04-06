#pragma once

#define RELEASE_VERSIONd
#define DEBUG

#define __GAMENAME__ "BF4"

#define DEVELOPER_OPTIONSd

//#define DUMP_BINS

#ifdef RELEASE_VERSION
#undef DEBUG
#define PATCH_SVR
#define ANTIHACK
#endif

#define GetTitleId() (XamGetCurrentTitleId() ^ 0x690690)
#define __TITLEID__ 0x45280F2A // 0x454109BA ^ 0x690690
#define __MSGRIDENT__ 0x72458CE3 // Unique XBLSMsgr ID, must be 4 bytes of data (should be completely random)
#define __TIMEDATESTAMP__ 0x546CDC23 // The timestamp for the latest default.xex/default_mp.xex for this game, grabbed from PLDR_DATA_TABLE_ENTRY->TimeDateStamp

#define ENCRYPTION_MARKER_BEGIN { \
	__asm { mr r4, r4 } \
	__asm { mr r5, r5 } \
}

#define ENCRYPTION_MARKER_END { \
	__asm { mr r5, r5 } \
	__asm { mr r4, r4 } \
}