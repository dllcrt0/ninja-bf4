#pragma once

namespace Natives {
	static void Write1Byte(DWORD dwAddress, BYTE bValue) {
		pInvoker.Invoke<int>(0xc19d5842, dwAddress, bValue);
	}

	static void Write2Byte(DWORD dwAddress, short sValue) {
		pInvoker.Invoke<int>(0x97df971a, dwAddress, sValue);
	}

	static void Write4Byte(DWORD dwAddress, DWORD dwValue) {
		pInvoker.Invoke<int>(0xcb6e7cf6, dwAddress, dwValue);
	}

	static void WriteFloat(DWORD dwAddress, float fValue) {
		pInvoker.Invoke<int>(0x4f0198f0, dwAddress, fValue);
	}

	static BYTE Read1Byte(DWORD dwAddress) {
		return pInvoker.Invoke<BYTE>(0xa1597e54, dwAddress);
	}

	static short Read2Byte(DWORD dwAddress) {
		return pInvoker.Invoke<short>(0x8e9ec1ac, dwAddress);
	}

	static DWORD Read4Byte(DWORD dwAddress) {
		return pInvoker.Invoke<DWORD>(0x534bb9a4, dwAddress);
	}

	static float ReadFloat(DWORD dwAddress) {
		return pInvoker.Invoke<float>(0x6e8a9c1c, dwAddress);
	}
}