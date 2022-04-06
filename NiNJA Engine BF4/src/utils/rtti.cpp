#include "stdafx.h"

RTTI pRTTI;

vector<DWORD> RTTI::BruteOffsetsWithIdent(DWORD dwStart, DWORD dwSize, DWORD dwIdent) {
	vector<DWORD> vec;

	for (int i = 0; i < dwSize; i++) {
		DWORD address = dwStart + i;
		if (ValidPtr((DWORD*)address)) {
			DWORD second = *(DWORD*)address;
			if (ValidPtr((DWORD*)second)) {
				if (*(DWORD*)second == dwIdent) {
					vec.push_back(i);
				}
			}
		}
	}

	return vec;
}