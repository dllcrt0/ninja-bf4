#pragma once
#include "menu/handler/submenus/settings/configMenu.h"

template<typename Type>
class NumberOption : public Option {
private:
	void(*FuncOnAction)();
	void(*FuncCurrentOption)();
	void(*FuncOnUpdate)();

	ScrollOptionType ScrollType;
	const char* pFormat;
	Type* pNumber;
	Type OriginalValue;
	bool* pBool;
	Type Step;
	Type Min;
	Type Max;

	shared_ptr<Type> SelectedNumberStorage;
	Type* pSelectedNumber;

	DWORD dwScrollSpeed;

	bool bHasMin;
	bool bHasMax;
	bool bLoop;

	bool bCanBeSaved;
	string pConfigName;

	bool bIsFloat;
public:
	NumberOption(ScrollOptionType type, const char* name, bool isFloat = false) : Option(name), bIsFloat(isFloat), ScrollType(type), bCanBeSaved(false), bHasMin(false), bHasMax(false), bLoop(false), dwScrollSpeed(100), FuncOnAction(nullsub), FuncOnUpdate(nullsub), FuncCurrentOption(nullsub) {}

	NumberOption& addToggle(bool& toggle) {
		pBool = &toggle;
		return *this;
	}

	NumberOption& addNumber(Type& number, const char* format, Type step) {
		if (ScrollType == SELECT) {
			SelectedNumberStorage = make_shared<Type>(0);
			pSelectedNumber = SelectedNumberStorage.get();
		}

		bIsScrollOption = true;
		pNumber = &number;
		OriginalValue = number;
		pFormat = format;
		Step = step;

		return *this;
	}

	NumberOption& addSelectedNumber(Type& selectedNumber) {
		pSelectedNumber = &selectedNumber;
		return *this;
	}

	NumberOption& addMin(Type min) {
		Min = min;
		bHasMin = true;
		return *this;
	}

	NumberOption& addMax(Type max) {
		Max = max;
		bHasMax = true;
		return *this;
	}

	NumberOption& addFunction(void(*func)()) {
		FuncOnAction = func;
		return *this;
	}

	NumberOption& addOnUpdate(void(*func)()) {
		FuncOnUpdate = func;
		return *this;
	}

	NumberOption& addRequirement(bool(*func)()) {
		FuncRequirement = func;
		return *this;
	}

	NumberOption& addOnHover(void(*func)()) {
		FuncOnUpdate = func;
		return *this;
	}

	NumberOption& setScrollSpeed(int speed) {
		dwScrollSpeed = speed;
		return *this;
	}

	NumberOption& addTooltip(string tooltip) {
		Tooltip = tooltip;
		return *this;
	}

	NumberOption& canLoop() {
		bLoop = true;
		return *this;
	}

	void Render(int pos);
	void RenderSelected(int pos);
};