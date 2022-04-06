#pragma once

class SubmenuHandler {
private:
	Submenu* pMainSubmenu;
	Submenu* pCurrentSubmenu;
	Submenu* pNextSubmenu;

	int iNextCurrentOption;
	int iNextScrollOffset;

	vector<Submenu*> pSubmenus;
public:
	int iCurrentOptionsWithoutBreaks;
	int iTotalOptionsWithoutBreaks;
	int iTotalOptions;

	void Init();
	void Update();
	void ASyncUpdate();
	void SetSubmenu(Submenu* submenu);
	void SetSubmenuMain();
	void SetSubmenuPrevious(bool resetPos);

	Submenu* GetCurrentSubmenu() {
		return pCurrentSubmenu;
	}

	vector<Submenu*> GetSubmenus() {
		return pSubmenus;
	}

	void AddSubmenu(Submenu* submenu) {
		pSubmenus.push_back(submenu);
	}

	int GetTotalOptions() {
		return pCurrentSubmenu->GetOptionAmount();
	}

	template<typename T>
	bool IsCurrentSubmenuOfType() {
		return dynamic_cast<const T*>(GetCurrentSubmenu()) != nullptr;
	}
};

extern SubmenuHandler pSubmenuHandler;