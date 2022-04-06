#pragma once

class Memory {
public:
	template<typename T>
	void Write(DWORD dwAddress, T value) {
		*(T*)dwAddress = value;
	}

	template<typename T>
	T Read(DWORD dwAddress) {
		return *(T*)dwAddress;
	}
};

extern Memory pMemory;