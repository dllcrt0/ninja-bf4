#pragma once
#include "menu/handler/submenus/settings/configMenu.h"

class ToggleOption : public Option {
private:
	void(*FuncOnAction)();
	void(*FuncCurrentOption)();
	void(*FuncOnUpdate)();

	bool* pBool;
	bool pDefBool;
	bool bCanBeSaved;
	string pConfigName;
public:
	ToggleOption(const char* name) : Option(name), FuncOnAction(nullsub), FuncOnUpdate(nullsub), FuncCurrentOption(nullsub), pBool(nullptr) { bCanBeSaved = false; }

	ToggleOption &addToggle(bool& toggle) {
		pBool = &toggle;
		pDefBool = toggle;
		return *this;
	}

	ToggleOption& addFunction(void(*func)()) {
		FuncOnAction = func;
		return *this;
	}

	ToggleOption& addOnUpdate(void(*func)()) {
		FuncOnUpdate = func;
		return *this;
	}

	ToggleOption& addOnHover(void(*func)()) {
		FuncCurrentOption = func;
		return *this;
	}

	ToggleOption& addRequirement(bool(*func)()) {
		FuncRequirement = func;
		return *this;
	}

	ToggleOption& addTooltip(string tooltip) {
		Tooltip = tooltip;
		return *this;
	}

	void Render(int pos);
	void RenderSelected(int pos);
};