#pragma once

class BreakOption : public Option {
public:
	BreakOption(const char* name) : Option(name) {}

	BreakOption& addRequirement(bool(*func)()) {
		FuncRequirement = func;
		return *this;
	}

	void Render(int pos);
	void RenderSelected(int pos);
};