#include "stdafx.h"
#include "configMenu.h"
#include "../settingsMenu.h"
#include <locale>
#include <cctype>

using namespace ConfigMenuVars;

namespace ConfigMenuVars {
	Vars vars;

	string &rtrim(string& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
		return s;
	}

	void CreateConfig(const char* pFile) { 
		pConfig.bCreateConfigExt = true;
		pConfig.pCreateConfigName = pFile;
	}

	void LoadConfig() {
		pConfig.bLoadConfigExt = true;
		pConfig.pLoadConfigName = vars.vFilenames[pMenu.iCurrentOption - 2].c_str();
	}
}

void ConfigMenu::Init() {
	SetParentSubmenu<SettingsMenu>();
	SetName("Config");

	addOption(ButtonOption("Create Config")
		.addKeyboard(CreateConfig)
		.addTooltip("Creates a config with your current options."));

	addOption(BreakOption("Configs"));
}

/*Update once when submenu is opened*/
void ConfigMenu::UpdateOnce() {
	ClearOptionsOffset(2);

	pUtils.GetFilesFromDirectory(vars.vFilenames, "NINJA:\\BF-Engine", ".ini");

	for (int i = 0; i < vars.vFilenames.size(); i++) {
		addOption(ButtonOption(pUtils.va("Load: %s", vars.vFilenames[i].c_str()))
			.addFunction(LoadConfig)
			.addTooltip("Load this config."));
	}
}

/*Update while submenu is active*/
void ConfigMenu::Update() {}

/*Background update*/
void ConfigMenu::FeatureUpdate() {}

/*Singleton*/
ConfigMenu* _instance;
ConfigMenu* ConfigMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ConfigMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
ConfigMenu::~ConfigMenu() { delete _instance; }