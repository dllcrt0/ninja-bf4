#pragma once
#include "menu/handler/submenu.h"

class ConfigMenu : public Submenu {
public:
	static ConfigMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ConfigMenu() : Submenu() {}
	~ConfigMenu();
};

namespace ConfigMenuVars
{
	struct Vars {
		vector<string> vFilenames;

		bool bResetOnLoad;
		
		const char* pFile;
	};

	string &rtrim(string &s);
	extern Vars vars;
}