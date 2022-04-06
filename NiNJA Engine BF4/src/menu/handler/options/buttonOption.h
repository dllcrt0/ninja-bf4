#pragma once

class ButtonOption : public Option {
private:
	void(*FuncOnAction)();
	void(*FuncOnActionWithArg)(const char*);
	void(*FuncCurrentOption)();
	void(*FuncOnUpdate)();

	const char* pFuncArg;

	bool bHasKeyboard;
	void(*FuncKeyboardCallback)(const char*);
public:
	ButtonOption(const char* name) : Option(name), bHasKeyboard(false), FuncOnActionWithArg(nullsub2), FuncOnAction(nullsub), FuncCurrentOption(nullsub), FuncOnUpdate(nullsub) {}

	ButtonOption& addFunction(void(*func)()) {
		FuncOnAction = func;
		return *this;
	}

	ButtonOption& addFunctionArg(void(*func)(const char*), const char* arg) {
		pFuncArg = arg;
		FuncOnActionWithArg = func;
		return *this;
	}

	ButtonOption& addOnUpdate(void(*func)()) {
		FuncOnUpdate = func;
		return *this;
	}

	ButtonOption& addOnHover(void(*func)()) {
		FuncCurrentOption = func;
		return *this;
	}

	ButtonOption& addRequirement(bool(*func)()) {
		FuncRequirement = func;
		return *this;
	}

	ButtonOption& addTooltip(string tooltip) {
		Tooltip = tooltip;
		return *this;
	}

	ButtonOption& addKeyboard(void(*func)(const char* str)) {
		FuncKeyboardCallback = func;
		bHasKeyboard = true;
		return *this;
	}

	void Render(int pos);
	void RenderSelected(int pos);
};