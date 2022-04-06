#include "stdafx.h"
#include "settingsMenu.h"
#include "settings/soundsMenu.h"
#include "settings/configMenu.h"
#include "settings/colorMenu.h"

using namespace SettingsMenuVars;

namespace SettingsMenuVars {
	Vars vars;
}

void SettingsMenu::Init() {
	SetName("Settings");

	addOption(SubmenuOption("Config")
		.addSubmenu<ConfigMenu>()
		.addTooltip("Load and save a config."));

	addOption(SubmenuOption("Sounds")
		.addSubmenu<SoundsMenu>()
		.addTooltip("Enable sounds for the menu."));

	addOption(SubmenuOption("Menu Colors")
		.addSubmenu<ColorMenu>()
		.addTooltip("Choose the colors of the menu, and all of \nits components!"));
}

/*Update once when submenu is opened*/
void SettingsMenu::UpdateOnce() {}

/*Update while submenu is active*/
void SettingsMenu::Update() {}

/*Background update*/
void SettingsMenu::FeatureUpdate() {}

/*Singleton*/
SettingsMenu* _instance;
SettingsMenu* SettingsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new SettingsMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
SettingsMenu::~SettingsMenu() { delete _instance; }