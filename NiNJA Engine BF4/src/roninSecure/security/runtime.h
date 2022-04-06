#pragma once

class Runtime {
public:
	void Delete(void* pFunction, int iSize);
	void Initialize(BYTE bKey, void* pFunction, int iSize);
	
	template<typename R>
	__declspec(noinline) R Call(BYTE bKey, void* pFunction) {
		Fuse(bKey, pFunction);
		R response = ((R(*)())pFunction)();
		Fuse(bKey, pFunction);
		return response;
	}

	template<typename R, typename T1>
	__declspec(noinline) R Call(BYTE bKey, void* pFunction, T1 p1) {
		Fuse(bKey, pFunction);
		R response = ((R(*)(T1))pFunction)(p1);
		Fuse(bKey, pFunction);
		return response;
	}

	template<typename R, typename T1, typename T2>
	__declspec(noinline) R Call(BYTE bKey, void* pFunction, T1 p1, T2 p2) {
		Fuse(bKey, pFunction);
		R response = ((R(*)(T1, T2))pFunction)(p1, p2);
		Fuse(bKey, pFunction);
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3>
	__declspec(noinline) R Call(BYTE bKey, void* pFunction, T1 p1, T2 p2, T3 p3) {
		Fuse(bKey, pFunction);
		R response = ((R(*)(T1, T2, T3))pFunction)(p1, p2, p3);
		Fuse(bKey, pFunction);
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4>
	__declspec(noinline) R Call(BYTE bKey, void* pFunction, T1 p1, T2 p2, T3 p3, T4 p4) {
		Fuse(bKey, pFunction);
		R response = ((R(*)(T1, T2, T3, T4))pFunction)(p1, p2, p3, p4);
		Fuse(bKey, pFunction);
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
	__declspec(noinline) R Call(BYTE bKey, void* pFunction, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5) {
		Fuse(bKey, pFunction);
		R response = ((R(*)(T1, T2, T3, T4, T5))pFunction)(p1, p2, p3, p4, p5);
		Fuse(bKey, pFunction);
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6>
	__declspec(noinline) R Call(BYTE bKey, void* pFunction, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6) {
		Fuse(bKey, pFunction);
		R response = ((R(*)(T1, T2, T3, T4, T5, T6))pFunction)(p1, p2, p3, p4, p5, p6);
		Fuse(bKey, pFunction);
		return response;
	}

	template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5, typename T6, typename T7>
	__declspec(noinline) R Call(BYTE bKey, void* pFunction, T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7) {
		Fuse(bKey, pFunction);
		R response = ((R(*)(T1, T2, T3, T4, T5, T6, T7))pFunction)(p1, p2, p3, p4, p5, p6, p7);
		Fuse(bKey, pFunction);
		return response;
	}

private:
	vector<pair<DWORD, int>> Functions;

	void Fuse(BYTE bKey, void* pFunction);
};