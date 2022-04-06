#pragma once

class Submenu;

class SubmenuOption : public Option {
private:
	void(*FuncOnAction)();
	void(*FuncCurrentOption)();
	void(*FuncOnUpdate)();

	Submenu* pSubmenu;

public:
	SubmenuOption(const char* name) : Option(name), pSubmenu(nullptr), FuncOnAction(nullsub), FuncCurrentOption(nullsub), FuncOnUpdate(nullsub) {}

	template<typename T>
	SubmenuOption& addSubmenu() {
		pSubmenu = T::GetInstance();
		return *this;
	}

	SubmenuOption& addSubmenu(Submenu* submenu) {
		pSubmenu = submenu;
		return *this;
	}

	SubmenuOption& addFunction(void(*func)()) {
		FuncOnAction = func;
		return *this;
	}

	SubmenuOption& addOnUpdate(void(*func)()) {
		FuncOnUpdate = func;
		return *this;
	}

	SubmenuOption& addOnHover(void(*func)()) {
		FuncCurrentOption = func;
		return *this;
	}

	SubmenuOption& setVisible(bool visible) {
		bVisible = visible;
		return *this;
	}

	SubmenuOption& addName(const char* name) {
		strcpy(szName, name);
		return *this;
	}

	SubmenuOption& addTooltip(string tooltip) {
		Tooltip = tooltip;
		return *this;
	}

	SubmenuOption& addRequirement(bool(*func)()) {
		FuncRequirement = func;
		return *this;
	}

	void Render(int pos);
	void RenderSelected(int pos);
};