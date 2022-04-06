#pragma once
#include "menu/handler/submenu.h"

class VisualMenu : public Submenu {
public:
	static VisualMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VisualMenu() : Submenu() {}
	~VisualMenu();
};

namespace VisualMenuVars
{
	struct Vars {
		int iBones;
		int iSnapline;
		int iSnaplinePosition;
		int iCrosshair;
		int iBoxes;
		int iBoxType;
		int iOccupiedVehicles;
		int iHealth;
		int iDistance;
		int iChams;
		int iName;
		int iWeapon;

		bool bSupplyCrates;
		bool bExplosives;
		bool bGrenades;
		bool bPickups;
		bool bEmptyVehicles;
		bool bShowAimbotInfo;

		GColor cEnemy;
		GColor cFriendly;
		GColor cPreferred;
		GColor cWallbangable;
		GColor cWhitelisted;
		GColor cVisible;
	};

	extern Vars vars;
}