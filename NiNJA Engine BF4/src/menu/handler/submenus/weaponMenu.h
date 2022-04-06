#pragma once
#include "menu/handler/submenu.h"

class WeaponMenu : public Submenu {
public:
	static WeaponMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WeaponMenu() : Submenu() {}
	~WeaponMenu();
};

namespace WeaponMenuVars
{
	struct Vars {
		bool bNoRecoil;
		bool bNoSway;
		bool bNoBreath;
		bool bNoParachuteConstraints;
		bool bNoSpread;
		bool bNoBulletDrop;
		bool bInstantHit;
		bool bInstantKill;
		bool bLongerBulletLife;
		bool bAlwaysHeadshot;
		bool bUnlimitedAmmo;
		bool bFastReload;
		bool bAutoReload;
		bool bSelfHealGun;
		bool bTeamHealGun;

		int iNumOfBulletsPerShell;
		int iNumOfBulletsPerShot;
		int iNumOfBulletsPerBurst;

		float fDamagePerHit;
	};

	extern Vars vars;
}