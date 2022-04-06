#include "stdafx.h"

InstructionGenerator pInstructionGenerator;
FunctionRemoval pFunctionRemoval;

__declspec(noinline) DWORD InstructionGenerator::GenerateStackStart() {
	BYTE szStackStart[] = {
		0x7D, 0x88, 0x02, 0xA6, // mflr %r12
		0x91, 0x81, 0xFF, 0xF8, // stw %r12, -8(%r1)
		0xFB, 0xC1, 0xFF, 0xE8, // std %r30, -0x18(%r1)
		0xFB, 0xE1, 0xFF, 0xF0, // std %r31, -0x10(%r1)
		0x94, 0x21, 0xF6, 0xDD  // stwu %r1, -0x923(%r1)
	};

	DWORD allocated = (DWORD)malloc(sizeof(szStackStart));
	memcpy((void*)allocated, szStackStart, sizeof(szStackStart));
	return allocated;
}

__declspec(noinline) DWORD InstructionGenerator::GenerateStackEnd() {
	BYTE szStackEnd[] = {
		0x38, 0x21, 0x09, 0x23, // addi %r1, %r1, 0x923
		0x81, 0x81, 0xFF, 0xF8, // lwz %r12, -8(%r1)
		0x7D, 0x88, 0x03, 0xA6, // mtlr %r12
		0xEB, 0xC1, 0xFF, 0xE8, // ld %r30, -0x18(%r1)
		0xEB, 0xE1, 0xFF, 0xF0, // ld %r31, -0x10(%r1)
		0x4E, 0x80, 0x00, 0x20  // blr
	};

	DWORD allocated = (DWORD)malloc(sizeof(szStackEnd));
	memcpy((void*)allocated, szStackEnd, sizeof(szStackEnd));
	return allocated;
}

__declspec(noinline) DWORD InstructionGenerator::Generate(DWORD dwInstructionCount) {
	DWORD dwInstructions[] = {
		0x7C832378, // mr r3, r4
		0x3D408991, // lis r10, ClientGameContext@h
		0x816A6E78, // lwz r11, ClientGameContext@l(r10)
		0x806B0030, // lwz r3, 0x30(r11)
		0x7C7F1B78, // mr r31, r3
		0x39600000, // li r11, 0
		0x556A0030, // clrrwi r10, r11, 7
		0x38800007, // li r4, 7
		0x7C9C2378, // mr r28, r4
		0x7CBA2B78, // mr r26, r5
		0x7CD83378, // mr r24, r6
		0x7CFD3B78, // mr r29, r7
		0x7D194378, // mr r25, r8
		0x7D374B78, // mr r25, r8
		0x7D5B5378, // mr r27, r10
		0x387E019C, // addi r3, r30, 0x19C
		0x38800020, // li r4, 0x20
		0x7D2A5850, // subf r9, r10, r11
		0x7D281670, // srawi r8, r9, 2
	};

	DWORD allocated = (DWORD)malloc(dwInstructionCount * 4);
	if (allocated) {
		for (DWORD i = 0; i < dwInstructionCount; i++) {
			*(DWORD*)(allocated + (i * 4)) = dwInstructions[pUtils.GetRandomValueInRange(0, 19)];
		}
	}

	return allocated;
}

__declspec(noinline) bool FunctionRemoval::HasMarker(DWORD dwAddress, DWORD dwMaxSize) {
	if (dwAddress) {
		DWORD* address = (DWORD*)dwAddress;

		for (DWORD i = 0; i < dwMaxSize; i++) {
			if (Natives::Read4Byte((DWORD)&address[i]) == 0x7C842378
				&& Natives::Read4Byte((DWORD)&address[i + 1]) == 0x7CA52B78) {
				return true;
			}
		}
	}

	return false;
}

__declspec(noinline) void FunctionRemoval::GetMarkers(DWORD dwAddress, DWORD* pdwStart, DWORD* pdwEnd) {
	if (dwAddress) {
		DWORD* address = (DWORD*)dwAddress;

		for (DWORD i = 0; i < 0x1000; i++) {
			if (address[i] == 0x7C842378
				&& address[i + 1] == 0x7CA52B78) {
				if (pdwStart) *pdwStart = dwAddress + (i * 4);
			} else if (address[i] == 0x7CA52B78
				&& address[i + 1] == 0x7C842378) {
				if (pdwEnd) *pdwEnd = dwAddress + (i * 4) + 8;
				break;
			}
		}
	}
}