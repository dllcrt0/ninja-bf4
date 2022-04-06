#pragma once
#include "menu/handler/submenu.h"

class ServerSettingsMenu : public Submenu {
public:
	static ServerSettingsMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ServerSettingsMenu() : Submenu() {}
	~ServerSettingsMenu();
};

namespace ServerSettingsMenuVars
{
	struct Vars {
		bool bAllUnlocksUnlocked;
		bool bHideMinimap;
		bool bHideHUD;
		bool bNoMinimapSpotting;
		bool bNo3DSpotting;
		bool bNoNameTags;
		bool bOnlySquadLeaderSpawn;
		bool bTeamSwitchingAllowed;
		bool bRevertBackToKillerCamera;
		bool bDisableHitIndicators;
		bool bDisableVehicleCommanderActions;
	};
	void UpdateStuff();
	extern Vars vars;
}