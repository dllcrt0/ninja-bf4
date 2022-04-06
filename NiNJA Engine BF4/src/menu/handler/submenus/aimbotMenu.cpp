#include "stdafx.h"
#include "aimbotMenu.h"
#include "mainMenu.h"

using namespace AimbotMenuVars;

namespace AimbotMenuVars {
	Vars vars;

	ScrollStruct<int> AimbotTypes[] = {
		"Off", 0,
		"Normal", 1,
		"Silent", 2,
		"Humanized", 3
	};

	ScrollStruct<BONES> TargetBone[] = {
		"Head", Head,
		"Neck", Neck,
		"Spine", Spine,
		"Left Arm", LeftArm,
		"Right Arm", RightArm,
		"Left Shoulder", LeftShoulder,
		"Right Shoulder", RightShoulder,
		"Throat", Throat,
		"Left Foot", LeftFoot,
		"Right Foot", RightFoot,
		"Hips", Hips,
		"Left Elbow", LeftElbowRoll,
		"Right Elbow", RightElbowRoll
	};

	ScrollStruct<int> SpoofWeapons[] = {
		"Off", -1,
		"Primary Weapon", 0,
		"Secondary Weapon", 1,
		"Gadget", 2,
		"Gadget 2", 5,
		"Grenade", 6,
		"Knife", 8
	};

	ScrollStruct<int> Ignores[] = {
		"Off", 0,
		"In Air", 1,
		"Falling", 2,
		"Parachuting", 3,
		"All", 4
	};
}

void AimbotMenu::Init() {
	SetName("Aimbot");

	vars.iMaxDistance = 300;
	vars.fSmoothScale = 1.f;

	addOption(ScrollOption<int>(SCROLL, "Aimbot Type").canLoop()
		.addScroll(vars.iAimbotType, 0, NUMOF(AimbotTypes) - 1, AimbotTypes)
		.addTooltip("Normal: Snaps to targets, Silent: Never \nchange your crosshair position, \nHumanized: Legit type aimbot that looks \nhuman."));

	addOption(ScrollOption<BONES>(SCROLL, "Target Bone").canLoop()
		.addScroll(vars.iTargetBone, 0, NUMOF(TargetBone) - 1, TargetBone)
		.addTooltip("The bone you want the aimbot to prioritize \nabove all others."));

	addOption(NumberOption<float>(SCROLL, "Smooth Scale")
		.addNumber(vars.fSmoothScale, "%.0f", 1.f).addMin(1.f).addMax(20.f)
		.addTooltip("How smoothly the humanized aimbot aims \nto your current target. Higher values result \nin a smoother transition."));

	addOption(NumberOption<int>(SCROLL, "Max Distance")
		.addNumber(vars.iMaxDistance, "%i", 10).addMin(0).addMax(2000).setScrollSpeed(10)
		.addTooltip("The max distance a player can be to \nbecome a target."));

	addOption(ToggleOption("Aim Required")
		.addToggle(vars.bAimRequried)
		.addTooltip("You need to aim in to use aimbot."));

	addOption(ToggleOption("Limit to FOV")
		.addToggle(vars.bLimitToFOV)
		.addTooltip("Will only detect enemies in the editable FOV \nrange (edit the range below)."));

	addOption(ToggleOption("Draw FOV Radius")
		.addToggle(vars.bDrawFOV)
		.addTooltip("Draws the FOV radius on-screen."));

	addOption(NumberOption<float>(SCROLL, "FOV Radius", true)
		.addNumber(vars.fFOVRadius, "%.0f", 1.0f).addMin(1.0f).addMax(1000.f).setScrollSpeed(15)
		.addTooltip("Edits the FOV radius on-screen."));

	addOption(ScrollOption<int>(SCROLL, "Ignore Event")
		.addScroll(vars.iIngores, 0, NUMOF(Ignores) - 1, Ignores)
		.addTooltip("Ignores the target on an event."));

	addOption(ToggleOption("Auto-Wall")
		.addToggle(vars.bAutoWall)
		.addTooltip("Automatically checks to see if enemies can \nbe damaged through the wall."));

	addOption(ToggleOption("Auto-Aim")
		.addToggle(vars.bAutoAim)
		.addTooltip("Automatically aims when an enemy is \nvisible, or wallbangable if autowall is \nenabled."));

	addOption(ToggleOption("Auto-Shoot")
		.addToggle(vars.bAutoShoot)
		.addTooltip("Automatically fires when an enemy is \nvisible, or wallbangable if autowall is \nenabled."));

	addOption(NumberOption<int>(SCROLL, "Rate of Fire")
		.addNumber(vars.iAutoShootDelay, "%i ms", 5).addMin(0).addMax(3000)
		.addTooltip("Rate of fire for the auto-shoot."));
 
	addOption(ToggleOption("Offscreen Targeting")
		.addToggle(vars.bOffscreenTargeting)
		.addTooltip("Whether or not to target players who are \ncurrently not on your screen."));

	addOption(ToggleOption("Unfair")
		.addToggle(vars.bUnfair)
		.addTooltip("Kills an enemy if you shoot next to them."));

	addOption(ToggleOption("Visibility Check")
		.addToggle(vars.bVisibilityCheck)
		.addTooltip("Only grabs the enemy if they're visible \non-screen, or through walls if autowall is \nenabled."));

	addOption(ToggleOption("Aim on Friendlies")
		.addToggle(vars.bAimOnFriendlies)
		.addTooltip("Allows the aimbot to grab friendlies."));

	addOption(ToggleOption("Aim on Enemies")
		.addToggle(vars.bAimOnEnemies)
		.addTooltip("Allows the aimbot to grab enemies."));

	addOption(ToggleOption("Aim on Vehicles")
		.addToggle(vars.bAimOnVehicles)
		.addTooltip("Allows the aimbot to grab enemies in \nvehicles."));

	addOption(ToggleOption("Spoof Weapon to Target")
		.addToggle(vars.bSpoofWeaponToTarget)
		.addTooltip("Spoofs your weapon to the enemies \nweapon."));

	addOption(ScrollOption<int>(SCROLL, "Spoof Weapon").canLoop()
		.addScroll(vars.iSpoofWeapon, 0, NUMOF(SpoofWeapons) - 1, SpoofWeapons)
		.addTooltip("Spoofs your weapon to this weapon. This \nwill prioritize above all others."));
}

/*Update once when submenu is opened*/
void AimbotMenu::UpdateOnce() {}

/*Update while submenu is active*/
void AimbotMenu::Update() {}

/*Background update*/
void AimbotMenu::FeatureUpdate() {
	if (vars.bDrawFOV) {
		Vec3 center(1280 / 2, 720 / 2, 0.f);
		pRenderer.DrawCircle(center, vars.fFOVRadius, GColor(255, 255, 255), 30); // usually 30 for sides
	}
}

/*Singleton*/
AimbotMenu* _instance;
AimbotMenu* AimbotMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new AimbotMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
AimbotMenu::~AimbotMenu() { delete _instance; }