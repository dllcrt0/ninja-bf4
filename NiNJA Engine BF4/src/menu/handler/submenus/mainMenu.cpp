#include "stdafx.h"
#include "mainMenu.h"
#include "main/mainChatCommands.h"
#include "settings/soundsMenu.h"
#include "main/spotMenu.h"
#include "main/ordersMenu.h"
#include "main/warningMenu.h"
#include "main/movementMenu.h"
#include "main/developerMenu.h"
#include "main/loadoutEditorMenu.h"
#include "main/serverSettingsMenu.h"

using namespace MainMenuVars;

namespace MainMenuVars {
	Vars vars;

	ScrollStruct<int> Presets[] = {
		// "Rage Railgun", 2,
		"Rage Snap Aim", 0,
		"Rage Silent Aim", 1,
		"Legit", 3,
		"Default", 4
	};

	void SetPreset() {
		switch (Presets[vars.iPreset].Result) {
			case 0: {
				AimbotMenuVars::vars.iAimbotType = 1;
				AimbotMenuVars::vars.iTargetBone = 7;
				AimbotMenuVars::vars.iMaxDistance = 300;
				AimbotMenuVars::vars.bAimRequried = true;
				AimbotMenuVars::vars.bLimitToFOV = false;
				AimbotMenuVars::vars.bDrawFOV = false;
				AimbotMenuVars::vars.fFOVRadius = 250.f;
				AimbotMenuVars::vars.iIngores = 0;
				AimbotMenuVars::vars.bAutoWall = true;
				AimbotMenuVars::vars.bAutoAim = false;
				AimbotMenuVars::vars.bAutoShoot = true;
				AimbotMenuVars::vars.iAutoShootDelay = 100;
				AimbotMenuVars::vars.bOffscreenTargeting = true;
				AimbotMenuVars::vars.bUnfair = true;
				AimbotMenuVars::vars.bVisibilityCheck = true;
				AimbotMenuVars::vars.bAimOnFriendlies = false;
				AimbotMenuVars::vars.bAimOnEnemies = true;
				AimbotMenuVars::vars.bAimOnVehicles = false;
				AimbotMenuVars::vars.bSpoofWeaponToTarget = false;
				AimbotMenuVars::vars.iSpoofWeapon = 0;

				vars.bDisableKillFeed = false;
				vars.bShowNiNJAWatermark = true;
				vars.bShowMenuInstructions = true;
				vars.bShowFrameRate = false;
				vars.bShowMapInfo = false;

				VisualMenuVars::vars.iBones = 3;
				VisualMenuVars::vars.iBoxes = 3;
				VisualMenuVars::vars.iBoxType = 2;
				VisualMenuVars::vars.iCrosshair = 2;
				VisualMenuVars::vars.iDistance = 0;
				VisualMenuVars::vars.iName = 2;
				VisualMenuVars::vars.iWeapon = 0;
				VisualMenuVars::vars.iChams = 0;
				VisualMenuVars::vars.iHealth = 0;
				VisualMenuVars::vars.iOccupiedVehicles = 0;
				VisualMenuVars::vars.iSnapline = 0;
				VisualMenuVars::vars.iSnaplinePosition = 1;
				VisualMenuVars::vars.bSupplyCrates = false;
				VisualMenuVars::vars.bExplosives = false;
				VisualMenuVars::vars.bGrenades = false;
				VisualMenuVars::vars.bPickups = false;
				VisualMenuVars::vars.bEmptyVehicles = false;
				VisualMenuVars::vars.bShowAimbotInfo = true;

				WeaponMenuVars::vars.bNoRecoil = true;
				WeaponMenuVars::vars.bNoSway = true;
				WeaponMenuVars::vars.bNoBreath = true;
				WeaponMenuVars::vars.bNoSpread = true;
				WeaponMenuVars::vars.bNoParachuteConstraints = true;
				WeaponMenuVars::vars.bNoBulletDrop = false;
				WeaponMenuVars::vars.bInstantHit = true;
				WeaponMenuVars::vars.bInstantKill = true;
				WeaponMenuVars::vars.bAlwaysHeadshot = false;
				WeaponMenuVars::vars.bLongerBulletLife = true;
				WeaponMenuVars::vars.bSelfHealGun = true;
				WeaponMenuVars::vars.bTeamHealGun = false;
				WeaponMenuVars::vars.bUnlimitedAmmo = true;
				WeaponMenuVars::vars.bAutoReload = false;

				MainChatCommandsMenuVars::vars.iChannelType = 0;
				MainChatCommandsMenuVars::vars.iMessageType = 0;
				MainChatCommandsMenuVars::vars.bCustomKillFeedDeath = false;
				MainChatCommandsMenuVars::vars.bCustomKillFeedKill = false;

				MovementMenuVars::vars.bTeleportAbove = true;
				MovementMenuVars::vars.bTeleportToCrosshair = true;
				MovementMenuVars::vars.iTeleportAboveHeight = 200;

				OrdersMenuVars::vars.bAlwaysOrder = false;
				OrdersMenuVars::vars.iOrder = 0;

				WarningMenuVars::vars.iProximity = 0;
				WarningMenuVars::vars.iAimAngle = 0;
				WarningMenuVars::vars.iVisible = 0;

				SoundsMenuVars::vars.bKillingSpree = true;
				SoundsMenuVars::vars.bKillingStreak = true;
				SoundsMenuVars::vars.iOnKill = 0;
				SoundsMenuVars::vars.iOnDeath = 0;

				VisualMenu::GetInstance()->FeatureUpdate();
				AimbotMenu::GetInstance()->FeatureUpdate();
				SettingsMenu::GetInstance()->UpdateOnce();
				SoundsMenu::GetInstance()->UpdateOnce();
				WeaponMenu::GetInstance()->FeatureUpdate();
				WeaponMenu::GetInstance()->UpdateOnce();
				MainChatCommandsMenu::GetInstance()->FeatureUpdate();
				ServerSettingsMenu::GetInstance()->UpdateOnce();
				ServerSettingsMenuVars::UpdateStuff();
				break;
			}
			case 1: {
				AimbotMenuVars::vars.iAimbotType = 2;
				AimbotMenuVars::vars.iTargetBone = 7;
				AimbotMenuVars::vars.iMaxDistance = 300;
				AimbotMenuVars::vars.bAimRequried = false;
				AimbotMenuVars::vars.bLimitToFOV = false;
				AimbotMenuVars::vars.bDrawFOV = false;
				AimbotMenuVars::vars.fFOVRadius = 250.f;
				AimbotMenuVars::vars.iIngores = 0;
				AimbotMenuVars::vars.bAutoWall = true;
				AimbotMenuVars::vars.bAutoAim = false;
				AimbotMenuVars::vars.bAutoShoot = true;
				AimbotMenuVars::vars.iAutoShootDelay = 10;
				AimbotMenuVars::vars.bOffscreenTargeting = true;
				AimbotMenuVars::vars.bUnfair = true;
				AimbotMenuVars::vars.bVisibilityCheck = true;
				AimbotMenuVars::vars.bAimOnFriendlies = false;
				AimbotMenuVars::vars.bAimOnEnemies = true;
				AimbotMenuVars::vars.bAimOnVehicles = false;
				AimbotMenuVars::vars.bSpoofWeaponToTarget = false;
				AimbotMenuVars::vars.iSpoofWeapon = 0;

				vars.bDisableKillFeed = false;
				vars.bShowNiNJAWatermark = true;
				vars.bShowMenuInstructions = true;
				vars.bShowFrameRate = false;
				vars.bShowMapInfo = false;

				VisualMenuVars::vars.iBones = 3;
				VisualMenuVars::vars.iBoxes = 3;
				VisualMenuVars::vars.iBoxType = 2;
				VisualMenuVars::vars.iCrosshair = 2;
				VisualMenuVars::vars.iDistance = 0;
				VisualMenuVars::vars.iName = 2;
				VisualMenuVars::vars.iWeapon = 0;
				VisualMenuVars::vars.iChams = 0;
				VisualMenuVars::vars.iHealth = 0;
				VisualMenuVars::vars.iOccupiedVehicles = 0;
				VisualMenuVars::vars.iSnapline = 2;
				VisualMenuVars::vars.iSnaplinePosition = 1;
				VisualMenuVars::vars.bSupplyCrates = false;
				VisualMenuVars::vars.bExplosives = false;
				VisualMenuVars::vars.bGrenades = false;
				VisualMenuVars::vars.bPickups = false;
				VisualMenuVars::vars.bEmptyVehicles = false;
				VisualMenuVars::vars.bShowAimbotInfo = true;

				WeaponMenuVars::vars.bNoRecoil = true;
				WeaponMenuVars::vars.bNoSway = true;
				WeaponMenuVars::vars.bNoBreath = true;
				WeaponMenuVars::vars.bNoSpread = true;
				WeaponMenuVars::vars.bNoParachuteConstraints = true;
				WeaponMenuVars::vars.bNoBulletDrop = false;
				WeaponMenuVars::vars.bInstantHit = true;
				WeaponMenuVars::vars.bInstantKill = true;
				WeaponMenuVars::vars.bAlwaysHeadshot = false;
				WeaponMenuVars::vars.bLongerBulletLife = true;
				WeaponMenuVars::vars.bSelfHealGun = true;
				WeaponMenuVars::vars.bTeamHealGun = false;
				WeaponMenuVars::vars.bUnlimitedAmmo = true;
				WeaponMenuVars::vars.bAutoReload = false;

				MainChatCommandsMenuVars::vars.iChannelType = 0;
				MainChatCommandsMenuVars::vars.iMessageType = 0;
				MainChatCommandsMenuVars::vars.bCustomKillFeedDeath = false;
				MainChatCommandsMenuVars::vars.bCustomKillFeedKill = false;

				MovementMenuVars::vars.bTeleportAbove = true;
				MovementMenuVars::vars.bTeleportToCrosshair = true;
				MovementMenuVars::vars.iTeleportAboveHeight = 200;

				OrdersMenuVars::vars.bAlwaysOrder = false;
				OrdersMenuVars::vars.iOrder = 0;

				WarningMenuVars::vars.iProximity = 0;
				WarningMenuVars::vars.iAimAngle = 0;
				WarningMenuVars::vars.iVisible = 0;

				SoundsMenuVars::vars.bKillingSpree = true;
				SoundsMenuVars::vars.bKillingStreak = true;
				SoundsMenuVars::vars.iOnKill = 0;
				SoundsMenuVars::vars.iOnDeath = 0;

				VisualMenu::GetInstance()->FeatureUpdate();
				AimbotMenu::GetInstance()->FeatureUpdate();
				SettingsMenu::GetInstance()->UpdateOnce();
				WeaponMenu::GetInstance()->FeatureUpdate();
				WeaponMenu::GetInstance()->UpdateOnce();
				SoundsMenu::GetInstance()->UpdateOnce();
				MovementMenu::GetInstance()->FeatureUpdate();
				MainChatCommandsMenu::GetInstance()->FeatureUpdate();
				ServerSettingsMenu::GetInstance()->UpdateOnce();
				ServerSettingsMenuVars::UpdateStuff();
				break;
			}
			case 2: {
				AimbotMenuVars::vars.iAimbotType = 2;
				AimbotMenuVars::vars.iTargetBone = 1;
				AimbotMenuVars::vars.iMaxDistance = 2000;
				AimbotMenuVars::vars.bAimRequried = false;
				AimbotMenuVars::vars.bLimitToFOV = false;
				AimbotMenuVars::vars.bDrawFOV = false;
				AimbotMenuVars::vars.fFOVRadius = 250.f;
				AimbotMenuVars::vars.iIngores = 0;
				AimbotMenuVars::vars.bAutoWall = true;
				AimbotMenuVars::vars.bAutoAim = false;
				AimbotMenuVars::vars.bAutoShoot = true;
				AimbotMenuVars::vars.iAutoShootDelay = 0;
				AimbotMenuVars::vars.bOffscreenTargeting = true;
				AimbotMenuVars::vars.bUnfair = true;
				AimbotMenuVars::vars.bVisibilityCheck = false;
				AimbotMenuVars::vars.bAimOnFriendlies = false;
				AimbotMenuVars::vars.bAimOnEnemies = true;
				AimbotMenuVars::vars.bAimOnVehicles = false;
				AimbotMenuVars::vars.bSpoofWeaponToTarget = false;
				AimbotMenuVars::vars.iSpoofWeapon = 0;

				vars.bDisableKillFeed = false;
				vars.bShowNiNJAWatermark = true;
				vars.bShowMenuInstructions = true;
				vars.bShowFrameRate = false;
				vars.bShowMapInfo = false;

				VisualMenuVars::vars.iBones = 3;
				VisualMenuVars::vars.iBoxes = 3;
				VisualMenuVars::vars.iBoxType = 2;
				VisualMenuVars::vars.iCrosshair = 2;
				VisualMenuVars::vars.iDistance = 0;
				VisualMenuVars::vars.iName = 2;
				VisualMenuVars::vars.iWeapon = 0;
				VisualMenuVars::vars.iChams = 0;
				VisualMenuVars::vars.iHealth = 0;
				VisualMenuVars::vars.iOccupiedVehicles = 0;
				VisualMenuVars::vars.iSnapline = 2;
				VisualMenuVars::vars.iSnaplinePosition = 1;
				VisualMenuVars::vars.bSupplyCrates = false;
				VisualMenuVars::vars.bExplosives = false;
				VisualMenuVars::vars.bGrenades = false;
				VisualMenuVars::vars.bPickups = false;
				VisualMenuVars::vars.bEmptyVehicles = false;
				VisualMenuVars::vars.bShowAimbotInfo = true;

				WeaponMenuVars::vars.bNoRecoil = true;
				WeaponMenuVars::vars.bNoSway = true;
				WeaponMenuVars::vars.bNoBreath = true;
				WeaponMenuVars::vars.bNoSpread = true;
				WeaponMenuVars::vars.bNoParachuteConstraints = true;
				WeaponMenuVars::vars.bNoBulletDrop = false;
				WeaponMenuVars::vars.bInstantHit = true;
				WeaponMenuVars::vars.bInstantKill = true;
				WeaponMenuVars::vars.bAlwaysHeadshot = false;
				WeaponMenuVars::vars.bLongerBulletLife = true;
				WeaponMenuVars::vars.bSelfHealGun = true;
				WeaponMenuVars::vars.bTeamHealGun = false;
				WeaponMenuVars::vars.bUnlimitedAmmo = true;
				WeaponMenuVars::vars.bAutoReload = false;

				MainChatCommandsMenuVars::vars.iChannelType = 0;
				MainChatCommandsMenuVars::vars.iMessageType = 0;
				MainChatCommandsMenuVars::vars.bCustomKillFeedDeath = false;
				MainChatCommandsMenuVars::vars.bCustomKillFeedKill = false;

				MovementMenuVars::vars.bTeleportAbove = true;
				MovementMenuVars::vars.bTeleportToCrosshair = true;
				MovementMenuVars::vars.iTeleportAboveHeight = 200;

				OrdersMenuVars::vars.bAlwaysOrder = false;
				OrdersMenuVars::vars.iOrder = 0;

				WarningMenuVars::vars.iProximity = 0;
				WarningMenuVars::vars.iAimAngle = 0;
				WarningMenuVars::vars.iVisible = 0;

				SoundsMenuVars::vars.bKillingSpree = true;
				SoundsMenuVars::vars.bKillingStreak = true;
				SoundsMenuVars::vars.iOnKill = 0;
				SoundsMenuVars::vars.iOnDeath = 0;

				VisualMenu::GetInstance()->FeatureUpdate();
				AimbotMenu::GetInstance()->FeatureUpdate();
				SettingsMenu::GetInstance()->UpdateOnce();
				WeaponMenu::GetInstance()->FeatureUpdate();
				WeaponMenu::GetInstance()->UpdateOnce();
				SoundsMenu::GetInstance()->UpdateOnce();
				MovementMenu::GetInstance()->FeatureUpdate();
				MainChatCommandsMenu::GetInstance()->FeatureUpdate();
				ServerSettingsMenu::GetInstance()->UpdateOnce();
				ServerSettingsMenuVars::UpdateStuff();

				LoadoutEditorMenu::GetInstance()->UpdateOnce();
				vars.bForceRail = true;
				ForceRail();
				break;
			}
			case 3: { // legit
				AimbotMenuVars::vars.iAimbotType = 3;
				AimbotMenuVars::vars.iTargetBone = 1;
				AimbotMenuVars::vars.iMaxDistance = 100;
				AimbotMenuVars::vars.bAimRequried = true;
				AimbotMenuVars::vars.bLimitToFOV = true;
				AimbotMenuVars::vars.bDrawFOV = true;
				AimbotMenuVars::vars.fFOVRadius = 200.f;
				AimbotMenuVars::vars.iIngores = 3;
				AimbotMenuVars::vars.bAutoWall = false;
				AimbotMenuVars::vars.bAutoAim = false;
				AimbotMenuVars::vars.bAutoShoot = false;
				AimbotMenuVars::vars.iAutoShootDelay = 100;
				AimbotMenuVars::vars.bOffscreenTargeting = false;
				AimbotMenuVars::vars.bUnfair = true;
				AimbotMenuVars::vars.bVisibilityCheck = true;
				AimbotMenuVars::vars.bAimOnFriendlies = false;
				AimbotMenuVars::vars.bAimOnEnemies = true;
				AimbotMenuVars::vars.bAimOnVehicles = false;
				AimbotMenuVars::vars.bSpoofWeaponToTarget = false;
				AimbotMenuVars::vars.iSpoofWeapon = 0;

				vars.bDisableKillFeed = false;
				vars.bShowNiNJAWatermark = true;
				vars.bShowMenuInstructions = true;
				vars.bShowFrameRate = false;
				vars.bShowMapInfo = false;

				VisualMenuVars::vars.iBones = 3;
				VisualMenuVars::vars.iBoxes = 3;
				VisualMenuVars::vars.iBoxType = 2;
				VisualMenuVars::vars.iCrosshair = 2;
				VisualMenuVars::vars.iDistance = 0;
				VisualMenuVars::vars.iName = 2;
				VisualMenuVars::vars.iWeapon = 0;
				VisualMenuVars::vars.iChams = 0;
				VisualMenuVars::vars.iHealth = 0;
				VisualMenuVars::vars.iOccupiedVehicles = 0;
				VisualMenuVars::vars.iSnapline = 0;
				VisualMenuVars::vars.iSnaplinePosition = 1;
				VisualMenuVars::vars.bSupplyCrates = false;
				VisualMenuVars::vars.bExplosives = false;
				VisualMenuVars::vars.bGrenades = false;
				VisualMenuVars::vars.bPickups = false;
				VisualMenuVars::vars.bEmptyVehicles = false;
				VisualMenuVars::vars.bShowAimbotInfo = true;

				WeaponMenuVars::vars.bNoRecoil = true;
				WeaponMenuVars::vars.bNoSway = true;
				WeaponMenuVars::vars.bNoBreath = true;
				WeaponMenuVars::vars.bNoSpread = true;
				WeaponMenuVars::vars.bNoParachuteConstraints = true;
				WeaponMenuVars::vars.bNoBulletDrop = false;
				WeaponMenuVars::vars.bInstantHit = true;
				WeaponMenuVars::vars.bInstantKill = false;
				WeaponMenuVars::vars.bAlwaysHeadshot = false;
				WeaponMenuVars::vars.bLongerBulletLife = true;
				WeaponMenuVars::vars.bSelfHealGun = true;
				WeaponMenuVars::vars.bTeamHealGun = false;
				WeaponMenuVars::vars.bUnlimitedAmmo = true;
				WeaponMenuVars::vars.bAutoReload = false;

				MainChatCommandsMenuVars::vars.iChannelType = 0;
				MainChatCommandsMenuVars::vars.iMessageType = 0;
				MainChatCommandsMenuVars::vars.bCustomKillFeedDeath = false;
				MainChatCommandsMenuVars::vars.bCustomKillFeedKill = false;

				MovementMenuVars::vars.bTeleportAbove = false;
				MovementMenuVars::vars.bTeleportToCrosshair = false;
				MovementMenuVars::vars.iTeleportAboveHeight = 200;

				OrdersMenuVars::vars.bAlwaysOrder = false;
				OrdersMenuVars::vars.iOrder = 0;

				WarningMenuVars::vars.iProximity = 0;
				WarningMenuVars::vars.iAimAngle = 0;
				WarningMenuVars::vars.iVisible = 0;

				SoundsMenuVars::vars.bKillingSpree = true;
				SoundsMenuVars::vars.bKillingStreak = true;
				SoundsMenuVars::vars.iOnKill = 0;
				SoundsMenuVars::vars.iOnDeath = 0;

				VisualMenu::GetInstance()->FeatureUpdate();
				AimbotMenu::GetInstance()->FeatureUpdate();
				SettingsMenu::GetInstance()->UpdateOnce();
				SoundsMenu::GetInstance()->UpdateOnce();
				WeaponMenu::GetInstance()->FeatureUpdate();
				WeaponMenu::GetInstance()->UpdateOnce();
				MainChatCommandsMenu::GetInstance()->FeatureUpdate();
				ServerSettingsMenu::GetInstance()->UpdateOnce();
				ServerSettingsMenuVars::UpdateStuff();
				break;
			}
			case 4: { // default
				AimbotMenuVars::vars.iAimbotType = 3;
				AimbotMenuVars::vars.iTargetBone = 1;
				AimbotMenuVars::vars.fSmoothScale = 1.f;
				AimbotMenuVars::vars.iMaxDistance = 300;
				AimbotMenuVars::vars.bAimRequried = false;
				AimbotMenuVars::vars.bLimitToFOV = false;
				AimbotMenuVars::vars.bDrawFOV = false;
				AimbotMenuVars::vars.fFOVRadius = 250.f;
				AimbotMenuVars::vars.iIngores = 0;
				AimbotMenuVars::vars.bAutoWall = false;
				AimbotMenuVars::vars.bAutoAim = false;
				AimbotMenuVars::vars.bAutoShoot = false;
				AimbotMenuVars::vars.iAutoShootDelay = 100;
				AimbotMenuVars::vars.bOffscreenTargeting = false;
				AimbotMenuVars::vars.bUnfair = false;
				AimbotMenuVars::vars.bVisibilityCheck = false;
				AimbotMenuVars::vars.bAimOnFriendlies = false;
				AimbotMenuVars::vars.bAimOnEnemies = false;
				AimbotMenuVars::vars.bAimOnVehicles = false;
				AimbotMenuVars::vars.bSpoofWeaponToTarget = false;
				AimbotMenuVars::vars.iSpoofWeapon = 0;

				vars.bDisableKillFeed = false;
				vars.bShowNiNJAWatermark = true;
				vars.bShowMenuInstructions = true;
				vars.bShowFrameRate = false;
				vars.bShowMapInfo = false;

				VisualMenuVars::vars.iBones = 0;
				VisualMenuVars::vars.iBoxes = 0;
				VisualMenuVars::vars.iBoxType = 0;
				VisualMenuVars::vars.iCrosshair = 0;
				VisualMenuVars::vars.iDistance = 0;
				VisualMenuVars::vars.iName = 0;
				VisualMenuVars::vars.iWeapon = 0;
				VisualMenuVars::vars.iChams = 0;
				VisualMenuVars::vars.iHealth = 0;
				VisualMenuVars::vars.iOccupiedVehicles = 0;
				VisualMenuVars::vars.iSnapline = 0;
				VisualMenuVars::vars.iSnaplinePosition = 0;
				VisualMenuVars::vars.bSupplyCrates = false;
				VisualMenuVars::vars.bExplosives = false;
				VisualMenuVars::vars.bGrenades = false;
				VisualMenuVars::vars.bPickups = false;
				VisualMenuVars::vars.bEmptyVehicles = false;
				VisualMenuVars::vars.bShowAimbotInfo = false;

				WeaponMenuVars::vars.bNoRecoil = false;
				WeaponMenuVars::vars.bNoSway = false;
				WeaponMenuVars::vars.bNoBreath = false;
				WeaponMenuVars::vars.bNoSpread = false;
				WeaponMenuVars::vars.bNoParachuteConstraints = false;
				WeaponMenuVars::vars.bNoBulletDrop = false;
				WeaponMenuVars::vars.bInstantHit = false;
				WeaponMenuVars::vars.bInstantKill = false;
				WeaponMenuVars::vars.bAlwaysHeadshot = false;
				WeaponMenuVars::vars.bLongerBulletLife = false;
				WeaponMenuVars::vars.bSelfHealGun = false;
				WeaponMenuVars::vars.bTeamHealGun = false;
				WeaponMenuVars::vars.bUnlimitedAmmo = false;
				WeaponMenuVars::vars.bAutoReload = false;

				MainChatCommandsMenuVars::vars.iChannelType = 0;
				MainChatCommandsMenuVars::vars.iMessageType = 0;
				MainChatCommandsMenuVars::vars.bCustomKillFeedDeath = false;
				MainChatCommandsMenuVars::vars.bCustomKillFeedKill = false;

				MovementMenuVars::vars.bTeleportAbove = false;
				MovementMenuVars::vars.bTeleportToCrosshair = false;
				MovementMenuVars::vars.iTeleportAboveHeight = 200;

				OrdersMenuVars::vars.bAlwaysOrder = false;
				OrdersMenuVars::vars.iOrder = 0;

				WarningMenuVars::vars.iProximity = 0;
				WarningMenuVars::vars.iAimAngle = 0;
				WarningMenuVars::vars.iVisible = 0;

				SoundsMenuVars::vars.bKillingSpree = true;
				SoundsMenuVars::vars.bKillingStreak = true;
				SoundsMenuVars::vars.iOnKill = 0;
				SoundsMenuVars::vars.iOnDeath = 0;

				VisualMenu::GetInstance()->FeatureUpdate();
				AimbotMenu::GetInstance()->FeatureUpdate();
				SettingsMenu::GetInstance()->UpdateOnce();
				SoundsMenu::GetInstance()->UpdateOnce();
				WeaponMenu::GetInstance()->FeatureUpdate();
				WeaponMenu::GetInstance()->UpdateOnce();
				MainChatCommandsMenu::GetInstance()->FeatureUpdate();
				ServerSettingsMenu::GetInstance()->UpdateOnce();
				ServerSettingsMenuVars::UpdateStuff();
				break;
			}
		}
	}

