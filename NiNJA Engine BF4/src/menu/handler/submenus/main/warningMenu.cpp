#include "stdafx.h"
#include "warningMenu.h"
#include "../mainMenu.h"

using namespace WarningMenuVars;

namespace WarningMenuVars {
	Vars vars;

	ScrollStruct<int> WarningDistance[] = {
		"Off", 0,
		"50m", 50,
		"100m", 100,
		"250m", 250,
		"500m", 500,
		"1000m", 1000,
		"Unlimited Distance", 99999
	};

	float WallDepth(Vec3 p1, Vec3 p2) {
		Vec3 v;
		pUtils.VectorSubtract(p2, p1, v);
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	Vec3 AnglesToForward(Vec3 angles, float Distance) {
		float angle, sr, sp, sy, cr, cp, cy, PiDiv;
		PiDiv = ((float)3.14159265359 / 180.f);
		angle = angles.y * PiDiv;
		sy = (float)sin(angle);
		cy = (float)cos(angle);
		angle = angles.x * PiDiv;
		sp = (float)sin(angle);
		cp = (float)cos(angle);
		angle = angles.z * PiDiv;
		sr = (float)sin(angle);
		cr = (float)cos(angle);
		
		return Vec3((cp * cy * Distance), (cp * sy * Distance), (-sp * Distance));
	}

	float GetAimDegree(Vec3 enemyPosition, Vec3 myPosition, Vec3 enemyAngle) {
		return WallDepth((enemyPosition + AnglesToForward(enemyAngle, WallDepth(myPosition, enemyPosition))), myPosition);
	}

	void AimAngleCallback(ClientPlayer* pEntity) {
		if (ValidPtr(pEntity)) {
			if (!pPlayer.IsFriendly(pEntity)) {
				auto soldier = pEntity->GetClientSoldier();
				if (ValidPtr(soldier)) {
					DbgPrint("%s angles are %.3f %.3f", pEntity->szName, soldier->fAuthorativeYaw, soldier->fAuthorativePitch);
				}
			}
		}
	}

	void ProximityCallback(ClientPlayer* pEntity) {
		if (ValidPtr(pEntity)) {
			if (!pPlayer.IsFriendly(pEntity)) {
				auto soldier = pEntity->GetClientSoldier();
				if (ValidPtr(soldier)) {
					auto predicted = soldier->pPredictedController;
					if (ValidPtr(predicted)) {
						auto mySoldier = pGame.GetMySoldierEntity();
						if (ValidPtr(mySoldier)) {
							auto myPredicted = mySoldier->pPredictedController;
							if (ValidPtr(myPredicted)) {
								auto distance = pUtils.GetDistance(myPredicted->Position, predicted->Position);
								if (distance <= WarningDistance[vars.iProximity].Result) {
									// IN PROXIMITY
									pDrawText.DrawOptionTextCenterAlign(pUtils.va("%s is in proximity", pEntity->szName), 0.f, 20.f + (vars.iDrawIndex * 18.f), 15.f, GColor(255, 0, 255, 255));
									vars.iDrawIndex++;
								}
							}
						}
					}
				}
			}
		}
	}

	void VisibleCallback(ClientPlayer* pEntity) {
		if (ValidPtr(pEntity)) {
			if (!pPlayer.IsFriendly(pEntity)) {
				auto soldier = pEntity->GetClientSoldier();
				if (ValidPtr(soldier)) {
					auto predicted = soldier->pPredictedController;
					if (ValidPtr(predicted)) {
						auto mySoldier = pGame.GetMySoldierEntity();
						if (ValidPtr(mySoldier)) {
							auto myPredicted = mySoldier->pPredictedController;
							if (ValidPtr(myPredicted)) {
								auto distance = pUtils.GetDistance(myPredicted->Position, predicted->Position);
								if (distance <= WarningDistance[vars.iVisible].Result) {
									if (soldier->IsVisible()) {
										// IS VISIBLE
										pDrawText.DrawOptionTextCenterAlign(pUtils.va("%s is visible", pEntity->szName), 0.f, 20.f + (vars.iDrawIndex * 18.f), 15.f, GColor(255, 255, 0, 255));
										vars.iDrawIndex++;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void WarningMenu::Init() {
	SetParentSubmenu<MainMenu>();
	SetName("Warnings");

	/*addOption(ScrollOption<int>(SCROLL, "Look Warning").canLoop()
		.addScroll(vars.iAimAngle, 0, NUMOF(WarningDistance) - 1, WarningDistance)
		.addTooltip("Shows a warning on screen when someone \nis looking at you."));*/

	addOption(ScrollOption<int>(SCROLL, "Proximity Warning").canLoop()
		.addScroll(vars.iProximity, 0, NUMOF(WarningDistance) - 1, WarningDistance)
		.addTooltip("Shows a warning on screen when someone \nis in your proximity (near you)."));
	
	addOption(ScrollOption<int>(SCROLL, "Visible Warning").canLoop()
		.addScroll(vars.iVisible, 0, NUMOF(WarningDistance) - 1, WarningDistance)
		.addTooltip("Shows a warning on screen when someone \nis visible."));
}

/*Update once when submenu is opened*/
void WarningMenu::UpdateOnce() {}

/*Update while submenu is active*/
void WarningMenu::Update() {}

/*Background update*/
void WarningMenu::FeatureUpdate() {
	if (vars.iAimAngle) {
		pPlayer.PlayerCallback(true, AimAngleCallback);
	}

	if (vars.iProximity) {
		pPlayer.PlayerCallback(true, ProximityCallback);
	}

	if (vars.iVisible) {
		pPlayer.PlayerCallback(true, VisibleCallback);
	}

	vars.iDrawIndex = 0;
}

/*Singleton*/
WarningMenu* _instance;
WarningMenu* WarningMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new WarningMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
WarningMenu::~WarningMenu() { delete _instance; }