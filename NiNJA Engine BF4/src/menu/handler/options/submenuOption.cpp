#include "stdafx.h"

void SubmenuOption::Render(int pos) {
	FuncOnUpdate();

	pDrawText.DrawOptionTextBold(pMenu.IsSelected(pos) ? string(szName).append(" ").c_str() : szName, 637 - 195, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + 2 + (pos * 22) + 7, 20.f, pMenu.IsSelected(pos) ? pRenderer.UIColors.selectedText : pRenderer.UIColors.text);
	//pDrawText.DrawOptionTextBold(pMenu.IsSelected(pos) ? string(szName).append(" ").c_str() : szName, 637 - 195, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + 2 + (pos * 22) + 7, 20.f, pMenu.IsSelected(pos) ? pRenderer.UIColors.selectedText : pRenderer.UIColors.text);
}

void SubmenuOption::RenderSelected(int position) {
	FuncCurrentOption();
	if (FuncRequirement() && IsOptionPressed()) {
		FuncOnAction();
		pSubmenuHandler.SetSubmenu(pSubmenu);
	}
}