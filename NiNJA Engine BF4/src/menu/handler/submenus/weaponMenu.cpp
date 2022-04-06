#include "stdafx.h"
#include "weaponMenu.h"

using namespace WeaponMenuVars;

namespace WeaponMenuVars {
	Vars vars;

	void SetDamagePerHit() {
		pWeapon.SetDamagePerHit(vars.fDamagePerHit);
	}

	bool BulletCountReq() {
		auto soldier = pGame.GetMySoldierEntity();
		if (ValidPtr(soldier)) {
			auto wep = soldier->pSoldierWeaponsComponent;
			if (ValidPtr(wep)) {
				return wep->ActiveSlot == M_PRIMARY || wep->ActiveSlot == M_SECONDARY;
			}
		}

		return false;
	}

	void UpdateNumberOf() {
		if (BulletCountReq()) {
			GetNumberOf set;
			set.iBurst = vars.iNumOfBulletsPerBurst;
			set.iShell = vars.iNumOfBulletsPerShell;
			set.iShot = vars.iNumOfBulletsPerShot;
			pWeapon.SetNumberOf(set);
		}
	}

	void CB_Parachute() {
		if (!vars.bNoParachuteConstraints) {
			pWeapon.NoParachuteConstraints(true);
		}
	}

	void CB_InstantHit() {
		if (!vars.bInstantHit) {
			pWeapon.InstantHit(true);
		}
	}

	void CB_InstantKill() {
		if (!vars.bInstantKill) {
			pWeapon.InstantKill(true);
		}
	}

	void CB_NoBreath() {
		if (!vars.bNoBreath) {
			pWeapon.NoBreath(true);
		}
	}

	void CB_NoSway() {
		if (!vars.bNoSway) {
			pWeapon.NoSway(true);
		}
	}

	void CB_LongerBulletLife() {
		if (!vars.bLongerBulletLife) {
			pWeapon.LongerBulletLife(true);
		}
	}
}

void WeaponMenu::Init() {
	SetName("Weapon");

	addOption(ToggleOption("No Recoil")
		.addToggle(vars.bNoRecoil)
		.addTooltip("Disables all weapon recoil so that you can \nfire straight and your weapon doesn't kick \nup at all."));

	addOption(ToggleOption("No Sway")
		.addToggle(vars.bNoSway)
		.addFunction(CB_NoSway)
		.addTooltip("Disables all weapon sway so that your \nweapon is held perfectly still. Makes a big \ndifference with sniper rifles."));
	
	addOption(ToggleOption("No Breath")
		.addToggle(vars.bNoBreath)
		.addFunction(CB_NoBreath)
		.addTooltip("Disables breath on sniper rifles."));

	addOption(ToggleOption("No Spread")
		.addToggle(vars.bNoSpread)
		.addTooltip("Disables all weapon spread so that your \nbullets fire perfectly straight no matter \nwhat."));

	addOption(ToggleOption("No Parachute Constraints")
		.addToggle(vars.bNoParachuteConstraints)
		.addFunction(CB_Parachute)
		.addTooltip("Unrestrict angles for parachute aiming."));

	addOption(ToggleOption("Instant Hit")
		.addToggle(vars.bInstantHit)
		.addFunction(CB_InstantHit)
		.addTooltip("Bullets register as being instantly hit."));

	addOption(ToggleOption("Instant Kill")
		.addToggle(vars.bInstantKill)
		.addFunction(CB_InstantKill)
		.addTooltip("Increases bullet damage substantially to kill \non impact."));

	addOption(ToggleOption("Always Headshot")
		.addToggle(vars.bAlwaysHeadshot)
		.addTooltip("Always get a headshot bonus."));

	addOption(ToggleOption("Longer Bullet Life")
		.addToggle(vars.bLongerBulletLife)
		.addFunction(CB_LongerBulletLife)
		.addTooltip("Bullets don't despawn as fast."));

	addOption(NumberOption<float>(SCROLL, "Damage Per Hit", true)
		.addNumber(vars.fDamagePerHit, "%.0f", 10.0f).addMin(0.f).addMax(2000.f).setScrollSpeed(5)
		.addFunction(SetDamagePerHit)
		.addTooltip("How much damage you want the bullet to \ngive on impact."));

	addOption(ToggleOption("Self Heal Gun")
		.addToggle(vars.bSelfHealGun)
		.addTooltip("Replenish your health when you shoot."));

	addOption(ToggleOption("Team Heal Gun")
		.addToggle(vars.bTeamHealGun)
		.addTooltip("Replenish your teams health when you \nshoot."));

	addOption(BreakOption("Ammo"));

	addOption(ToggleOption("Unlimited Ammo")
		.addToggle(vars.bUnlimitedAmmo)
		.addTooltip("Your weapons ammo won't deplete."));

	addOption(ToggleOption("Auto Reload")
		.addToggle(vars.bAutoReload)
		.addTooltip("Reloads your weapon automatically when \nneeded."));

	addOption(BreakOption("Bullet Counts")
		.addRequirement(BulletCountReq));

	addOption(NumberOption<int>(SCROLL, "Bullets Per Shell")
		.addNumber(vars.iNumOfBulletsPerShell, "%i", 1).addMin(1).addMax(50)
		.addFunction(UpdateNumberOf)
		.addRequirement(BulletCountReq)
		.addTooltip("How many bullets are spawned in each \nshot (won't deplete ammo count)."));

	addOption(NumberOption<int>(SCROLL, "Bullets Per Shot")
		.addNumber(vars.iNumOfBulletsPerShot, "%i", 1).addMin(1).addMax(50)
		.addFunction(UpdateNumberOf)
		.addRequirement(BulletCountReq)
		.addTooltip("How many bullets your weapon shoots per \nsingle shot (will deplete ammo count)."));

	addOption(NumberOption<int>(SCROLL, "Bullets Per Burst")
		.addNumber(vars.iNumOfBulletsPerBurst, "%i", 1).addMin(1).addMax(50)
		.addFunction(UpdateNumberOf)
		.addRequirement(BulletCountReq)
		.addTooltip("How many bullets your weapon shoots per \nburst shot (will deplete ammo count)."));
}

