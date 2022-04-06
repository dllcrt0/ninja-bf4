#include "stdafx.h"

Menu pMenu;

void Menu::Init() {
	DbgPrint("Initializing menu");

	pMenu.iYPositonEditable = -50;
	iMaxOptions = 13;
	//ToggleOpen();
	pSubmenuHandler.Init();
}

void Menu::Render() {
	if (!bOpen) return;

	pRenderer.Render();
}

void Menu::ASyncUpdate() {
	if (IsOpenPressed() && !IsOpen()) {
		ToggleOpen();
	}

	MenuControls();
}

bool Menu::IsSelected(int op) {
	return iCurrentRenderingScroll == (op + 1);
}