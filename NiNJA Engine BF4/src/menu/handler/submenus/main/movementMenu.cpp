#include "stdafx.h"
#include "movementMenu.h"
#include "../mainMenu.h"
#include "../main/developerMenu.h"

using namespace MovementMenuVars;

namespace MovementMenuVars {
	Vars vars;
	
	bool reset;
	int lastReset;

	void Teleport(Vec3 pos) {
		DbgPrint("Teleporting to %.3f %.3f %.3f", pos.x, pos.y, pos.z);

		if (ValidPtr(pGame.GetMySoldierEntity())) {
			if (ValidPtr(pGame.GetMySoldierEntity()->pClientVaultComponent)) {
				Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[0]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[0]));
				Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[1]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[1]));
				Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[2]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop));
				Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[3]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop));
				Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[4]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop));
				Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[5]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop));
				Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[6]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop));
				Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[7]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop));
				Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[8]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop));
				Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[9]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop));
				Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[10]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop));
				Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[11]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop));
				Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[12]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[2]));
				Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[13]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[3]));

				auto vault = pGame.GetMySoldierEntity()->pClientVaultComponent;
				if (ValidPtr(vault)) {
					vault->VaultCoords = pos;
					vault->fVaultHeight = 0.f;
					vault->fUnk[0] = 0.f;
					vault->fUnk[1] = 0.f;
					vault->fUnk[2] = 0.f;
					vault->iState = 2;

					lastReset = GetTickCount();
					reset = true;
				}
			}
		}
	}
}

void MovementMenu::Init() {
	SetParentSubmenu<MainMenu>();
	SetName("Movement");

	vars.fFlyHackSpeed = 20.f;
	vars.iTeleportAboveHeight = 30.f;

	addOption(ToggleOption("Teleport Above")
		.addToggle(vars.bTeleportAbove)
		.addTooltip("Use bind LS + RS to teleport above."));

	addOption(NumberOption<int>(SCROLL, "Teleport Above Height")
		.addNumber(vars.iTeleportAboveHeight, "%i", 5).addMin(5).addMax(2000).setScrollSpeed(70)
		.addTooltip("Sets the height for teleporting above. "));

	addOption(ToggleOption("Teleport to Crosshair")
		.addToggle(vars.bTeleportToCrosshair)
		.addTooltip("Teleports you to your crosshair. Use RB to \nteleport."));

	addOption(ToggleOption("Fly Mode")
		.addToggle(vars.bFlyHack)
		.addTooltip("Walk into the sky! You can also toggle this \nwithout the menu open by pressing LS & LT."));

	addOption(NumberOption<float>(SCROLL, "Fly Speed")
		.addNumber(vars.fFlyHackSpeed, "%.0f", 1.f).addMin(5.f).addMax(100.f).setScrollSpeed(10)
		.addTooltip("Changes the speed of the fly mode. Hold in \nLS to get a speed boost."));

}

/*Update once when submenu is opened*/
void MovementMenu::UpdateOnce() {}

/*Update while submenu is active*/
void MovementMenu::Update() {}

/*Background update*/
void MovementMenu::FeatureUpdate() {
	static int toggleTimer = 0;

	if (IsDown(LEFT_THUMB) && IsDown(LEFT_TRIGGER)) {
		if (GetTickCount() - toggleTimer > 1500) {
			toggleTimer = GetTickCount();

			vars.bFlyHack ^= true;
		}
	}

	if (vars.bTeleportAbove) {
		if (!vars.bFlyHack) {
			if (pController.IsJustPressed(RIGHT_THUMB) && pController.IsJustPressed(LEFT_THUMB)) {
				auto soldier = pGame.GetMySoldierEntity();
				if (ValidPtr(soldier)) {
					auto prediction = soldier->pPredictedController;
					if (ValidPtr(prediction)) {
						auto pos = pGame.GetMySoldierEntity()->pPredictedController->Position;
						pos.y += (float)vars.iTeleportAboveHeight;

						Teleport(pos);
					}
				}
			}
		} else {
			DbgPrint("Cancelling teleport because fly hack is enabled and you are flying.");
		}
	}

	if (vars.bTeleportToCrosshair) {
		if (!vars.bFlyHack) {
			if (pController.IsJustPressed(RIGHT_SHOULDER) && !pMenu.IsOpen()) {
				if (ValidPtr(pGame.GetSoldierWeapon())) {
					if (ValidPtr(pGame.GetSoldierWeapon()->pAimingSimulation)) {
						if (ValidPtr(pGame.GetSoldierWeapon()->pAimingSimulation->pData)) {
							pGame.GetSoldierWeapon()->pAimingSimulation->pData->fRayDistance = 9999.f;
							auto pos = pGame.GetSoldierWeapon()->pAimingSimulation->RayPosition;

							DbgPrint("RayPos: %.3f %.3f %.3f", pos.x, pos.y, pos.z);

							if (pos.x != 0.f && pos.y != 0.f) {
								Teleport(pos);
							}
						}
					}
				}
			}
		} else {
			DbgPrint("Cancelling teleport because fly hack is enabled and you are flying.");
		}
	}

	if (reset) {
		if (GetTickCount() - lastReset > 500) {
			reset = false;

			if (ValidPtr(pGame.GetMySoldierEntity())) {
				if (ValidPtr(pGame.GetMySoldierEntity()->pClientVaultComponent)) {
					auto vault = pGame.GetMySoldierEntity()->pClientVaultComponent;
					if (ValidPtr(vault)) {
						vault->iState = 0;
					}

					Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[0]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[4]));
					Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[1]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[5]));
					Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[2]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[6]));
					Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[3]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[7]));
					Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[4]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[8]));
					Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[5]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[9]));
					Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[6]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[10]));
					Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[7]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[11]));
					Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[8]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[12]));
					Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[9]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[13]));
					Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[10]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[14]));
					Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[11]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[15]));
					Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[12]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[16]));
					Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatches[13]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTeleportPatchValues[17]));
				}
			}
		}
	}
}

/*Singleton*/
MovementMenu* _instance;
MovementMenu* MovementMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MovementMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
MovementMenu::~MovementMenu() { delete _instance; }