	void ResetFPS() {
		vars.fLowestFPS = 0.0f;
	}

	void CB_Unload() {
		NiNJA.bTerminateProcess = true;
	}

	string SplitToLastWord(const char* pStr) {
		string str(pStr);
		size_t found = str.find_last_of("/\\");

		if (found) {
			return str.substr(found + 1);
		}

		return pStr;
	}

	const char* GetReadableLevelName() {
		if (ValidPtr(pGame.GetClientGameContext())) {
			if (ValidPtr(pGame.GetClientGameContext()->pOnlineManager)) {
				if (ValidPtr(pGame.GetClientGameContext()->pOnlineManager->pClient)) {
					if (ValidPtr(pGame.GetClientGameContext()->pOnlineManager->pClient->pClientLevel)) {
						if (ValidPtr(pGame.GetClientGameContext()->pOnlineManager->pClient->pClientLevel->pLevelSetup)) {
							auto hash = pUtils.Joaat(SplitToLastWord(pGame.GetClientGameContext()->pOnlineManager->pClient->pClientLevel->pLevelName).c_str());
							switch (hash) {
								case 0x6b38512: return "Lumphini Garden";
								case 0x36dcdf66: return "Caspian Border 2014";
								case 0x4f97fb69: return "Dragon Valley 2015";
								case 0xf1417a4d: return "Lancang Dam";
								case 0x1873d6f9: return "Propaganda";
								case 0xd77004a7: return "Siege of Shanghai";
								case 0xb40dc524: return "Silk Road";
								case 0x57ddf91d: return "Sunken Dragon";
								case 0xf2dd5374: return "Community Map";
								case 0x4056d132: return "Operation Mortar";
								case 0xdd0a8a9b: return "Nansha Strike";
								case 0x2ee5036c: return "Pearl Market";
								case 0xeccae2e9: return "Hainan Resort";
								case 0xd0de998: return "Golmud Railway";
								case 0xae6d10c9: return "Operation Whiteout";
								case 0xb075382f: return "Zavod 311";
								case 0x398bffdd: return "Dawnbreaker";
								case 0x941a853e: return "Guilin Peaks";
								case 0x9054d4ef: return "Zavod - Graveyard Shift";
								case 0xea25c64c: return "Paracel Storm";
								case 0xa59ca842: return "Dragon Pass";
								case 0x7adb2bfe: return "Hammerhead";
								case 0xfc0deba0: return "Operation Metro 2014";
								case 0xee352cf0: return "Lost Islands";
								case 0xc5d7e8b8: return "Altai Range";
								case 0x9aaa96b4: return "Operation Firestorm 2014";
								case 0x15ae8eb: return "Giants of Karelia";
								case 0xc3af57f9: return "Wavebreaker";
								case 0x84a09fb9: return "Operation Locker";
								case 0x34692f14: return "Rogue Transmission";
								case 0xc6ee4410: return "Gulf of Oman 2014";
								case 0x5225166d: return "Hangar 21";
								case 0xc26770b9: return "Flood Zone";
							}
						}
					}
				}
			}
		}

		return "Unknown";
	}

