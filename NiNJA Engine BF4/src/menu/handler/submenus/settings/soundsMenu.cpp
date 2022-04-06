#include "stdafx.h"
#include "soundsMenu.h"
#include "../settingsMenu.h"

using namespace SoundsMenuVars;

namespace SoundsMenuVars {
	Vars vars;
	
	ScrollStruct<int> SoundsTest[] = {
		"None", 0
	};
}

void SoundsMenu::Init() {
	SetParentSubmenu<SettingsMenu>();
	SetName("Sounds");

	addOption(ToggleOption("Killing Spree FX")
		.addToggle(vars.bKillingSpree)
		.addTooltip("Plays the sounds from Halo 2 when you \nhave a killing spree (double kill, etc). Resets \nafter 4.5s without a kill."));

	addOption(ToggleOption("Killing Streak FX")
		.addToggle(vars.bKillingStreak)
		.addTooltip("Plays a sound from Halo 2 when you have a \nkilling streak (kills without dying). Resets \nafter death."));
}

/*Update once when submenu is opened*/
void SoundsMenu::UpdateOnce() {
	ClearOptionsOffset(2);

	for (int i = 1; i < 100; i++) SoundsTest[i].pName = NULL;

	pUtils.GetFilesFromDirectory(vars.vFilenames, "NINJA:\\ninjacache\\media\\sounds", ".xma");

	for (int i = 0; i < vars.vFilenames.size(); i++) {
		SoundsTest[i + 1].pName = vars.vFilenames[i].c_str();
		SoundsTest[i + 1].Result = i + 1;
	}

	addOption(ScrollOption<int>(SCROLL, "On Kill FX").canLoop()
		.addScroll(vars.iOnKill, 0, vars.vFilenames.size(), SoundsTest)
		.addTooltip("Plays a sound of your choosing from inside \nof the sounds folder when you get a kill."));

	addOption(ScrollOption<int>(SCROLL, "On Death FX").canLoop()
		.addScroll(vars.iOnDeath, 0, vars.vFilenames.size(), SoundsTest)
		.addTooltip("Plays a sound of your choosing from inside \nof the sounds folder when you die."));
}

/*Update while submenu is active*/
void SoundsMenu::Update() {}

/*Background update*/
void SoundsMenu::FeatureUpdate() {}

/*Singleton*/
SoundsMenu* _instance;
SoundsMenu* SoundsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new SoundsMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
SoundsMenu::~SoundsMenu() { delete _instance; }