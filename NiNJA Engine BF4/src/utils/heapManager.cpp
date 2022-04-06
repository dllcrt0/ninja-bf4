#include "stdafx.h"

DWORD HeapManager::Allocate(int size) {
	auto address = (DWORD)malloc(size);
	AllocatedPtrs.push_back(address);
	return address;
}

void HeapManager::Dealloc(DWORD* address) {
	delete address;
}