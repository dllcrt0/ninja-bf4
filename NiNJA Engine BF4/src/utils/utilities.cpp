#include "stdafx.h"

Utils pUtils;

#define PI (3.14159265358979323846264338327950288f)
char g_szVaBuffer[0x1000];

const char* Utils::va(const char* pFormat, ...) {
	memset(g_szVaBuffer, 0, 0x1000);
	va_list ap;
	va_start(ap, pFormat);
	RtlVsprintf(g_szVaBuffer, pFormat, ap);
	va_end(ap);

	return g_szVaBuffer;
}

const char* Utils::vaAlloc(const char* pFormat, ...) {
	char* allocated = new char[0x250];
	va_list ap;
	va_start(ap, pFormat);
	RtlVsprintf(allocated, pFormat, ap);
	va_end(ap);
	return allocated;
}

const char* Utils::vaBuff(char* buf, int size, const char* pFormat, ...) {
	memset(buf, 0, size);
	va_list ap;
	va_start(ap, pFormat);
	RtlVsprintf(buf, pFormat, ap);
	va_end(ap);

	return buf;
}

bool Utils::FileExists(const char* file) {
	if (GetFileAttributes(file) == -1) {
		DWORD lastError = GetLastError();
		if (lastError == ERROR_FILE_NOT_FOUND || lastError == ERROR_PATH_NOT_FOUND)
			return false;
	}
	return true;
}

bool Utils::DirectoryExists(const char* file) {
	DWORD ftyp = GetFileAttributesA(file);
	if (ftyp == -1)
		return false;

	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
		return true;

	return false;
}

void Utils::Split(const string& s, char c, vector<string>& v) {
	string::size_type i = 0;
	string::size_type j = s.find(c);

	while (j != string::npos) {
		v.push_back(s.substr(i, j - i));
		i = ++j;
		j = s.find(c, j);

		if (j == string::npos)
			v.push_back(s.substr(i, s.length()));
	}
}

void Utils::BinToFile(const char* fname, void* dPtr, ULONG len) {
	if (dPtr) {
		FILE* fp = fopen(fname, "wb");
		if (fp) {
			DbgPrint("Writing file \"%s\"", fname);
			fwrite(dPtr, len, 1, fp);
			fflush(fp);
			fclose(fp);
		}
	}
}

HRESULT Utils::DoMountPath(const char* szDrive, const char* szDevice, const char* sysStr) {
	STRING DeviceName, LinkName;
	CHAR szDestinationDrive[MAX_PATH];
	sprintf_s(szDestinationDrive, MAX_PATH, sysStr, szDrive);
	RtlInitAnsiString(&DeviceName, szDevice);
	RtlInitAnsiString(&LinkName, szDestinationDrive);
	ObDeleteSymbolicLink(&LinkName);
	return (HRESULT)ObCreateSymbolicLink(&LinkName, &DeviceName);
}

HRESULT Utils::MountPath(const char* szDrive, const char* szDevice, bool both) {
	HRESULT res;
	if (both) {
		res = DoMountPath(szDrive, szDevice, SYS_STRING);
		res = DoMountPath(szDrive, szDevice, USR_STRING);
	} else {
		if (KeGetCurrentProcessType() == 2) //SYSTEM_PROC
			res = DoMountPath(szDrive, szDevice, SYS_STRING);
		else
			res = DoMountPath(szDrive, szDevice, USR_STRING);
	}
	return res;
}

const char* Utils::GetMountPath() {
	char* DEVICE_DYNAMIC = new char[MAX_PATH];
	wstring ws;
	PLDR_DATA_TABLE_ENTRY TableEntry;
	XexPcToFileHeader((PVOID)0x90e00000, &TableEntry);

	ws = TableEntry->FullDllName.Buffer;
	string FullDllName(ws.begin(), ws.end());

	ws = TableEntry->BaseDllName.Buffer;
	string BaseDllName(ws.begin(), ws.end());

	string::size_type i = FullDllName.find(BaseDllName);

	if (i != string::npos)
		FullDllName.erase(i, BaseDllName.length());

	memset(DEVICE_DYNAMIC, 0x0, MAX_PATH);
	strcpy(DEVICE_DYNAMIC, FullDllName.c_str());

	return DEVICE_DYNAMIC;
}

