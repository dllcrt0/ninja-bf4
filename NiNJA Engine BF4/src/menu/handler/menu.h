#pragma once

class Menu {
public:
	bool bShouldUpdateDrawToggles;
	bool bOpen;

	int iYPositonEditable;
	int iCurrentRenderingScroll;
	int iMaxOptions;
	int iCurrentOption;
	int iScrollOffset;
	int iBreakScroll;
	int iSelectedTab;

	bool bFastPressLeft;
	bool bFastPressRight;

	void Render();
	void ASyncUpdate();
	void Init();

	bool IsSelected(int op);
	
	void Open() {
		bOpen = true;
		pSubmenuHandler.SetSubmenuMain();
	}

	void Close() {
		bOpen = false;
	}

	void ToggleOpen() { 
		Open(); 
	}
	
	bool IsOpen() { 
		return bOpen; 
	}
};

extern Menu pMenu;