	const char* GetReadableGameType() {
		if (ValidPtr(pGame.GetClientGameContext())) {
			if (ValidPtr(pGame.GetClientGameContext()->pOnlineManager)) {
				if (ValidPtr(pGame.GetClientGameContext()->pOnlineManager->pClient)) {
					if (ValidPtr(pGame.GetClientGameContext()->pOnlineManager->pClient->pClientLevel)) {
						if (ValidPtr(pGame.GetClientGameContext()->pOnlineManager->pClient->pClientLevel->pLevelSetup)) {
							auto hash = pUtils.Joaat(pGame.GetClientGameContext()->pOnlineManager->pClient->pClientLevel->pLevelSetup->pGameType);

							switch (hash) {
								case 0x49d75453: return "Conquest Large"; // ConquestLarge0
								case 0x9b7a2c62: return "Conquest Small"; // ConquestSmall0
								case 0x36657b42: return "Rush"; // RushLarge0
								case 0xd7794724: return "Squad Deathmatch"; // SquadDeathMatch0
								case 0x57b5297d: return "Team Deathmatch"; // TeamDeathMatch0
								case 0xd8d99758: return "Domination"; // Domination0
								case 0xee30604e: return "Air Superiority"; // AirSuperiority0
								case 0xe55554c7: return "Capture The Flag"; // CaptureTheFlag0
								case 0xe80f2c70: return "Defuse"; // Elimination0
								case 0xbc1e8e64: return "Obliteration"; // Obliteration
								case 0x6004b22e: return "Carrier Assault Large"; // CarrierAssaultLarge0
								case 0x7dba2323: return "Carrier Assault Small"; // CarrierAssaultSmall0
								case 0xcacb8b0c: return "Chain Link"; // Chainlink0
								case 0x8baa6b2b: return "Squad Obliteration"; // SquadObliteration0
								case 0x955c235c: return "Gun Master"; // GunMaster0
								case 0x8b490f36: return "Gun Master v2"; // GunMaster1
								case 0xe9946b5a: return "Squad Deathmatch v2"; // SquadDeathMatch1
								case 0x486e8af0: return "Team Deathmatch v2"; // TeamDeathMatch1
							}
						}
					}
				}
			}
		}

		return "Unknown";
	}

