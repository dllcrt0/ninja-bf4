#pragma once
#include "menu/handler/submenu.h"

class AimbotMenu : public Submenu {
public:
	static AimbotMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	AimbotMenu() : Submenu() {}
	~AimbotMenu();
};

namespace AimbotMenuVars
{
	struct Vars {
		bool bAimRequried;
		bool bAutoWall;
		bool bAutoAim;
		bool bAutoShoot;
		bool bUnfair;
		bool bAlwaysHeadshot;
		bool bOffscreenTargeting;
		bool bVisibilityCheck;
		bool bAimOnFriendlies;
		bool bAimOnEnemies;
		bool bAimOnVehicles;
		bool bSpoofWeaponToTarget;
		bool bDrawFOV;
		bool bLimitToFOV;

		int iSpoofWeapon;
		int iAimbotType;
		int iTargetBone;
		int iAutoShootDelay;
		int iMaxDistance;
		int iIngores;

		float fSmoothScale;
		float fFOVRadius;
	};

	extern Vars vars;
	extern ScrollStruct<BONES> TargetBone[13];
	extern ScrollStruct<int> SpoofWeapons[7];
}