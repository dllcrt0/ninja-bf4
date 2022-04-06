#include "stdafx.h"

Controller pController;

void Controller::Update() {
	pController.PadState[A].bIsDown = Key_IsDown(KEY_A) || pController.bABypass;
	pController.PadState[X].bIsDown = Key_IsDown(KEY_X);
	pController.PadState[Y].bIsDown = Key_IsDown(KEY_Y);
	pController.PadState[B].bIsDown = Key_IsDown(KEY_B) || pController.bBBypass;
	pController.PadState[DPAD_UP].bIsDown = Key_IsDown(KEY_DPAD_UP) || pController.bUpBypass;
	pController.PadState[DPAD_DOWN].bIsDown = Key_IsDown(KEY_DPAD_DOWN) || pController.bDownBypass;
	pController.PadState[DPAD_LEFT].bIsDown = Key_IsDown(KEY_DPAD_LEFT) || pController.bLeftBypass;
	pController.PadState[DPAD_RIGHT].bIsDown = Key_IsDown(KEY_DPAD_RIGHT) || pController.bRightBypass;
	pController.PadState[START].bIsDown = Key_IsDown(KEY_START);
	pController.PadState[BACK].bIsDown = Key_IsDown(KEY_BACK);
	pController.PadState[LEFT_THUMB].bIsDown = Key_IsDown(KEY_LEFT_THUMB);
	pController.PadState[RIGHT_THUMB].bIsDown = Key_IsDown(KEY_RIGHT_THUMB);
	pController.PadState[LEFT_SHOULDER].bIsDown = Key_IsDown(KEY_LEFT_SHOULDER) || pController.bLBBypass;
	pController.PadState[RIGHT_SHOULDER].bIsDown = Key_IsDown(KEY_RIGHT_SHOULDER) || pController.bRBBypass;
	pController.PadState[LEFT_TRIGGER].bIsDown = Key_IsDown(KEY_LEFT_TRIGGER) || pController.bZoomProgress >= 125;
	pController.PadState[RIGHT_TRIGGER].bIsDown = Key_IsDown(KEY_RIGHT_TRIGGER);

	for (int i = 0; i < 16; i++) {
		pController.PadState[i].bIsJustPressed = !pController.PadState[i].bWasJustDown && pController.PadState[i].bIsDown;
		pController.PadState[i].bWasJustDown = pController.PadState[i].bIsDown;
	}

	pController.bABypass = false;
	pController.bBBypass = false;
	pController.bDownBypass = false;
	pController.bLeftBypass = false;
	pController.bRightBypass = false;
	pController.bUpBypass = false;
	pController.bLBBypass = false;
	pController.bRBBypass = false;
	pController.bZoomProgress = 0;
}

bool Controller::IsDown(int key) {
	return PadState[key].bIsDown;
}

bool Controller::IsJustPressed(int key) {
	return PadState[key].bIsJustPressed;
}

bool IsDown(int key) {
	return pController.IsDown(key);
}

bool IsJustPressed(int key) {
	return pController.IsJustPressed(key);
}