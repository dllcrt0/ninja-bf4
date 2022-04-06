#pragma once

enum eGamepadButtons {
	KEY_DPAD_UP = 0x00000001,
	KEY_DPAD_DOWN = 0x00000002,
	KEY_DPAD_LEFT = 0x00000004,
	KEY_DPAD_RIGHT = 0x00000008,
	KEY_START = 0x00001000,
	KEY_BACK = 0x00002000,
	KEY_LEFT_THUMB = 0x00000400,
	KEY_RIGHT_THUMB = 0x00000800,
	KEY_LEFT_SHOULDER = 0x00010000,
	KEY_RIGHT_SHOULDER = 0x00020000,
	KEY_A = 0x00000020,
	KEY_B = 0x00000080,
	KEY_X = 0x00000040,
	KEY_Y = 0x00000010,
	KEY_LEFT_TRIGGER = 0x00004000, // 0xc000
	KEY_RIGHT_TRIGGER = 0x00008000
};

enum eGamePadIndex {
	DPAD_UP,
	DPAD_DOWN,
	DPAD_LEFT,
	DPAD_RIGHT,
	START,
	BACK,
	LEFT_THUMB,
	RIGHT_THUMB,
	LEFT_SHOULDER,
	RIGHT_SHOULDER,
	A,
	B,
	X,
	Y,
	LEFT_TRIGGER,
	RIGHT_TRIGGER
};

static bool Key_IsDown(int key) {
	if (ValidPtr(pGame.GetButtonSetup())) {
		if (ValidPtr(pGame.GetButtonSetup()->pButtonMonitor)) {
			return pGame.GetButtonSetup()->pButtonMonitor->nButton & key;
		}
	}

	return false;
}

static bool Key_IsComboDown(int key) {
	if (ValidPtr(pGame.GetButtonSetup())) {
		if (ValidPtr(pGame.GetButtonSetup()->pButtonMonitor)) {
			return pGame.GetButtonSetup()->pButtonMonitor->nButton == key;
		}
	}

	return false;
}

struct GamePadState {
	bool bIsDown;
	bool bIsJustPressed;
	bool bWasJustDown;

	void Reset() {
		bIsDown = false;
		bWasJustDown = false;
	}
};

class Controller {
public:
	GamePadState PadState[16];
	bool bLeftBypass, bRightBypass, bUpBypass, bDownBypass, bABypass, bBBypass, bLBBypass, bRBBypass;
	BYTE bZoomProgress;
	bool bCanFly;
	int FlyThumb;

	static void Update();
	bool IsJustPressed(int key);
	bool IsDown(int key);
};

extern Controller pController;

bool IsDown(int key);
bool IsJustPressed(int key);