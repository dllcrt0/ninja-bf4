#include "stdafx.h"

void BreakOption::Render(int pos) {
	pDrawText.DrawOptionTextBoldCenterAlign(szName, 0, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + 2 + (pos * 22) + 7, 18.f, GColor(255, 255, 255, 255, true));
}

void BreakOption::RenderSelected(int pos) {
	switch (pMenu.iBreakScroll) {
	case 1: //Up
		if (pMenu.iScrollOffset > 0 && pMenu.iCurrentOption - pMenu.iScrollOffset == 0)pMenu.iScrollOffset--;
		if (pMenu.iCurrentOption - 1 < 0)Bottom();
		else pMenu.iCurrentOption--;
		break;
	case 2: //Down
		if (pMenu.iScrollOffset < pSubmenuHandler.GetTotalOptions() - pMenu.iMaxOptions && pMenu.iCurrentOption + 1 - pMenu.iScrollOffset == pMenu.iMaxOptions) pMenu.iScrollOffset++;
		if (pMenu.iCurrentOption + 1 >= pSubmenuHandler.GetTotalOptions())Top();
		else pMenu.iCurrentOption++;
		break;
	case 3: //Bottom
		if (pMenu.iScrollOffset > 0 && pMenu.iCurrentOption - pMenu.iScrollOffset == 1)pMenu.iScrollOffset--;
		pMenu.iCurrentOption--; break;
	case 4: //Top
		if (pMenu.iScrollOffset < pSubmenuHandler.GetTotalOptions() - pMenu.iMaxOptions && pSubmenuHandler.GetTotalOptions() >= pMenu.iMaxOptions && pMenu.iCurrentOption - pMenu.iScrollOffset == pMenu.iMaxOptions) pMenu.iScrollOffset++;
		pMenu.iCurrentOption++; break;
	default:
		break;
	}
}
