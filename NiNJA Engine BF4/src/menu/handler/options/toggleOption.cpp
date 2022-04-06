#include "stdafx.h"
#include "menu/handler/submenus/settings/configMenu.h"

void ToggleOption::Render(int pos) {
	FuncOnUpdate();
	
	GColor colors[2];
	if (*pBool) {
		colors[0] = GColor(0, 167, 255);
		colors[1] = GColor(2, 147, 224);
	} else {
		colors[0] = GColor(0, 0, 0, 170);
		colors[1] = GColor(255, 255, 255);
	}

	pRenderer.DrawOutlinedRect(640 + (410 / 2) - 20, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + (pos * 22) + 17, 10, 10, 1, colors[0], colors[1], true);

	pDrawText.DrawOptionTextBold(pMenu.IsSelected(pos) ? string(szName).append(" ").c_str() : szName, 637 - 195, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + 2 + (pos * 22) + 7, 20.f, pMenu.IsSelected(pos) ? pRenderer.UIColors.selectedText : pRenderer.UIColors.text);
}

void ToggleOption::RenderSelected(int pos) {
	FuncCurrentOption();
	if (IsOptionPressed() && FuncRequirement()) {
		*pBool ^= true;
		FuncOnAction();
	}
}