/*Update once when submenu is opened*/
void WeaponMenu::UpdateOnce() {
	auto soldier = pGame.GetMySoldierEntity();
	if (ValidPtr(soldier)) {
		auto wep = soldier->pSoldierWeaponsComponent;
		if (ValidPtr(wep)) {
			if (wep->ActiveSlot == M_PRIMARY || wep->ActiveSlot == M_SECONDARY) {
				auto num = pWeapon.GetNumberOfValues();
				vars.iNumOfBulletsPerShell = num.iShell;
				vars.iNumOfBulletsPerShot = num.iShot;
				vars.iNumOfBulletsPerBurst = num.iBurst;
				vars.fDamagePerHit = pWeapon.GetDamagePerHit();
			}
		}
	}
}

/*Update while submenu is active*/
void WeaponMenu::Update() {
	static eWeaponSlot currentWeaponIndex;

	auto soldier = pGame.GetMySoldierEntity();
	if (ValidPtr(soldier)) {
		auto wep = soldier->pSoldierWeaponsComponent;
		if (ValidPtr(wep)) {
			if (wep->ActiveSlot == M_PRIMARY || wep->ActiveSlot == M_SECONDARY) {
				if (currentWeaponIndex != wep->ActiveSlot) {
					currentWeaponIndex = wep->ActiveSlot;
					
					auto num = pWeapon.GetNumberOfValues();
					vars.iNumOfBulletsPerShell = num.iShell;
					vars.iNumOfBulletsPerShot = num.iShot;
					vars.iNumOfBulletsPerBurst = num.iBurst;
					vars.fDamagePerHit = pWeapon.GetDamagePerHit();
				}
			}
		}
	}
}

/*Background update*/
void WeaponMenu::FeatureUpdate() {
	if (vars.bNoSway) {
		pWeapon.NoSway();
	}

	if (vars.bFastReload) {
		pWeapon.FastReload();
	}

	if (vars.bNoBreath) {
		pWeapon.NoBreath();
	}

	if (vars.bNoBulletDrop) {
		pWeapon.NoBulletDrop();
	}

	if (vars.bInstantHit) {
		pWeapon.InstantHit();
	}

	if (vars.bInstantKill) {
		pWeapon.InstantKill();
	}

	if (vars.bLongerBulletLife) {
		pWeapon.LongerBulletLife();
	}

	if (vars.bNoParachuteConstraints) {
		pWeapon.NoParachuteConstraints();
	}
}

/*Singleton*/
WeaponMenu* _instance;
WeaponMenu* WeaponMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WeaponMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
WeaponMenu::~WeaponMenu() { delete _instance; }