#pragma once

class InstructionGenerator {
public:
	DWORD GenerateStackStart();
	DWORD GenerateStackEnd();
	DWORD Generate(DWORD dwInstructionCount);
};

extern InstructionGenerator pInstructionGenerator;

class FunctionRemoval {
public:
	bool HasMarker(DWORD dwAddress, DWORD dwMaxSize);
	void GetMarkers(DWORD dwAddress, DWORD* pdwStart, DWORD* pdwEnd);
};

extern FunctionRemoval pFunctionRemoval;