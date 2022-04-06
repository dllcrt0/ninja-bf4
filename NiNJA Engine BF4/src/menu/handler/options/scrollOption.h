#pragma once

template<typename Type>
class ScrollOption;

template<typename Type>
void v_idk(ScrollOption<Type>*) {}

template<typename Type>
class ScrollOption : public Option {
private:
	void(*FuncOnAction)();
	void(*FuncCurrentOption)();
	void(*FuncOnUpdate)(ScrollOption*);

	ScrollOptionType ScrollType;
	int* pNumber;
	shared_ptr<int> SelectedNumberStorage;
	int* pSelectedNumber;
	bool* pBool;
	int iMin;
	int iMax;

	int iScrollSpeed;

	bool bHasMin;
	bool bHasMax;
	bool bLoop;

	bool bLeftDisabled;
	bool bRightDisabled;
	int iLeftTimer;
	int iRightTimer;

	bool bCanBeSaved;
	string pConfigName;

	ScrollStruct<Type>* pScrollStruct;
public:
	ScrollOption(ScrollOptionType type, const char* name) : Option(name), ScrollType(type), bHasMin(false), bCanBeSaved(false), bHasMax(false), bLoop(false), iScrollSpeed(100), FuncOnAction(nullsub), FuncCurrentOption(nullsub), FuncOnUpdate(v_idk) {}

	ScrollOption& addScroll(int& number, int min, int max, ScrollStruct<Type>* scrollStruct) {
		if (ScrollType == SELECT) {
			SelectedNumberStorage = make_shared<int>(0);
			pSelectedNumber = SelectedNumberStorage.get();
		}

		bIsScrollOption = true;
		bHasMin = true;
		bHasMax = true;
		iMin = min;
		iMax = max;
		pNumber = &number;
		pScrollStruct = scrollStruct;
		return *this;
	}

	ScrollOption& addToggle(bool& toggle) {
		pBool = &toggle;
		return *this;
	}

	ScrollOption& addFunction(void(*func)()) {
		FuncOnAction = func;
		return *this;
	}

	ScrollOption& addOnUpdate(void(*func)(ScrollOption*)) {
		FuncOnUpdate = func;
		return *this;
	}

	ScrollOption& addRequirement(bool(*func)()) {
		FuncRequirement = func;
		return *this;
	}

	ScrollOption& addOnHover(void(*func)()) {
		FuncCurrentOption = func;
		return *this;
	}

	ScrollOption &canLoop() {
		bLoop = true;
		return *this;
	}

	ScrollOption& addTooltip(string tooltip) {
		Tooltip = tooltip;
		return *this;
	}

	void Render(int pos) {
		FuncOnUpdate(this);

		switch (ScrollType) {
		case TOGGLE: {
			auto text = pScrollStruct[*pNumber].pName;
			pDrawText.DrawOptionTextBoldRightAlign(pMenu.IsSelected(pos) ? string(text).append(" ").c_str() : text, 440 - 20, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + 2 + (pos * 22) + 7, 20.f, pMenu.IsSelected(pos) ? pRenderer.UIColors.selectedText : pRenderer.UIColors.text);
			break;
		}

		case SCROLL:
		case SCROLLSELECT:
			auto text = pScrollStruct[*pNumber].pName;
			pDrawText.DrawOptionTextBoldRightAlign(pMenu.IsSelected(pos) ? string(text).append(" ").c_str() : text, 440, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + 2 + (pos * 22) + 7, 20.f, pMenu.IsSelected(pos) ? pRenderer.UIColors.selectedText : pRenderer.UIColors.text);
			break;
		}

		pDrawText.DrawOptionTextBold(pMenu.IsSelected(pos) ? string(szName).append(" ").c_str() : szName, 637 - 195, pMenu.iYPositonEditable + (360 - (pRenderer.fBgPosition / 2) + pRenderer.fPositionAnimFix) + 2 + (pos * 22) + 7, 20.f, pMenu.IsSelected(pos) ? pRenderer.UIColors.selectedText : pRenderer.UIColors.text);
	}

	void RenderSelected(int pos) {
		FuncCurrentOption();

		if (IsOptionPressed() && FuncRequirement()) {
			switch (ScrollType) {
			case TOGGLE: *pBool ^= true; break;
			case SELECT: *pSelectedNumber = *pNumber; break;
			case SCROLLSELECT: FuncOnAction(); break;
			}
		}

		if (IsJustPressed(DPAD_LEFT)) {
			if (!bLeftDisabled) {
				*pNumber -= 1;
				if (bLoop) {
					if (*pNumber < iMin) *pNumber = iMax;
				} else if (*pNumber < iMin && bHasMin) *pNumber = iMin;
				if (ScrollType == SCROLL) FuncOnAction();
				if (ScrollType == TOGGLE && *pBool) FuncOnAction();
			}
			bLeftDisabled = false;
			iLeftTimer = 0;
		}

		if (IsJustPressed(DPAD_RIGHT)) {
			if (!bRightDisabled) {
				*pNumber += 1;
				if (bLoop) {
					if (*pNumber > iMax) *pNumber = iMin;
				} else if (*pNumber > iMax && bHasMax) *pNumber = iMax;
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
				if ((pUtils.timeGetTime() - TIMER) > iScrollSpeed) {
					*pNumber -= 1;
					if (bLoop) {
						if (*pNumber < iMin) *pNumber = iMax;
					} else if (*pNumber < iMin && bHasMin) *pNumber = iMin;
					if (ScrollType == SCROLL) FuncOnAction();
					if (ScrollType == TOGGLE && *pBool) FuncOnAction();
					TIMER = pUtils.timeGetTime();
				}
			}
		}

		if (IsDown(DPAD_RIGHT)) {
			if (++iRightTimer > 10) {
				bRightDisabled = true;
				if ((pUtils.timeGetTime() - TIMER) > iScrollSpeed) {
					*pNumber += 1;
					if (bLoop) {
						if (*pNumber > iMax) *pNumber = iMin;
					} else if (*pNumber > iMax && bHasMax) *pNumber = iMax;
					if (ScrollType == SCROLL) FuncOnAction();
					if (ScrollType == TOGGLE && *pBool) FuncOnAction();
					TIMER = pUtils.timeGetTime();
				}
			}
		}

		pRenderer.SetTooltip(Tooltip);
	}
};