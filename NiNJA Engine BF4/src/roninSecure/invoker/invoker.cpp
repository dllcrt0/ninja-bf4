#include "stdafx.h"

Invoker pInvoker;

void Invoker::RegisterNative(DWORD NativeAddress, NativeHandler Handler) {
	NativeRegistration* pRegistration = &NativeTable[NativeAddress & 0xFF];

	if (pRegistration != nullptr) {
		while (pRegistration->pNextNode != nullptr) {
			pRegistration = pRegistration->pNextNode;
		}

		if (pRegistration->GetCount() == 9) {
			pRegistration->pNextNode = (NativeRegistration*)malloc(sizeof(NativeRegistration));
			pRegistration = pRegistration->pNextNode;
		}

		pRegistration->SetAddress(pRegistration->GetCount(), NativeAddress);
		pRegistration->SetFunction(pRegistration->GetCount(), Handler);
		pRegistration->SetCount(pRegistration->GetCount() + 1);

		DbgPrint("Registered native 0x%X at 0x%X", NativeAddress, (DWORD)Handler);
	}
}

NativeHandler Invoker::GetNativeHandler(DWORD NativeAddress) {
	NativeHandler Handler = nullptr;
	NativeRegistration* pRegistration = &NativeTable[NativeAddress & 0xFF];

	if (pRegistration != nullptr) {
		for (; pRegistration; pRegistration = pRegistration->pNextNode) {
			for (int i = 0; i < pRegistration->GetCount(); i++) {
				if (NativeAddress == pRegistration->GetAddress(i)) {
					Handler = pRegistration->GetFunction(i);
					break;
				}
			}
		}
	}

	return Handler;
}

namespace NativeHandlers {
	void Write1Byte(NativeCallContext* pContext) {
		DWORD dwAddress = pContext->GetArgument<DWORD>(0);
		BYTE bPatchData = pContext->GetArgument<BYTE>(1);

		pMemory.Write<BYTE>(dwAddress, bPatchData);
	}

	void Write2Byte(NativeCallContext* pContext) {
		DWORD dwAddress = pContext->GetArgument<DWORD>(0);
		short sPatchData = pContext->GetArgument<short>(1);

		pMemory.Write<short>(dwAddress, sPatchData);
	}

	void Write4Byte(NativeCallContext* pContext) {
		DWORD dwAddress = pContext->GetArgument<DWORD>(0);
		DWORD dwPatchData = pContext->GetArgument<DWORD>(1);

		pMemory.Write<DWORD>(dwAddress, dwPatchData);
	}

	void WriteFloat(NativeCallContext* pContext) {
		DWORD dwAddress = pContext->GetArgument<DWORD>(0);
		float fPatchData = pContext->GetArgument<float>(1);

		pMemory.Write<float>(dwAddress, fPatchData);
	}

	void Read1Byte(NativeCallContext* pContext) {
		DWORD dwAddress = pContext->GetArgument<DWORD>(0);

		BYTE ret = pMemory.Read<BYTE>(dwAddress);
		pContext->SetResult(0, ret);
	}

	void Read2Byte(NativeCallContext* pContext) {
		DWORD dwAddress = pContext->GetArgument<DWORD>(0);

		short ret = pMemory.Read<short>(dwAddress);
		pContext->SetResult(0, ret);
	}

	void Read4Byte(NativeCallContext* pContext) {
		DWORD dwAddress = pContext->GetArgument<DWORD>(0);

		DWORD ret = pMemory.Read<DWORD>(dwAddress);
		pContext->SetResult(0, ret);
	}

	void ReadFloat(NativeCallContext* pContext) {
		DWORD dwAddress = pContext->GetArgument<DWORD>(0);

		float ret = pMemory.Read<float>(dwAddress);
		pContext->SetResult(0, ret);
	}
}

void Invoker::Initialize() {
	RegisterNative(0xc19d5842, NativeHandlers::Write1Byte);
	RegisterNative(0x97df971a, NativeHandlers::Write2Byte);
	RegisterNative(0xcb6e7cf6, NativeHandlers::Write4Byte);
	RegisterNative(0x4f0198f0, NativeHandlers::WriteFloat);
	RegisterNative(0xa1597e54, NativeHandlers::Read1Byte);
	RegisterNative(0x8e9ec1ac, NativeHandlers::Read2Byte);
	RegisterNative(0x534bb9a4, NativeHandlers::Read4Byte);
	RegisterNative(0x6e8a9c1c, NativeHandlers::ReadFloat);
}