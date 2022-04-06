#pragma once

class RTTI {
public:
	vector<DWORD> BruteOffsetsWithIdent(DWORD dwStart, DWORD dwSize, DWORD dwIdent);
};

extern RTTI pRTTI;