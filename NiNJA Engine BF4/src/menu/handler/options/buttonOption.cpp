#include "stdafx.h"

DWORD WINAPI KeyboardHandlerThread(LPVOID lp) {
	XOVERLAPPED Overlapped;
	ZeroMemory(&Overlapped, sizeof(Overlapped));

	WCHAR wideBuffer[0x100];
	XShowKeyboardUI(0, VKBD_DEFAULT, L"", L"NiNJA Engine", L"Enter the text here!", wideBuffer, 0x100, &Overlapped);
	
	while (!XHasOverlappedIoCompleted(&Overlapped)) Sleep(1000);

	if (lstrlenW(wideBuffer)) {
		char lowKeyboardBuffer[0x100];
		wcstombs(lowKeyboardBuffer, wideBuffer, 0x100);

		if (strlen(lowKeyboardBuffer)) {
			Invoke::Call<int>((DWORD)lp, lowKeyboardBuffer);
		}
	}

	return 0;
}

void ButtonOption::Render(int pos) {
	FuncOnUpdate();
	
	pDrawText.DrawOptionTextBold(pMenu.IsSelected(pos) ? string(szName).append(" ").c_str() : szName, 637 - 195, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + 2 + (pos * 22) + 7, 20.f, pMenu.IsSelected(pos) ? pRenderer.UIColors.selectedText : pRenderer.UIColors.text);
}

void ButtonOption::RenderSelected(int pos) {
	FuncCurrentOption();

	if (IsOptionPressed() && FuncRequirement()) {
		if (bHasKeyboard) {
			CreateThread(0, 0, KeyboardHandlerThread, FuncKeyboardCallback, 0, 0);
		}

		if (FuncOnActionWithArg) {
			FuncOnActionWithArg(pFuncArg);
		}

		FuncOnAction();
	}
}