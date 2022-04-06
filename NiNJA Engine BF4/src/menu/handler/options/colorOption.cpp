#include "stdafx.h"

void ColorOption::Render(int pos) {
	FuncOnUpdate();

	if (bPreviewBox && pColor) {
		pRenderer.DrawOutlinedRect(640 + (410 / 2) - 20, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + (pos * 22) + 17, 10, 10, 1, *pColor, *pColor, true);
	}

	pDrawText.DrawOptionTextBold(pMenu.IsSelected(pos) ? string(szName).append(" ").c_str() : szName, 637 - 195, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + 2 + (pos * 22) + 7, 20.f, pMenu.IsSelected(pos) ? pRenderer.UIColors.selectedText : pRenderer.UIColors.text);
}

void ColorOption::RenderSelected(int position) {
	FuncCurrentOption();
	if (IsOptionPressed()) {
		FuncOnAction();

		if (pSubmenu) {
			pSubmenuHandler.SetSubmenu(pSubmenu);
		}
	}
}