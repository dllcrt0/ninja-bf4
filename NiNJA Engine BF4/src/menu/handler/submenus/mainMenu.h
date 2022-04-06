#pragma once
#include "menu/handler/submenu.h"

class MainMenu : public Submenu {
public:
	static MainMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MainMenu() : Submenu() {}
	~MainMenu();
};

namespace MainMenuVars
{
	struct Vars {
		int iPreset;

		float fLowestFPS;

		bool bShowFrameRate;
		bool bShowMapInfo;
		bool bShowPing;
		bool bShowNiNJAWatermark;
		bool bShowMenuInstructions;
		bool bDisableKillFeed;
		bool bGodmode;
		bool bForceRail;

		SoldierWeaponUnlockAsset* pOldAsset;
	};

	void ForceRail();
	extern Vars vars;
}