#include "stdafx.h"
#include "serverSettingsMenu.h"
#include "../currentGameMenu.h"

using namespace ServerSettingsMenuVars;

namespace ServerSettingsMenuVars {
	Vars vars;
	
	void UpdateStuff() {
		auto synced = pGame.GetSyncedBFSettings();
		if (ValidPtr(synced)) {
			synced->bAllUnlocksUnlocked = vars.bAllUnlocksUnlocked;
			synced->bHideMinimap = vars.bHideMinimap;
			synced->bHideHUD = vars.bHideHUD;
			synced->bNoMinimapSpotting = vars.bNoMinimapSpotting;
			synced->bNo3DSpotting = vars.bNo3DSpotting;
			synced->bNoNameTags = vars.bNoNameTags;
			synced->bOnlySquadLeaderSpawn = vars.bOnlySquadLeaderSpawn;
			synced->bTeamSwitchingAllowed = vars.bTeamSwitchingAllowed;
			synced->bDisableHitIndicators = vars.bDisableHitIndicators;
		}
	}
}

void ServerSettingsMenu::Init() {
	SetParentSubmenu<CurrentGameMenu>();
	SetName("Server Settings");

	addOption(ToggleOption("Disable Minimap")
		.addToggle(vars.bHideMinimap)
		.addFunction(UpdateStuff)
		.addTooltip("Stops the minimap from showing."));

	addOption(ToggleOption("Disable HUD")
		.addToggle(vars.bHideHUD)
		.addFunction(UpdateStuff)
		.addTooltip("Stops the HUD elements from showing \n(ammo counter, etc)."));

	addOption(ToggleOption("Disable Minimap Spotting")
		.addToggle(vars.bNoMinimapSpotting)
		.addFunction(UpdateStuff)
		.addTooltip("Stops spots showing on the minimap."));

	addOption(ToggleOption("Disable 3D Spotting")
		.addToggle(vars.bNo3DSpotting)
		.addFunction(UpdateStuff)
		.addTooltip("Stops spots showing on the screen."));

	addOption(ToggleOption("Disable Name Tags")
		.addToggle(vars.bNoNameTags)
		.addFunction(UpdateStuff)
		.addTooltip("Stops name tags showing on the screen."));

	addOption(ToggleOption("Only Squad Leader Spawn")
		.addToggle(vars.bOnlySquadLeaderSpawn)
		.addFunction(UpdateStuff)
		.addTooltip("For when you're in hardcore."));

	addOption(ToggleOption("Unlock All")
		.addToggle(vars.bAllUnlocksUnlocked)
		.addFunction(UpdateStuff)
		.addTooltip("Unlock access to all weapons while in this \nserver."));

	addOption(ToggleOption("Team Switching Allowed")
		.addToggle(vars.bTeamSwitchingAllowed)
		.addFunction(UpdateStuff)
		.addTooltip("Whether you're allowed to switch teams."));

	addOption(ToggleOption("Disable Hit Indicators")
		.addToggle(vars.bDisableHitIndicators)
		.addFunction(UpdateStuff)
		.addTooltip("Stops hit indicators showing on the screen."));
}

/*Update once when submenu is opened*/
void ServerSettingsMenu::UpdateOnce() {
	auto synced = pGame.GetSyncedBFSettings();
	if (ValidPtr(synced)) {
		vars.bAllUnlocksUnlocked = synced->bAllUnlocksUnlocked;
		vars.bHideMinimap = synced->bHideMinimap;
		vars.bHideHUD = synced->bHideHUD;
		vars.bNoMinimapSpotting = synced->bNoMinimapSpotting;
		vars.bNo3DSpotting = synced->bNo3DSpotting;
		vars.bNoNameTags = synced->bNoNameTags;
		vars.bOnlySquadLeaderSpawn = synced->bOnlySquadLeaderSpawn;
		vars.bTeamSwitchingAllowed = synced->bTeamSwitchingAllowed;
		vars.bDisableHitIndicators = synced->bDisableHitIndicators;
	}
}

/*Update while submenu is active*/
void ServerSettingsMenu::Update() {}

/*Background update*/
void ServerSettingsMenu::FeatureUpdate() {
	auto synced = pGame.GetSyncedBFSettings();
	if (ValidPtr(synced)) {
		synced->bRevertBackToKillerCamera = true;
	}
}	

/*Singleton*/
ServerSettingsMenu* _instance;
ServerSettingsMenu* ServerSettingsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ServerSettingsMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
ServerSettingsMenu::~ServerSettingsMenu() { delete _instance; }