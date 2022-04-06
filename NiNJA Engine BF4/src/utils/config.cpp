#include "stdafx.h"
#include "menu/handler/submenus/main/mainChatCommands.h"
#include "menu/handler/submenus/main/movementMenu.h"
#include "menu/handler/submenus/main/ordersMenu.h"
#include "menu/handler/submenus/main/warningMenu.h"
#include "menu/handler/submenus/settings/soundsMenu.h"
#include "menu/handler/submenus/main/developerMenu.h"

Config pConfig;

const char* FormatToggle(bool status) {
	return status ? "true" : "false";
}

void Config::Initialize() {
	if (!pUtils.DirectoryExists("NINJA:\\BF-Engine\\")) {
		DbgPrint("Creating BF-Engine directory");
		CreateDirectoryA("NINJA:\\BF-Engine\\", nullptr);
	}

	if (pUtils.FileExists("NINJA:\\BF4-Engine.ini")) {
		bCanLoadConfigOnceInitialized = true;
	}
}

void Config::CreateConfig(const char* pFile) {
	DbgPrint("Creating config: %s", pFile);

	FILE* file = fopen(pFile, "w+");
	if (file) {
		fprintf(file, "[Aimbot Menu]\n");
		fprintf(file, "Aimbot Type = %i\n", AimbotMenuVars::vars.iAimbotType);
		fprintf(file, "Target Bone = %i\n", AimbotMenuVars::vars.iTargetBone);
		fprintf(file, "Smooth Scale = %.0f\n", AimbotMenuVars::vars.fSmoothScale);
		fprintf(file, "Max Distance = %i\n", AimbotMenuVars::vars.iMaxDistance);
		fprintf(file, "Aim Required = %s\n", FormatToggle(AimbotMenuVars::vars.bAimRequried));
		fprintf(file, "Limit to FOV = %s\n", FormatToggle(AimbotMenuVars::vars.bLimitToFOV));
		fprintf(file, "Draw FOV Radius = %s\n", FormatToggle(AimbotMenuVars::vars.bDrawFOV));
		fprintf(file, "FOV Radius = %.0f\n", AimbotMenuVars::vars.fFOVRadius);
		fprintf(file, "Ignore Event = %i\n", AimbotMenuVars::vars.iIngores);
		fprintf(file, "Auto-Wall = %s\n", FormatToggle(AimbotMenuVars::vars.bAutoWall));
		fprintf(file, "Auto-Aim = %s\n", FormatToggle(AimbotMenuVars::vars.bAutoAim));
		fprintf(file, "Auto-Shoot = %s\n", FormatToggle(AimbotMenuVars::vars.bAutoShoot));
		fprintf(file, "Rate of Fire = %i\n", AimbotMenuVars::vars.iAutoShootDelay);
		fprintf(file, "Offscreen Targeting = %s\n", FormatToggle(AimbotMenuVars::vars.bOffscreenTargeting));
		fprintf(file, "Unfair = %s\n", FormatToggle(AimbotMenuVars::vars.bUnfair));
		fprintf(file, "Visibility Check = %s\n", FormatToggle(AimbotMenuVars::vars.bVisibilityCheck));
		fprintf(file, "Aim on Friendlies = %s\n", FormatToggle(AimbotMenuVars::vars.bAimOnFriendlies));
		fprintf(file, "Aim on Enemies = %s\n", FormatToggle(AimbotMenuVars::vars.bAimOnEnemies));
		fprintf(file, "Aim on Vehicles = %s\n", FormatToggle(AimbotMenuVars::vars.bAimOnVehicles));
		fprintf(file, "Spoof Weapon to Target = %s\n", FormatToggle(AimbotMenuVars::vars.bSpoofWeaponToTarget));
		fprintf(file, "Spoof Weapon = %i\n", AimbotMenuVars::vars.iSpoofWeapon);

#ifdef DEVELOPER_OPTIONS
		fprintf(file, "\n[Developer Menu]\n");
		fprintf(file, "Fly Mode = %s\n", FormatToggle(DeveloperMenuVars::vars.bFlyHack));
		fprintf(file, "Fly Speed = %.0f\n", DeveloperMenuVars::vars.fFlyHackSpeed);
#endif

		fprintf(file, "\n[Main Menu]\n");
		// fprintf(file, "Hack vs Hack (Force Railgun) = %s\n", FormatToggle(MainMenuVars::vars.bForceRail));
		fprintf(file, "Disable Killfeed = %s\n", FormatToggle(MainMenuVars::vars.bDisableKillFeed));
		fprintf(file, "Show Engine Logo = %s\n", FormatToggle(MainMenuVars::vars.bShowNiNJAWatermark));
		fprintf(file, "Show Menu Instructions = %s\n", FormatToggle(MainMenuVars::vars.bShowMenuInstructions));
		fprintf(file, "Show FPS = %s\n", FormatToggle(MainMenuVars::vars.bShowFrameRate));
		fprintf(file, "Show Map Info = %s\n", FormatToggle(MainMenuVars::vars.bShowMapInfo));

		fprintf(file, "\n[Visuals Menu]\n");
		fprintf(file, "Bones = %i\n", VisualMenuVars::vars.iBones);
		fprintf(file, "Boxes = %i\n", VisualMenuVars::vars.iBoxes);
		fprintf(file, "Box Type = %i\n", VisualMenuVars::vars.iBoxType);
		fprintf(file, "Crosshair Type = %i\n", VisualMenuVars::vars.iCrosshair);
		fprintf(file, "Distance = %i\n", VisualMenuVars::vars.iDistance);
		fprintf(file, "Name = %i\n", VisualMenuVars::vars.iName);
		fprintf(file, "Weapon = %i\n", VisualMenuVars::vars.iWeapon);
		fprintf(file, "Chams = %i\n", VisualMenuVars::vars.iChams);
		fprintf(file, "Health = %i\n", VisualMenuVars::vars.iHealth);
		fprintf(file, "Occupied Vehicles = %i\n", VisualMenuVars::vars.iOccupiedVehicles);
		fprintf(file, "Snaplines = %i\n", VisualMenuVars::vars.iSnapline);
		fprintf(file, "Snapline Position = %i\n", VisualMenuVars::vars.iSnaplinePosition);
		fprintf(file, "Supply Crates = %s\n", FormatToggle(VisualMenuVars::vars.bSupplyCrates));
		fprintf(file, "Explosives = %s\n", FormatToggle(VisualMenuVars::vars.bExplosives));
		fprintf(file, "Grenades = %s\n", FormatToggle(VisualMenuVars::vars.bGrenades));
		fprintf(file, "Pickups = %s\n", FormatToggle(VisualMenuVars::vars.bPickups));
		fprintf(file, "Vehicles = %s\n", FormatToggle(VisualMenuVars::vars.bEmptyVehicles));
		fprintf(file, "Show Aimbot Information = %s\n", FormatToggle(VisualMenuVars::vars.bShowAimbotInfo));

		fprintf(file, "\n[Weapon Menu]\n");
		fprintf(file, "No Recoil = %s\n", FormatToggle(WeaponMenuVars::vars.bNoRecoil));
		fprintf(file, "No Sway = %s\n", FormatToggle(WeaponMenuVars::vars.bNoSway));
		fprintf(file, "No Breath = %s\n", FormatToggle(WeaponMenuVars::vars.bNoBreath));
		fprintf(file, "No Spread = %s\n", FormatToggle(WeaponMenuVars::vars.bNoSpread));
		fprintf(file, "No Parachute Constraints = %s\n", FormatToggle(WeaponMenuVars::vars.bNoParachuteConstraints));
		fprintf(file, "No Bullet Drop = %s\n", FormatToggle(WeaponMenuVars::vars.bNoBulletDrop));
		fprintf(file, "Instant Hit = %s\n", FormatToggle(WeaponMenuVars::vars.bInstantHit));
		fprintf(file, "Instant Kill = %s\n", FormatToggle(WeaponMenuVars::vars.bInstantKill));
		fprintf(file, "Always Headshot = %s\n", FormatToggle(WeaponMenuVars::vars.bAlwaysHeadshot));
		fprintf(file, "Longer Bullet Life = %s\n", FormatToggle(WeaponMenuVars::vars.bLongerBulletLife));
		fprintf(file, "Self Heal Gun = %s\n", FormatToggle(WeaponMenuVars::vars.bSelfHealGun));
		fprintf(file, "Team Heal Gun = %s\n", FormatToggle(WeaponMenuVars::vars.bTeamHealGun));
		fprintf(file, "Unlimited Ammo = %s\n", FormatToggle(WeaponMenuVars::vars.bUnlimitedAmmo));
		fprintf(file, "Auto Reload = %s\n", FormatToggle(WeaponMenuVars::vars.bAutoReload));
		fprintf(file, "Bullets Per Shell = %i\n", WeaponMenuVars::vars.iNumOfBulletsPerShell);
		fprintf(file, "Bullets Per Shot = %i\n", WeaponMenuVars::vars.iNumOfBulletsPerShot);
		fprintf(file, "Bullets Per Burst = %i\n", WeaponMenuVars::vars.iNumOfBulletsPerBurst);

		fprintf(file, "\n[Chat Messages Menu]\n");
		fprintf(file, "Channel Type = %i\n", MainChatCommandsMenuVars::vars.iChannelType);
		fprintf(file, "Message Type = %i\n", MainChatCommandsMenuVars::vars.iMessageType);
		fprintf(file, "Kill Feed - Kills = %s\n", FormatToggle(MainChatCommandsMenuVars::vars.bCustomKillFeedKill));
		fprintf(file, "Kill Feed - Death = %s\n", FormatToggle(MainChatCommandsMenuVars::vars.bCustomKillFeedDeath));

		fprintf(file, "\n[Movement Menu]\n");
		fprintf(file, "Teleport Above = %s\n", FormatToggle(MovementMenuVars::vars.bTeleportAbove));
		fprintf(file, "Teleport Above Height = %i\n", MovementMenuVars::vars.iTeleportAboveHeight);
		fprintf(file, "Teleport to Crosshair = %s\n", FormatToggle(MovementMenuVars::vars.bTeleportToCrosshair));

		fprintf(file, "\n[Orders Menu]\n");
		fprintf(file, "Order to Send = %i\n", OrdersMenuVars::vars.iOrder);
		fprintf(file, "Always Order = %s\n", FormatToggle(OrdersMenuVars::vars.bAlwaysOrder));

		fprintf(file, "\n[Warning Menu]\n");
		fprintf(file, "Proximity Warning = %i\n", WarningMenuVars::vars.iProximity);
		fprintf(file, "Visible Warning = %i\n", WarningMenuVars::vars.iVisible);

		fprintf(file, "\n[Visuals Customization Menu]\n");
		fprintf(file, "Enemy = %.2f,%.2f,%.2f,%.2f\n", VisualMenuVars::vars.cEnemy.R, VisualMenuVars::vars.cEnemy.G, VisualMenuVars::vars.cEnemy.B, VisualMenuVars::vars.cEnemy.A);
		fprintf(file, "Friendly = %.2f,%.2f,%.2f,%.2f\n", VisualMenuVars::vars.cFriendly.R, VisualMenuVars::vars.cFriendly.G, VisualMenuVars::vars.cFriendly.B, VisualMenuVars::vars.cFriendly.A);
		fprintf(file, "Preferred = %.2f,%.2f,%.2f,%.2f\n", VisualMenuVars::vars.cPreferred.R, VisualMenuVars::vars.cPreferred.G, VisualMenuVars::vars.cPreferred.B, VisualMenuVars::vars.cPreferred.A);
		fprintf(file, "Wallbangable = %.2f,%.2f,%.2f,%.2f\n", VisualMenuVars::vars.cWallbangable.R, VisualMenuVars::vars.cWallbangable.G, VisualMenuVars::vars.cWallbangable.B, VisualMenuVars::vars.cWallbangable.A);
		fprintf(file, "Whitelisted = %.2f,%.2f,%.2f,%.2f\n", VisualMenuVars::vars.cWhitelisted.R, VisualMenuVars::vars.cWhitelisted.G, VisualMenuVars::vars.cWhitelisted.B, VisualMenuVars::vars.cWhitelisted.A);
		fprintf(file, "Visible = %.2f,%.2f,%.2f,%.2f\n", VisualMenuVars::vars.cVisible.R, VisualMenuVars::vars.cVisible.G, VisualMenuVars::vars.cVisible.B, VisualMenuVars::vars.cVisible.A);

		fprintf(file, "\n[Sounds Menu]\n");
		fprintf(file, "Killing Spree FX = %s\n", FormatToggle(SoundsMenuVars::vars.bKillingSpree));
		fprintf(file, "Killing Streak FX = %s\n", FormatToggle(SoundsMenuVars::vars.bKillingStreak));

		fprintf(file, "\n[Internal Settings]\n");
		fprintf(file, "Last Version = %i\n", pConfig.iServerChangelogVersion);

		fprintf(file, "\n[Menu Colors Menu]\n");
		fprintf(file, "Background = %.2f,%.2f,%.2f,%.2f\n", pRenderer.UIColors.menuBackground.R, pRenderer.UIColors.menuBackground.G, pRenderer.UIColors.menuBackground.B, pRenderer.UIColors.menuBackground.A);
		fprintf(file, "Border = %.2f,%.2f,%.2f,%.2f\n", pRenderer.UIColors.border.R, pRenderer.UIColors.border.G, pRenderer.UIColors.border.B, pRenderer.UIColors.border.A);
		fprintf(file, "Description Text = %.2f,%.2f,%.2f,%.2f\n", pRenderer.UIColors.descriptionText.R, pRenderer.UIColors.descriptionText.G, pRenderer.UIColors.descriptionText.B, pRenderer.UIColors.descriptionText.A);
		fprintf(file, "Scrollbar = %.2f,%.2f,%.2f,%.2f\n", pRenderer.UIColors.scrollbar.R, pRenderer.UIColors.scrollbar.G, pRenderer.UIColors.scrollbar.B, pRenderer.UIColors.scrollbar.A);
		fprintf(file, "Selected Text = %.2f,%.2f,%.2f,%.2f\n", pRenderer.UIColors.selectedText.R, pRenderer.UIColors.selectedText.G, pRenderer.UIColors.selectedText.B, pRenderer.UIColors.selectedText.A);
		fprintf(file, "Text = %.2f,%.2f,%.2f,%.2f\n", pRenderer.UIColors.text.R, pRenderer.UIColors.text.G, pRenderer.UIColors.text.B, pRenderer.UIColors.text.A);
		fprintf(file, "Title Text = %.2f,%.2f,%.2f,%.2f\n", pRenderer.UIColors.titleText.R, pRenderer.UIColors.titleText.G, pRenderer.UIColors.titleText.B, pRenderer.UIColors.titleText.A);
		fprintf(file, "Top/Bottom = %.2f,%.2f,%.2f,%.2f\n", pRenderer.UIColors.topBottom.R, pRenderer.UIColors.topBottom.G, pRenderer.UIColors.topBottom.B, pRenderer.UIColors.topBottom.A);
		fprintf(file, "Slider = %.2f,%.2f,%.2f,%.2f", pRenderer.UIColors.slider.R, pRenderer.UIColors.slider.G, pRenderer.UIColors.slider.B, pRenderer.UIColors.slider.A);

		fclose(file);
	} else {
		DbgPrint("Couldn't load file, shit");
	}
}