	void ForceRail() {
		if (vars.bForceRail) {
			if (!ValidPtr(vars.pOldAsset)) {
				vars.pOldAsset = (SoldierWeaponUnlockAsset*)*(DWORD*)((DWORD)pGame.GetLocalPlayer() + 0x1218);
			}
		} else {
			if (ValidPtr(vars.pOldAsset) && vars.pOldAsset->pName) {
				LoadoutEditorMenu::GetInstance()->UpdateOnce();
				LoadoutEditorMenuVars::ChangeLoadoutItem(vars.pOldAsset->pName);
			}
		}
	}
}

void MainMenu::Init() {
	SetName("Main");

	addOption(ScrollOption<int>(SCROLLSELECT, "Preset").canLoop()
		.addScroll(vars.iPreset, 0, NUMOF(Presets) - 1, Presets)
		.addFunction(SetPreset)
		.addTooltip("Welcome to NiNJA Engine! To get started \nwith the best settings for you, choose a \npreset that best describes your play style!"));

	/*addOption(ToggleOption("Hack vs Hack (Force Railgun)")
		.addToggle(vars.bForceRail)
		.addFunction(ForceRail)
		.addTooltip("Don't want to reset all your settings with a \npreset? Enable this to shoot through \nmultiple walls across the map. Trust us, it's \nfucking OP."));*/

	addOption(SubmenuOption("Movement")
		.addSubmenu<MovementMenu>()
		.addTooltip("Options based on modifying your movement."));

	addOption(SubmenuOption("Chat Messages")
		.addSubmenu<MainChatCommandsMenu>()
		.addTooltip("Sends a message in the chat, either one of \nour presets or your own inputted message."));

	addOption(SubmenuOption("Warnings")
		.addSubmenu<WarningMenu>()
		.addTooltip("Shows you a warning on screen on an \nevent."));

	addOption(ToggleOption("Disable Killfeed")
		.addToggle(vars.bDisableKillFeed)
		.addTooltip("Disables the killfeed on the top right."));

	addOption(ToggleOption("Show Engine Logo")
		.addToggle(vars.bShowNiNJAWatermark)
		.addTooltip("Displays the NiNJA Engine logo in the top \nleft corner."));

	addOption(ToggleOption("Show FPS")
		.addToggle(vars.bShowFrameRate)
		.addFunction(ResetFPS)
		.addTooltip("Displays the current FPS in the top left \ncorner."));

	addOption(ToggleOption("Show Map Info")
		.addToggle(vars.bShowMapInfo)
		.addTooltip("Displays the current mapname, gametype \nand server type in the top left corner."));

	addOption(ToggleOption("Show Menu Instructions")
		.addToggle(vars.bShowMenuInstructions)
		.addTooltip("Displays instructions on how to open the \nmenu."));

	addOption(ButtonOption("Unload Engine")
		.addFunction(CB_Unload)
		.addTooltip("Unload the engine."));

	vars.bShowMenuInstructions = true;
}

