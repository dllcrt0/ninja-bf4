#include "stdafx.h"

void Runtime::Initialize(BYTE bKey, void* pFunction, int iSize) {
	for (int i = 0; i < iSize; i++) {
		*(BYTE*)((DWORD)pFunction + i) = *(BYTE*)((DWORD)pFunction + i) ^ bKey;
	}

	Functions.push_back(make_pair((DWORD)pFunction, iSize));
}

void Runtime::Delete(void* pFunction, int iSize) {
	for (int i = 0; i < iSize; i++) {
		*(BYTE*)((DWORD)pFunction + i) = 0x0;
	}
}

void Runtime::Fuse(BYTE bKey, void* pFunction) {
	bool exists = false;
	int index = 0;
	
	for (int i = 0; i < Functions.size(); i++) {
		if (Functions[i].first == (DWORD)pFunction) {
			exists = true;
			index = i;
			break;
		}
	}

	if (exists) {
		for (int i = 0; i < Functions[index].second; i++) {
			*(BYTE*)((DWORD)pFunction + i) = *(BYTE*)((DWORD)pFunction + i) ^ bKey;
		}
	}
}