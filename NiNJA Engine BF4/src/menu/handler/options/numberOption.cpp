#include "stdafx.h"

static bool bLeftDisabled = false;
static bool bRightDisabled = false;
static int iLeftTimer = 0;
static int iRightTimer = 0;

template class NumberOption<int>;
template class NumberOption<float>;

template <class Type>
void NumberOption<Type>::Render(int pos) {
	FuncOnUpdate();

	switch (ScrollType) {
		case TOGGLE: {
			auto buffer = 20;

			pRenderer.DrawRect(756 - buffer, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + (pos * 22) + 21, 80, 3, JustifyLeft, GColor(140, 140, 140)); // grey bar
			pRenderer.DrawRect(756 + (float)((*pNumber * 78) / Max) - buffer + 1, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + (pos * 22) + 21 - 5, 1, 13, JustifyLeft, pRenderer.UIColors.slider); // blue bar (value scroller)

			auto text = pUtils.va(pFormat, *pNumber);
			pDrawText.DrawOptionTextBoldRightAlign(pMenu.IsSelected(pos) ? string(text).append(" ").c_str() : text, 352 - buffer, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + 2 + (pos * 22) + 7, 20.f, pMenu.IsSelected(pos) ? pRenderer.UIColors.selectedText : pRenderer.UIColors.text);
			
			GColor colors[2];
			if (*pBool) {
				colors[0] = GColor(0, 167, 255);
				colors[1] = GColor(2, 147, 224);
			} else {
				colors[0] = GColor(0, 0, 0, 170);
				colors[1] = GColor(255, 255, 255);
			}

			pRenderer.DrawOutlinedRect(640 + (410 / 2) - 20, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + (pos * 22) + 17, 10, 10, 1, colors[0], colors[1], true);
			break;
		}

		case SCROLL:
		case SCROLLSELECT:
			pRenderer.DrawRect(756, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + (pos * 22) + 21, 80, 3, JustifyLeft, GColor(140, 140, 140)); // grey bar
			pRenderer.DrawRect(756 + (float)((*pNumber * 78) / Max) + 1, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + (pos * 22) + 21 - 5, 1, 13, JustifyLeft, pRenderer.UIColors.slider); // blue bar (value scroller)

			auto text = pUtils.va(pFormat, *pNumber);
			pDrawText.DrawOptionTextBoldRightAlign(pMenu.IsSelected(pos) ? string(text).append(" ").c_str() : text, 352, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + 2 + (pos * 22) + 7, 20.f, pMenu.IsSelected(pos) ? pRenderer.UIColors.selectedText : pRenderer.UIColors.text);
			break;
	}

	pDrawText.DrawOptionTextBold(pMenu.IsSelected(pos) ? string(szName).append(" ").c_str() : szName, 637 - 195, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + 2 + (pos * 22) + 7, 20.f, pMenu.IsSelected(pos) ? pRenderer.UIColors.selectedText : pRenderer.UIColors.text);
}

template <class Type>
void NumberOption<Type>::RenderSelected(int pos) {
	FuncCurrentOption();

	if (IsOptionPressed() && FuncRequirement()) {
		switch (ScrollType) {
			case TOGGLE: *pBool ^= true; FuncOnAction(); break;
			case SELECT: *pSelectedNumber = *pNumber; break;
			case SCROLL:
			case SCROLLSELECT: FuncOnAction();  break;
		}
	}

	if (IsJustPressed(DPAD_LEFT)) {
		if (!bLeftDisabled) {
			*pNumber -= Step;
			if (bLoop) {
				if (*pNumber < Min) *pNumber = Max;
			} else if (*pNumber < Min && bHasMin) *pNumber = Min;
			if (ScrollType == SCROLL) FuncOnAction();
			if (ScrollType == TOGGLE && *pBool) FuncOnAction();
		}
		bLeftDisabled = false;
		iLeftTimer= 0;
	}

	if (IsJustPressed(DPAD_RIGHT)) {
		if (!bRightDisabled) {
			*pNumber += Step;
			if (bLoop) {
				if (*pNumber > Max) *pNumber = Min;
			} else if (*pNumber > Max && bHasMax) *pNumber = Max;
			if (ScrollType == SCROLL) FuncOnAction();
			if (ScrollType == TOGGLE && *pBool) FuncOnAction();
		}
		bRightDisabled = false;
		iRightTimer = 0;
	}

	static int TIMER;

	if (IsDown(DPAD_LEFT)) {
		if (++iLeftTimer > 10) {
			bLeftDisabled = true;
			if ((pUtils.timeGetTime() - TIMER) > dwScrollSpeed) {
				*pNumber -= Step;
				if (bLoop) {
					if (*pNumber < Min) *pNumber = Max;
				} else if (*pNumber < Min && bHasMin) *pNumber = Min;
				if (ScrollType == SCROLL) FuncOnAction();
				if (ScrollType == TOGGLE && *pBool) FuncOnAction();
				TIMER = pUtils.timeGetTime();
			}
		}
	}

	if (IsDown(DPAD_RIGHT)) {
		if (++iRightTimer > 10) {
			bRightDisabled = true;
			if ((pUtils.timeGetTime() - TIMER) > dwScrollSpeed) {
				*pNumber += Step;
				if (bLoop) {
					if (*pNumber > Max) *pNumber = Min;
				} else if (*pNumber > Max && bHasMax) *pNumber = Max;
				if (ScrollType == SCROLL) FuncOnAction();
				if (ScrollType == TOGGLE && *pBool) FuncOnAction();
				TIMER = pUtils.timeGetTime();
			}
		}
	}
}