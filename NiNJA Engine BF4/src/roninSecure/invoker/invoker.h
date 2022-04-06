#pragma once

class NativeCallContext {
protected:
	void* pReturn;
	int iArgCount;
	void* pArgs;

public:
	template<typename type>
	inline type GetArgument(int Index) {
		intptr_t* args = (intptr_t*)pArgs;
		return *(type*)&args[Index];
	}

	template<typename type>
	inline void SetResult(int Index, type Value) {
		intptr_t* returnValue = (intptr_t*)pReturn;
		*(type*)&returnValue[Index] = Value;
	}

	template<typename returnType>
	inline returnType GetResult(int Index) {
		intptr_t* returnValue = (intptr_t*)pReturn;
		return *(returnType*)&returnValue[Index];
	}

	inline int GetArgCount() {
		return iArgCount;
	}
};

class NativeContext : public NativeCallContext {
private:
	enum {
		maxNativeParams = 20,
		argSize = 4
	};

	byte szTempStack[maxNativeParams * argSize];

public:
	inline NativeContext() {
		pArgs = &szTempStack;
		pReturn = &szTempStack;

		iArgCount = 0;

		memset(szTempStack, 0, sizeof(szTempStack));
	}

	template<typename type>
	inline void Push(type value) {
		if (sizeof(type) < argSize)
			*(uintptr_t*)(szTempStack + argSize * iArgCount) = 0;

		*(type*)(szTempStack + argSize * iArgCount) = value;
		iArgCount++;
	}

	inline void Reverse() {
		uintptr_t tempValues[maxNativeParams];
		uintptr_t* args = (uintptr_t*)pArgs;

		for (int i = 0; i < iArgCount; i++) {
			int target = iArgCount - i - 1;
			tempValues[target] = args[i];
		}

		memcpy(szTempStack, tempValues, sizeof(tempValues));
	}

	template<typename type>
	inline type GetResult() {
		return *(type*)(m_tempStack);
	}
};

class NativeManager : public NativeContext {
public:
	NativeManager()
		:NativeContext() {
	}

	void Reset() {
		iArgCount = 0;
	}

	inline void* GetResultPointer() {
		return pReturn;
	}
};

static int NativeRegistrationKeys[28] = {
	0x73B7B7C2,0x65B7B7C3,0x65B7B7C4,0x65B7B7C5,0x65B7B7C6,0x65B7B7C7,0x65B7B7C8,0x65B7B7C9,0x65B7B7d2,0x65B7B7e2,0x65B7B7f2,0x65B7B8C2,0x65B7B9C2,0x65B7B0C2,
	0x65B7B712,0x73B7B722,0x65B7B732,0x73B7B742,0x65B6B7C2,0x65B7B7C2,0x65B5B7C2,0x65B4B7C2,0x65B3B7C2,0x65B327C2,0x71B7B7C2,0x72B7B7C2,0x73B7B7C2,0x65B7B7C2
};

typedef void(*NativeHandler)(NativeCallContext*);
struct NativeRegistration {
	NativeRegistration* pNextNode;
	NativeHandler Function[10];
	DWORD dwCount;
	DWORD dwAddress[10];
	bool bFirst;

	inline NativeHandler GetFunction(int index) {
		DWORD func = (DWORD)Function[index];

		func ^= (index + 12) ^ 10;
		func ^= index;
		func += 10;
		func ^= 69;
		func -= 1337;
		func ^= 0x20;
		func ^= 0x10;

		return (NativeHandler)func;
	}

	inline DWORD GetCount() {
		if (!bFirst) return 0;
		DWORD count = dwCount;

		count ^= NativeRegistrationKeys[2] + 10;
		count ^= NativeRegistrationKeys[3];
		count ^= NativeRegistrationKeys[1];
		count ^= 0x26;
		count ^= 0x25;
		count ^= 0x24;
		count ^= 0x23;
		count ^= 0x22;
		count ^= 0x21;
		count ^= 0x20;
		count -= 1337;
		count ^= 0x12;

		return count;
	}

	inline DWORD GetAddress(int index) {
		DWORD address = dwAddress[index];

		address ^= (index + 5) ^ 8;
		address ^= index;
		address += 5;
		address ^= 69;
		address -= 1327;
		address ^= 0x40;
		address ^= 0x11;

		return address;
	}

	inline void SetFunction(int index, NativeHandler func) {
		DWORD f = (DWORD)func;

		f ^= 0x10;
		f ^= 0x20;
		f += 1337;
		f ^= 69;
		f -= 10;
		f ^= index;
		f ^= (index + 12) ^ 10;

		Function[index] = (NativeHandler)f;
	}

	inline void SetCount(DWORD count) {
		bFirst = true;

		count ^= 0x12;
		count += 1337;
		count ^= 0x20;
		count ^= 0x21;
		count ^= 0x22;
		count ^= 0x23;
		count ^= 0x24;
		count ^= 0x25;
		count ^= 0x26;
		count ^= NativeRegistrationKeys[1];
		count ^= NativeRegistrationKeys[3];
		count ^= NativeRegistrationKeys[2] + 10;

		dwCount = count;
	}