void Config::LoadConfig(const char* pFile) {
	auto ini = IniParse(pFile);
	if (ini.IsGood()) {
		DbgPrint("Parsed ini into maps, about to parse");
		DbgPrint("%i sections, %i keys", ini.GetSections().size(), ini.GetKeys().size());

		// developer menu
#ifdef DEVELOPER_OPTIONS
		DeveloperMenuVars::vars.bFlyHack = ini.ReadBool("Developer Menu", "Fly Mode", DeveloperMenuVars::vars.bFlyHack);
		DeveloperMenuVars::vars.fFlyHackSpeed = ini.ReadFloat("Developer Menu", "Fly Speed", DeveloperMenuVars::vars.fFlyHackSpeed);
#endif

		// aimbot menu
		AimbotMenuVars::vars.iAimbotType = ini.ReadInt("Aimbot Menu", "Aimbot Type", AimbotMenuVars::vars.iAimbotType);
		AimbotMenuVars::vars.iTargetBone = ini.ReadInt("Aimbot Menu", "Target Bone", AimbotMenuVars::vars.iTargetBone);
		AimbotMenuVars::vars.fSmoothScale = ini.ReadFloat("Aimbot Menu", "Smooth Scale", AimbotMenuVars::vars.fSmoothScale);
		AimbotMenuVars::vars.iMaxDistance = ini.ReadInt("Aimbot Menu", "Max Distance", AimbotMenuVars::vars.iMaxDistance);
		AimbotMenuVars::vars.bAimRequried = ini.ReadBool("Aimbot Menu", "Aim Required", AimbotMenuVars::vars.bAimRequried);
		AimbotMenuVars::vars.bLimitToFOV = ini.ReadBool("Aimbot Menu", "Limit to FOV", AimbotMenuVars::vars.bLimitToFOV);
		AimbotMenuVars::vars.bDrawFOV = ini.ReadBool("Aimbot Menu", "Draw FOV Radius", AimbotMenuVars::vars.bDrawFOV);
		AimbotMenuVars::vars.fFOVRadius = ini.ReadFloat("Aimbot Menu", "FOV Radius", AimbotMenuVars::vars.fFOVRadius);
		AimbotMenuVars::vars.iIngores = ini.ReadInt("Aimbot Menu", "Ignore Event", AimbotMenuVars::vars.iIngores);
		AimbotMenuVars::vars.bAutoWall = ini.ReadBool("Aimbot Menu", "Auto-Wall", AimbotMenuVars::vars.bAutoWall);
		AimbotMenuVars::vars.bAutoAim = ini.ReadBool("Aimbot Menu", "Auto-Aim", AimbotMenuVars::vars.bAutoAim);
		AimbotMenuVars::vars.bAutoShoot = ini.ReadBool("Aimbot Menu", "Auto-Shoot", AimbotMenuVars::vars.bAutoShoot);
		AimbotMenuVars::vars.iAutoShootDelay = ini.ReadInt("Aimbot Menu", "Rate of Fire", AimbotMenuVars::vars.iAutoShootDelay);
		AimbotMenuVars::vars.bOffscreenTargeting = ini.ReadBool("Aimbot Menu", "Offscreen Targeting", AimbotMenuVars::vars.bOffscreenTargeting);
		AimbotMenuVars::vars.bUnfair = ini.ReadBool("Aimbot Menu", "Unfair", AimbotMenuVars::vars.bUnfair);
		AimbotMenuVars::vars.bVisibilityCheck = ini.ReadBool("Aimbot Menu", "Visibility Check", AimbotMenuVars::vars.bVisibilityCheck);
		AimbotMenuVars::vars.bAimOnFriendlies = ini.ReadBool("Aimbot Menu", "Aim on Friendlies", AimbotMenuVars::vars.bAimOnFriendlies);
		AimbotMenuVars::vars.bAimOnEnemies = ini.ReadBool("Aimbot Menu", "Aim on Enemies", AimbotMenuVars::vars.bAimOnEnemies);
		AimbotMenuVars::vars.bAimOnVehicles = ini.ReadBool("Aimbot Menu", "Aim on Vehicles", AimbotMenuVars::vars.bAimOnVehicles);
		AimbotMenuVars::vars.bSpoofWeaponToTarget = ini.ReadBool("Aimbot Menu", "Spoof Weapon to Target", AimbotMenuVars::vars.bSpoofWeaponToTarget);
		AimbotMenuVars::vars.iSpoofWeapon = ini.ReadInt("Aimbot Menu", "Spoof Weapon", AimbotMenuVars::vars.iSpoofWeapon);

		// main menu
		//MainMenuVars::vars.bForceRail = ini.ReadBool("Main Menu", "Hack vs Hack (Force Railgun)", MainMenuVars::vars.bForceRail);
		MainMenuVars::vars.bDisableKillFeed = ini.ReadBool("Main Menu", "Disable Killfeed", MainMenuVars::vars.bDisableKillFeed);
		MainMenuVars::vars.bShowNiNJAWatermark = ini.ReadBool("Main Menu", "Show Engine Logo", MainMenuVars::vars.bShowNiNJAWatermark);
		MainMenuVars::vars.bShowMenuInstructions = ini.ReadBool("Main Menu", "Show Menu Instructions", MainMenuVars::vars.bShowMenuInstructions);
		MainMenuVars::vars.bShowFrameRate = ini.ReadBool("Main Menu", "Show FPS", MainMenuVars::vars.bShowFrameRate);
		MainMenuVars::vars.bShowMapInfo = ini.ReadBool("Main Menu", "Show Map Info", MainMenuVars::vars.bShowMapInfo);

		// visual menu
		VisualMenuVars::vars.iBones = ini.ReadInt("Visuals Menu", "Bones", VisualMenuVars::vars.iBones);
		VisualMenuVars::vars.iBoxes = ini.ReadInt("Visuals Menu", "Boxes", VisualMenuVars::vars.iBoxes);
		VisualMenuVars::vars.iBoxType = ini.ReadInt("Visuals Menu", "Box Type", VisualMenuVars::vars.iBoxType);
		VisualMenuVars::vars.iCrosshair = ini.ReadInt("Visuals Menu", "Crosshair Type", VisualMenuVars::vars.iCrosshair);
		VisualMenuVars::vars.iDistance = ini.ReadInt("Visuals Menu", "Distance", VisualMenuVars::vars.iDistance);
		VisualMenuVars::vars.iName = ini.ReadInt("Visuals Menu", "Name", VisualMenuVars::vars.iName);
		VisualMenuVars::vars.iWeapon = ini.ReadInt("Visuals Menu", "Weapon", VisualMenuVars::vars.iWeapon);
		VisualMenuVars::vars.iChams = ini.ReadInt("Visuals Menu", "Chams", VisualMenuVars::vars.iChams);
		VisualMenuVars::vars.iHealth = ini.ReadInt("Visuals Menu", "Health", VisualMenuVars::vars.iHealth);
		VisualMenuVars::vars.iOccupiedVehicles = ini.ReadInt("Visuals Menu", "Occupied Vehicles", VisualMenuVars::vars.iOccupiedVehicles);
		VisualMenuVars::vars.iSnapline = ini.ReadInt("Visuals Menu", "Snaplines", VisualMenuVars::vars.iSnapline);
		VisualMenuVars::vars.iSnaplinePosition = ini.ReadInt("Visuals Menu", "Snapline Position", VisualMenuVars::vars.iSnaplinePosition);
		VisualMenuVars::vars.bSupplyCrates = ini.ReadBool("Visuals Menu", "Supply Crates", VisualMenuVars::vars.bSupplyCrates);
		VisualMenuVars::vars.bExplosives = ini.ReadBool("Visuals Menu", "Explosives", VisualMenuVars::vars.bExplosives);
		VisualMenuVars::vars.bGrenades = ini.ReadBool("Visuals Menu", "Grenades", VisualMenuVars::vars.bGrenades);
		VisualMenuVars::vars.bPickups = ini.ReadBool("Visuals Menu", "Pickups", VisualMenuVars::vars.bPickups);
		VisualMenuVars::vars.bEmptyVehicles = ini.ReadBool("Visuals Menu", "Vehicles", VisualMenuVars::vars.bEmptyVehicles);
		VisualMenuVars::vars.bShowAimbotInfo = ini.ReadBool("Visuals Menu", "Show Aimbot Information", VisualMenuVars::vars.bShowAimbotInfo);

		// weapon menu
		WeaponMenuVars::vars.bNoRecoil = ini.ReadBool("Weapon Menu", "No Recoil", WeaponMenuVars::vars.bNoRecoil);
		WeaponMenuVars::vars.bNoSway = ini.ReadBool("Weapon Menu", "No Sway", WeaponMenuVars::vars.bNoSway);
		WeaponMenuVars::vars.bNoBreath = ini.ReadBool("Weapon Menu", "No Breath", WeaponMenuVars::vars.bNoBreath);
		WeaponMenuVars::vars.bNoSpread = ini.ReadBool("Weapon Menu", "No Spread", WeaponMenuVars::vars.bNoSpread);
		WeaponMenuVars::vars.bNoParachuteConstraints = ini.ReadBool("Weapon Menu", "No Parachute Constraints", WeaponMenuVars::vars.bNoParachuteConstraints);
		WeaponMenuVars::vars.bNoBulletDrop = ini.ReadBool("Weapon Menu", "No Bullet Drop", WeaponMenuVars::vars.bNoBulletDrop);
		WeaponMenuVars::vars.bInstantHit = ini.ReadBool("Weapon Menu", "Instant Hit", WeaponMenuVars::vars.bInstantHit);
		WeaponMenuVars::vars.bInstantKill = ini.ReadBool("Weapon Menu", "Instant Kill", WeaponMenuVars::vars.bInstantKill);
		WeaponMenuVars::vars.bAlwaysHeadshot = ini.ReadBool("Weapon Menu", "Always Headshot", WeaponMenuVars::vars.bAlwaysHeadshot);
		WeaponMenuVars::vars.bLongerBulletLife = ini.ReadBool("Weapon Menu", "Longer Bullet Life", WeaponMenuVars::vars.bLongerBulletLife);
		WeaponMenuVars::vars.bSelfHealGun = ini.ReadBool("Weapon Menu", "Self Heal Gun", WeaponMenuVars::vars.bSelfHealGun);
		WeaponMenuVars::vars.bTeamHealGun = ini.ReadBool("Weapon Menu", "Team Heal Gun", WeaponMenuVars::vars.bTeamHealGun);
		WeaponMenuVars::vars.bUnlimitedAmmo = ini.ReadBool("Weapon Menu", "Unlimited Ammo", WeaponMenuVars::vars.bUnlimitedAmmo);
		WeaponMenuVars::vars.bAutoReload = ini.ReadBool("Weapon Menu", "Auto Reload", WeaponMenuVars::vars.bAutoReload);
		WeaponMenuVars::vars.iNumOfBulletsPerShell = ini.ReadInt("Weapon Menu", "Bullets Per Shell", WeaponMenuVars::vars.iNumOfBulletsPerShell);
		WeaponMenuVars::vars.iNumOfBulletsPerShot = ini.ReadInt("Weapon Menu", "Bullets Per Shot", WeaponMenuVars::vars.iNumOfBulletsPerShot);
		WeaponMenuVars::vars.iNumOfBulletsPerBurst = ini.ReadInt("Weapon Menu", "Bullets Per Burst", WeaponMenuVars::vars.iNumOfBulletsPerBurst);

		// chat messages menu
		MainChatCommandsMenuVars::vars.iChannelType = ini.ReadInt("Chat Messages Menu", "Channel Type", MainChatCommandsMenuVars::vars.iChannelType);
		MainChatCommandsMenuVars::vars.iMessageType = ini.ReadInt("Chat Messages Menu", "Message Type", MainChatCommandsMenuVars::vars.iMessageType);
		MainChatCommandsMenuVars::vars.bCustomKillFeedKill = ini.ReadBool("Chat Messages Menu", "Kill Feed - Kills", MainChatCommandsMenuVars::vars.bCustomKillFeedKill);
		MainChatCommandsMenuVars::vars.bCustomKillFeedDeath = ini.ReadBool("Chat Messages Menu", "Kill Feed - Death", MainChatCommandsMenuVars::vars.bCustomKillFeedDeath);

		// movement menu
		MovementMenuVars::vars.bTeleportAbove = ini.ReadBool("Movement Menu", "Teleport Above", MovementMenuVars::vars.bTeleportAbove);
		MovementMenuVars::vars.iTeleportAboveHeight = ini.ReadInt("Movement Menu", "Teleport Above Height", MovementMenuVars::vars.iTeleportAboveHeight);
		MovementMenuVars::vars.bTeleportToCrosshair = ini.ReadBool("Movement Menu", "Teleport to Crosshair", MovementMenuVars::vars.bTeleportToCrosshair);

		// orders menu
		OrdersMenuVars::vars.iOrder = ini.ReadInt("Orders Menu", "Order to Send", OrdersMenuVars::vars.iOrder);
		OrdersMenuVars::vars.bAlwaysOrder = ini.ReadBool("Orders Menu", "Always Order", OrdersMenuVars::vars.bAlwaysOrder);

		// warning menu
		WarningMenuVars::vars.iProximity = ini.ReadInt("Warning Menu", "Proximity Warning", WarningMenuVars::vars.iProximity);
		WarningMenuVars::vars.iVisible = ini.ReadInt("Warning Menu", "Visible Warning", WarningMenuVars::vars.iVisible);

		// visual customization menu
		VisualMenuVars::vars.cEnemy = ini.ReadColor("Visuals Customization Menu", "Enemy", VisualMenuVars::vars.cEnemy);
		VisualMenuVars::vars.cFriendly = ini.ReadColor("Visuals Customization Menu", "Friendly", VisualMenuVars::vars.cFriendly);
		VisualMenuVars::vars.cPreferred = ini.ReadColor("Visuals Customization Menu", "Preferred", VisualMenuVars::vars.cPreferred);
		VisualMenuVars::vars.cWallbangable = ini.ReadColor("Visuals Customization Menu", "Wallbangable", VisualMenuVars::vars.cWallbangable);
		VisualMenuVars::vars.cWhitelisted = ini.ReadColor("Visuals Customization Menu", "Whitelisted", VisualMenuVars::vars.cWhitelisted);
		VisualMenuVars::vars.cVisible = ini.ReadColor("Visuals Customization Menu", "Visible", VisualMenuVars::vars.cVisible);

		// sounds menu
		SoundsMenuVars::vars.bKillingSpree = ini.ReadBool("Sounds Menu", "Killing Spree FX", SoundsMenuVars::vars.bKillingSpree);
		SoundsMenuVars::vars.bKillingStreak = ini.ReadBool("Sounds Menu", "Killing Streak FX", SoundsMenuVars::vars.bKillingStreak);

		// menu colors menu
		pRenderer.UIColors.menuBackground = ini.ReadColor("Menu Colors Menu", "Background", pRenderer.UIColors.menuBackground);
		pRenderer.UIColors.border = ini.ReadColor("Menu Colors Menu", "Border", pRenderer.UIColors.border);
		pRenderer.UIColors.descriptionText = ini.ReadColor("Menu Colors Menu", "Description Text", pRenderer.UIColors.descriptionText);
		pRenderer.UIColors.scrollbar = ini.ReadColor("Menu Colors Menu", "Scrollbar", pRenderer.UIColors.scrollbar);
		pRenderer.UIColors.selectedText = ini.ReadColor("Menu Colors Menu", "Selected Text", pRenderer.UIColors.selectedText);
		pRenderer.UIColors.text = ini.ReadColor("Menu Colors Menu", "Text", pRenderer.UIColors.text);
		pRenderer.UIColors.titleText = ini.ReadColor("Menu Colors Menu", "Title Text", pRenderer.UIColors.titleText);
		pRenderer.UIColors.topBottom = ini.ReadColor("Menu Colors Menu", "Top/Bottom", pRenderer.UIColors.topBottom);
		pRenderer.UIColors.slider = ini.ReadColor("Menu Colors Menu", "Slider", pRenderer.UIColors.slider);
	}
}

void Config::UpdateCurrentSettings() {
	CreateConfig("NINJA:\\BF4-Engine.ini");
}