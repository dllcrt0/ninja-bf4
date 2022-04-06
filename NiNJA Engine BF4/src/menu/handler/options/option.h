#pragma once

bool DefaultRequirement();
void nullsub();
void nullsub2(const char*);

struct CreateConfigData {
	bool bValid;
	char szName[100];
	char szValue[20];
};

class Option {
protected:
	bool(*FuncRequirement)();
	char szName[100];
	string Tooltip;
	bool bVisible;
	byte bHoverTimer;
	bool bIsScrollOption;

public:
	Option() : bVisible(true), bIsScrollOption(false), FuncRequirement(DefaultRequirement), bHoverTimer(0) {}
	Option(const char* name) : bVisible(true), bIsScrollOption(false), FuncRequirement(DefaultRequirement), bHoverTimer(0) { SetName(name); }

	virtual void Render(int pos);
	virtual void RenderSelected(int pos);

	virtual void SetName(const char* name) {
		strcpy(szName, name);
	}

	void SetVisible(bool visible) { 
		bVisible = visible;
	}

	bool IsVisible() { 
		return bVisible && FuncRequirement();
	}

	bool IsScroll() { 
		return bIsScrollOption;
	}

	const char* GetName() { 
		return szName; 
	}

	string GetTooltip() { 
		return Tooltip;
	}
};