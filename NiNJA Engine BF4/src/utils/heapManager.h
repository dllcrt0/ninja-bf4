#pragma once

class HeapManager {
public:
	DWORD Allocate(int size);
	void Dealloc(DWORD* address);
private:
	vector<DWORD> AllocatedPtrs;
};