DWORD Utils::GetModuleImportAddr(HANDLE HModule, HANDLE HImportedModule, DWORD Ordinal) {
	PLDR_DATA_TABLE_ENTRY_2 Module = (PLDR_DATA_TABLE_ENTRY_2)HModule;

	DWORD address = (DWORD)GetProcAddress((HMODULE)HImportedModule, (LPCSTR)Ordinal);

	if (address == NULL || HModule == NULL)
		return 0;

	void* headerBase = Module->XexHeaderBase;
	PXEX_IMPORT_DESCRIPTOR importDesc = (PXEX_IMPORT_DESCRIPTOR)RtlImageXexHeaderField(headerBase, 0x000103FF);
	if (importDesc == NULL)
		return 0;

	char* stringTable = (char*)(importDesc + 1);

	PXEX_IMPORT_TABLE_2 importTable = (PXEX_IMPORT_TABLE_2)(stringTable + importDesc->NameTableSize);

	for (DWORD x = 0; x < importDesc->ModuleCount; x++) {
		DWORD* importAdd = (DWORD*)(importTable + 1);
		for (DWORD y = 0; y < importTable->ImportTable.ImportCount; y++) {
			DWORD value = *((DWORD*)importAdd[y]);
			if (value == address) {
				return (DWORD)(&importAdd[y + 1]);
			}
		}

		importTable = (PXEX_IMPORT_TABLE_2)(((BYTE*)importTable) + importTable->TableSize);
	}

	return 0;
}

bool Utils::LaunchSysThread(LPTHREAD_START_ROUTINE start, PVOID param, PHANDLE hThread, PDWORD dwThreadId, DWORD dwThreadPriority, bool bSpecial, DWORD dwStackSize) {
	DWORD flags = EX_CREATE_FLAG_SUSPENDED | EX_CREATE_FLAG_SYSTEM;
	
	if (bSpecial) {
#ifdef ANTIHACK
		flags = 0x00000001 | 0x00000002 | 0x04000000 | 0x00000020 | 0x18000424;
#endif
	}

	HANDLE lThread;
	DWORD lThreadId;

	if (ExCreateThread(&lThread, dwStackSize, &lThreadId, (VOID*)XapiThreadStartup, start, param, flags) != ERROR_SUCCESS)
		return false;

	if (!lThread)
		return false;

	if (hThread)
		*hThread = lThread;

	if (dwThreadId)
		*dwThreadId = lThreadId;

	XSetThreadProcessor(lThread, 4);
	SetThreadPriority(lThread, dwThreadPriority);
	ResumeThread(lThread);
	return true;
}

DWORD Utils::ResolveFunction(HMODULE hModule, DWORD Ordinal) {
	DWORD address;
	XexGetProcedureAddress(hModule, Ordinal, &address);
	return address;
}

int Utils::GetRandomValueInRange(int min, int max) {
	srand(time(0));

	int iValue = rand() % max;
	if (iValue > (max - 1)) {
		iValue = 0;
	}

	return iValue;
}

float Utils::GetDistance(Vec3 start, Vec3 end) {
	float dx = end.x - start.x;
	float dy = end.y - start.y;
	float dz = end.z - start.z;
	return (float)sqrt((dx * dx) + (dy * dy) + (dz * dz));
}

DWORD Utils::timeGetTime() {
	return ((DWORD(*)())decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTimeGetTime))();
}

DWORD Utils::Joaat(const char* value) {
	size_t len = strlen(value);
	unsigned int hash, i;
	for (hash = i = 0; i < len; ++i) {
		hash += tolower(value[i]);
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

void Utils::AngleVectors(Vec3* angles, Vec3* forward, Vec3* right, Vec3* up) {
	float angle;
	float sr, sp, sy, cr, cp, cy;
	angle = angles->y * (PI * 2.0f / 360.0f);
	sy = sinf(angle);
	cy = cosf(angle);
	angle = angles->x * (PI * 2.0f / 360.0f);
	sp = sinf(angle);
	cp = cosf(angle);
	angle = angles->z * (PI * 2.0f / 360.0f);
	sr = sinf(angle);
	cr = cosf(angle);

	if (forward) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right) {
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up) {
		up->x = (cr*sp*cy + -sr * -sy);
		up->y = (cr*sp*sy + -sr * cy);
		up->z = cr * cp;
	}
}

void Utils::VectorSubtract(Vec3 a, Vec3 b, Vec3& c) {
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

float Utils::DotProduct(Vec3 hax, Vec3 Vector) {
	return (hax.x * Vector.x) + (hax.y * Vector.y) + (hax.z * Vector.z);
}

void Utils::Ease(float& toEase, float& easeFrom, float multiplier) {
	toEase += toEase < easeFrom ? abs(toEase - easeFrom) / multiplier : -abs(toEase - easeFrom) / multiplier;
}

void Utils::GetFilesFromDirectory(vector<string>& buffer, string folder, string extention) {
	buffer.clear();
	string loc = folder + "\\*" + extention;
	WIN32_FIND_DATAA fd;
	HANDLE hFind = ::FindFirstFileA(loc.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				string orig = fd.cFileName;
				string newstring = orig.substr(0, orig.size() - extention.size());
				buffer.push_back(newstring.c_str());
			}
		} while (::FindNextFileA(hFind, &fd));
		::FindClose(hFind);
	}
}