	inline void SetAddress(int index, DWORD address) {
		address ^= 0x11;
		address ^= 0x40;
		address += 1327;
		address ^= 69;
		address -= 5;
		address ^= index;
		address ^= (index + 5) ^ 8;

		dwAddress[index] = address;
	}
};

class Invoker {
public:
	void Initialize();
	void RegisterNative(DWORD NativeAddress, NativeHandler Handler);
	NativeHandler GetNativeHandler(DWORD NativeAddress);

	int* Call(DWORD NativeAddress, NativeManager* Context) {
		int* result = nullptr;
		NativeHandler handler = GetNativeHandler(NativeAddress);
		if (handler) {
			__try {
				handler(Context);
			} __except (true) {}

			result = (int*)Context->GetResultPointer();
		}

		return result;
	}

	template<typename R>
	R Invoke(DWORD NativeAddress) {
		NativeManager context;
		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1>
	R Invoke(DWORD NativeAddress, T1 P1) {
		NativeManager context;

		context.Push(P1);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);
		context.Push(P12);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);
		context.Push(P12);
		context.Push(P13);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);
		context.Push(P12);
		context.Push(P13);
		context.Push(P14);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);
		context.Push(P12);
		context.Push(P13);
		context.Push(P14);
		context.Push(P15);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);
		context.Push(P12);
		context.Push(P13);
		context.Push(P14);
		context.Push(P15);
		context.Push(P16);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);
		context.Push(P12);
		context.Push(P13);
		context.Push(P14);
		context.Push(P15);
		context.Push(P16);
		context.Push(P17);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);
		context.Push(P12);
		context.Push(P13);
		context.Push(P14);
		context.Push(P15);
		context.Push(P16);
		context.Push(P17);
		context.Push(P18);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18, T19 P19) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);
		context.Push(P12);
		context.Push(P13);
		context.Push(P14);
		context.Push(P15);
		context.Push(P16);
		context.Push(P17);
		context.Push(P18);
		context.Push(P19);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18, T19 P19, T20 P20) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);
		context.Push(P12);
		context.Push(P13);
		context.Push(P14);
		context.Push(P15);
		context.Push(P16);
		context.Push(P17);
		context.Push(P18);
		context.Push(P19);
		context.Push(P20);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18, T19 P19, T20 P20, T21 P21) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);
		context.Push(P12);
		context.Push(P13);
		context.Push(P14);
		context.Push(P15);
		context.Push(P16);
		context.Push(P17);
		context.Push(P18);
		context.Push(P19);
		context.Push(P20);
		context.Push(P21);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18, T19 P19, T20 P20, T21 P21, T22 P22) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);
		context.Push(P12);
		context.Push(P13);
		context.Push(P14);
		context.Push(P15);
		context.Push(P16);
		context.Push(P17);
		context.Push(P18);
		context.Push(P19);
		context.Push(P20);
		context.Push(P21);
		context.Push(P22);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18, T19 P19, T20 P20, T21 P21, T22 P22, T23 P23) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);
		context.Push(P12);
		context.Push(P13);
		context.Push(P14);
		context.Push(P15);
		context.Push(P16);
		context.Push(P17);
		context.Push(P18);
		context.Push(P19);
		context.Push(P20);
		context.Push(P21);
		context.Push(P22);
		context.Push(P23);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18, T19 P19, T20 P20, T21 P21, T22 P22, T23 P23, T24 P24) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);
		context.Push(P12);
		context.Push(P13);
		context.Push(P14);
		context.Push(P15);
		context.Push(P16);
		context.Push(P17);
		context.Push(P18);
		context.Push(P19);
		context.Push(P20);
		context.Push(P21);
		context.Push(P22);
		context.Push(P23);
		context.Push(P24);

		return *(R*)Call(NativeAddress, &context);
	}

	template <typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16, typename T17, typename T18, typename T19, typename T20, typename T21, typename T22, typename T23, typename T24, typename T25>
	R Invoke(DWORD NativeAddress, T1 P1, T2 P2, T3 P3, T4 P4, T5 P5, T6 P6, T7 P7, T8 P8, T9 P9, T10 P10, T11 P11, T12 P12, T13 P13, T14 P14, T15 P15, T16 P16, T17 P17, T18 P18, T19 P19, T20 P20, T21 P21, T22 P22, T23 P23, T24 P24, T25 P25) {
		NativeManager context;

		context.Push(P1);
		context.Push(P2);
		context.Push(P3);
		context.Push(P4);
		context.Push(P5);
		context.Push(P6);
		context.Push(P7);
		context.Push(P8);
		context.Push(P9);
		context.Push(P10);
		context.Push(P11);
		context.Push(P12);
		context.Push(P13);
		context.Push(P14);
		context.Push(P15);
		context.Push(P16);
		context.Push(P17);
		context.Push(P18);
		context.Push(P19);
		context.Push(P20);
		context.Push(P21);
		context.Push(P22);
		context.Push(P23);
		context.Push(P24);
		context.Push(P25);

		return *(R*)Call(NativeAddress, &context);
	}

	NativeRegistration* GetNatives(int index) {
		return &NativeTable[index];
	}

private:
	NativeRegistration NativeTable[256];
};

extern Invoker pInvoker;