#pragma once

__forceinline Cheats *Not(Cheats *in) {
	return (Cheats*)~(DWORD)in;
}

#define MOUNT_NAME "NINJA:"
#define SYS_STRING "\\System??\\%s"
#define USR_STRING "\\??\\%s"
#define DEVICE_HDD "\\Device\\Harddisk0\\Partition1\\"
#define DEVICE_USB "\\Device\\Mass0"

#define ADDFLAG(v, f)  { \
	if((v & f) == 0) { \
		v |= f; \
	} \
} \

#define DELFLAG(v, f) { \
	if((v & f) == f) { \
		v &= ~f; \
	} \
} \

#define SAFE_FREE(p) { \
	if(p) { \
		free(p); \
	} \
} \

#define XSAFE_FREE(p) { \
	if(p) { \
		XPhysicalFree(p); \
	} \
} \

#define SAFE_DELETE(p) { \
	if(p) { \
		delete p; \
	} \
} \

#define SAFE_DELETE_ARRAY(p) { \
	if(p) { \
		delete[] p; \
	} \
} \

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) { if ( (x) != NULL && (x)->Release() == 0 ) { (x) = NULL; } }
#endif

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

typedef struct _XEX_IMPORT_TABLE_2 {
	DWORD                 TableSize;
	HV_IMAGE_IMPORT_TABLE ImportTable;
} XEX_IMPORT_TABLE_2, *PXEX_IMPORT_TABLE_2;

typedef struct _LDR_DATA_TABLE_ENTRY_2 {
	LIST_ENTRY     InLoadOrderLinks;
	LIST_ENTRY     InClosureOrderLinks;
	LIST_ENTRY     InInitializationOrderLinks;
	VOID*          NtHeadersBase;
	VOID*          ImageBase;
	DWORD          SizeOfNtImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	DWORD          Flags;
	DWORD          SizeOfFullImage;
	VOID*          EntryPoint;
	WORD           LoadCount;
	WORD           ModuleIndex;
	VOID*          DllBaseOriginal;
	DWORD          CheckSum;
	DWORD          ModuleLoadFlags;
	DWORD          TimeDateStamp;
	VOID*          LoadedImports;
	VOID*          XexHeaderBase;
	union {
		ANSI_STRING               LoadFileName;
		struct {
			_LDR_DATA_TABLE_ENTRY* ClosureRoot;
			_LDR_DATA_TABLE_ENTRY* TraversalParent;
		} asEntry;
	};
} LDR_DATA_TABLE_ENTRY_2, *PLDR_DATA_TABLE_ENTRY_2;

class Invoke {
public:
	template<typename T>
	static T Call(DWORD dwAddress) { return ((T(*)())dwAddress)(); }

	template<typename T, typename P1>
	static T Call(DWORD dwAddress, P1 p1) { return ((T(*)(P1))dwAddress)(p1); }

	template<typename T, typename P1, typename P2>
	static T Call(DWORD dwAddress, P1 p1, P2 p2) { return ((T(*)(P1, P2))dwAddress)(p1, p2); }

	template<typename T, typename P1, typename P2, typename P3>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3) { return ((T(*)(P1, P2, P3))dwAddress)(p1, p2, p3); }

	template<typename T, typename P1, typename P2, typename P3, typename P4>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4) { return ((T(*)(P1, P2, P3, P4))dwAddress)(p1, p2, p3, p4); }

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) { return ((T(*)(P1, P2, P3, P4, P5))dwAddress)(p1, p2, p3, p4, p5); }

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) { return ((T(*)(P1, P2, P3, P4, P5, P6))dwAddress)(p1, p2, p3, p4, p5, p6); }

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) { return ((T(*)(P1, P2, P3, P4, P5, P6, P7))dwAddress)(p1, p2, p3, p4, p5, p6, p7); }

	template<typename T, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
	static T Call(DWORD dwAddress, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) { return ((T(*)(P1, P2, P3, P4, P5, P6, P7, P8))dwAddress)(p1, p2, p3, p4, p5, p6, p7, p8); }
};

class Utils {
public:
	void Split(const string& s, char c, vector<string>& v);
	bool FileExists(const char* file);
	bool DirectoryExists(const char* file);
	const char* va(const char* pFormat, ...);
	const char* vaAlloc(const char* pFormat, ...);
	const char* vaBuff(char* buf, int size, const char* pFormat, ...);
	void BinToFile(const char* fname, void* dPtr, ULONG len);
	HRESULT DoMountPath(const char* szDrive, const char* szDevice, const char* sysStr);
	HRESULT MountPath(const char* szDrive, const char* szDevice, bool both);
	const char* GetMountPath();
	DWORD GetModuleImportAddr(HANDLE HModule, HANDLE HImportedModule, DWORD Ordinal);
	bool LaunchSysThread(LPTHREAD_START_ROUTINE start, PVOID param, PHANDLE hThread, PDWORD dwThreadId, DWORD dwThreadPriority, bool bSpecial, DWORD dwStackSize);
	DWORD ResolveFunction(HMODULE hModule, DWORD Ordinal);
	int GetRandomValueInRange(int min, int max);
	float GetDistance(Vec3 start, Vec3 end);
	DWORD timeGetTime();
	DWORD Joaat(const char* value);
	void AngleVectors(Vec3* angles, Vec3* forward, Vec3* right, Vec3* up);
	void VectorSubtract(Vec3 a, Vec3 b, Vec3& c);
	float DotProduct(Vec3 hax, Vec3 Vector);
	void Ease(float& toEase, float& easeFrom, float multiplier);
	void GetFilesFromDirectory(vector<string>& buffer, string folder, string extention);
};

extern Utils pUtils;