/*Update once when submenu is opened*/
void MainMenu::UpdateOnce() {}

/*Update while submenu is active*/
void MainMenu::Update() {}

/*Background update*/
void MainMenu::FeatureUpdate() {
	/*if (vars.bForceRail && vars.pOldAsset) {
		static int timerCheck = 0;
		if (GetTickCount() - timerCheck > 2000) {
			timerCheck = GetTickCount();

			if (!pGame.bIsUsingRailgun) {
				LoadoutEditorMenu::GetInstance()->UpdateOnce();

				LoadoutEditorMenuVars::vars.iType = 0;
				LoadoutEditorMenuVars::ChangeLoadoutItem("U_Railgun");
			}
		}
	}*/

	if (vars.bShowMenuInstructions) {
		if (!pMenu.IsOpen()) {
			pDrawText.DrawOptionTextBoldCenterAlignWithBox("Press D-LEFT & LT to open NiNJA Engine", -4.f, 640.f, 30.f, 14.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150));
		}
	}

	int displayIndex = 0;

	if (vars.bShowNiNJAWatermark) {
		pDrawText.DrawOptionTextWithBox("NiNJA Engine v1.0.2 - RELEASE CANDIDATE", 20.f, 30.f + (displayIndex * 24.f), 16.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150), false);
		displayIndex++;
	}

	if (vars.bShowMapInfo) {
		const char* pServerType = NULL;

		switch (pGame.GetSyncedBFSettings()->ServerType) {
			case ServerType_Official: pServerType = "Official"; break;
			case ServerType_Ranked: pServerType = "Ranked"; break;
			case ServerType_Unranked: pServerType = "Unranked"; break;
			case ServerType_Private: pServerType = "Private"; break;
		}

		auto gameType = GetReadableGameType();
		if (strcmp(gameType, "Unknown")) {
			auto levelName = GetReadableLevelName();
			if (strcmp(levelName, "Unknown")) {
				pDrawText.DrawOptionTextWithBox(pUtils.va("Playing %s on %s", gameType, levelName), 20.f, 30.f + (displayIndex * 24.f), 16.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150), false);
				displayIndex++;
			}
		}

		pDrawText.DrawOptionTextWithBox(pUtils.va("Server Type: %s", pServerType), 20.f, 30.f + (displayIndex * 24.f), 16.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150), false);
		displayIndex++;

		// only issue with this is that if you go into test range its still the same as the previous, i'll make
		// a fix for this later...
		if (*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwServerInfoData)) {
			auto base = (DWORD)*(DWORD**)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwServerInfoData);
			if (base) {
				auto next = *(DWORD*)(base + 0x44);
				if (next) {
					auto again = *(DWORD*)next;
					if (again) {
						auto str = (char*)*(DWORD*)(again + 0x10);
						if (!str || strlen(str) <= 2) {
							// idk, stupid check.
							str = "[DICE]";
						}

						pDrawText.DrawOptionTextWithBox(pUtils.va("Server Name: %s", str), 20.f, 30.f + (displayIndex * 24.f), 16.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150), false);
						displayIndex++;
					}
				}
			}
		}
	}

	if (vars.bShowFrameRate) {
		static float fps;
		static int timer;

		if (ValidPtr(pGame.GetClientGameContext())
			&& ValidPtr(pGame.GetClientGameContext()->pClient)) {

			if (GetTickCount() - timer > 1000) {
				timer = GetTickCount();
				fps = pGame.GetClientGameContext()->pClient->fFPS2;
			}

			pDrawText.DrawOptionTextWithBox(pUtils.va("FPS: %.0f", fps), 20.f, 30.f + (displayIndex * 24.f), 16.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150), false);
			displayIndex++;
		}
	}
}

/*Singleton*/
MainMenu* _instance;
MainMenu* MainMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MainMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
MainMenu::~MainMenu() { delete _instance; }