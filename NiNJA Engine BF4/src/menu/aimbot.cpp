#include "stdafx.h"
#include "menu/handler/submenus/aimbotMenu.h"
#include "menu/handler/submenus/main/loadoutEditorMenu.h"

Aimbot pAimbot;

float DegToRad(float d) {
	return d * (M_PI / 180);
}

void Aimbot::ProcessVisibility() {
	if (!AimbotMenuVars::vars.bVisibilityCheck) return;

	auto context = pGame.GetClientGameContext();
	auto mySoldier = pGame.GetMySoldierEntity();

	if (ValidPtr(context) && ValidPtr(context->pClientPlayerManager)) {
		for (int i = 0; i < 24; i++) {
			auto player = context->pClientPlayerManager->GetPlayer(i);

			if (!ValidPtr(player)) {
				szVisible[i] = false;
				continue;
			}

			if (player == pGame.GetLocalPlayer()) {
				szVisible[i] = false;
				continue;
			}

			auto soldier = player->GetClientSoldier();

			if (!player->InVehicle()) {
				if (!ValidPtr(soldier)) {
					szVisible[i] = false;
					continue;
				}

				if (!player->IsAlive()) {
					szVisible[i] = false;
					continue;
				}
			}

			if (!ValidPtr(mySoldier)) {
				szVisible[i] = false;
				continue;
			}

			if (!ValidPtr(mySoldier->pClientPlayer)) {
				szVisible[i] = false;
				continue;
			}

			if (!mySoldier->pClientPlayer->IsAlive()) {
				szVisible[i] = false;
				continue;
			}

			bool isFriend = pPlayer.IsFriendly(player);
			if (isFriend) {
				if (!AimbotMenuVars::vars.bAimOnFriendlies) {
					szVisible[i] = false;
					continue;
				}
			} else {
				if (!AimbotMenuVars::vars.bAimOnEnemies) {
					szVisible[i] = false;
					continue;
				}
			}

			if (player->InVehicle() && !AimbotMenuVars::vars.bAimOnVehicles) {
				szVisible[i] = false;
				continue;
			}

			Vec3 bonePosition;
			if (!pPlayer.GetBonePosition(soldier, LeftEye, bonePosition)) {
				szVisible[i] = false;
				continue;
			}

			Vec3 from;
			pPlayer.GetBonePosition(mySoldier, LeftEye, from);

			RayCastHit ray;
			Vec3 _from = from;
			Vec3 _to = bonePosition;

			szVisible[i] = !Invoke::Call<bool>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwPhysicsRay), pGame.dwRaycastHandle, "ControllableFinder", &_from, &_to, &ray, (0x4 | 0x10 | 0x20 | 0x80), nullptr);
		}
	}
}

void Aimbot::ProcessAutoWall() {
	if (!AimbotMenuVars::vars.bAutoWall) return;

	auto context = pGame.GetClientGameContext();
	auto mySoldier = pGame.GetMySoldierEntity();

	if (ValidPtr(context) && ValidPtr(context->pClientPlayerManager)) {
		for (int i = 0; i < 24; i++) {
			auto player = context->pClientPlayerManager->GetPlayer(i);

			if (!ValidPtr(player)) {
				szWallbangable[i] = false;
				continue;
			}

			if (player == pGame.GetLocalPlayer()) {
				szWallbangable[i] = false;
				continue;
			}

			auto soldier = player->GetClientSoldier();

			if (!player->InVehicle()) {
				if (!ValidPtr(soldier)) {
					szWallbangable[i] = false;
					continue;
				}

				if (!player->IsAlive()) {
					szWallbangable[i] = false;
					continue;
				}
			}

			if (!ValidPtr(mySoldier)) {
				szWallbangable[i] = false;
				continue;
			}

			if (!ValidPtr(mySoldier->pClientPlayer)) {
				szWallbangable[i] = false;
				continue;
			}

			if (!mySoldier->pClientPlayer->IsAlive()) {
				szWallbangable[i] = false;
				continue;
			}

			bool isFriend = pPlayer.IsFriendly(player);
			if (isFriend) {
				if (!AimbotMenuVars::vars.bAimOnFriendlies) {
					szWallbangable[i] = false;
					continue;
				}
			} else {
				if (!AimbotMenuVars::vars.bAimOnEnemies) {
					szWallbangable[i] = false;
					continue;
				}
			}

			if (player->InVehicle() && !AimbotMenuVars::vars.bAimOnVehicles) {
				szWallbangable[i] = false;
				continue;
			}

			Vec3 bonePosition;
			if (!pPlayer.GetBonePosition(soldier, AimbotMenuVars::TargetBone[AimbotMenuVars::vars.iTargetBone].Result, bonePosition)) {
				szWallbangable[i] = false;
				continue;
			}

			bool bReturn = false;
			RayCastHit ray;

			Vec3 _from = pGame.GetMySoldierEntity()->pPredictedController->Position;
			Vec3 _to = bonePosition;
			Vec3 dir = ((_to - _from) / 1000.f);

			int wallCount = 0;

			while ((bReturn = Invoke::Call<bool>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwPhysicsRay), pGame.dwRaycastHandle, "ControllableFinder", &_from, &_to, &ray, (0x4 | 0x10 | 0x20 | 0x80), NULL))) {
				if (ray.Material.IsPenetrable() || ray.Material.IsSeeThrough()) {
					/*if (!ray.pRigidBody)
						break;*/

					wallCount++;
					if (wallCount > 5) {
						break;
					}

					_from = ray.Position;
				} else break;
			}

			szWallbangable[i] = !bReturn;
		}
	}
}

ClientSoldierEntity* Aimbot::GetTarget(TargetInformation& target) {
	ClientSoldierEntity* ret = nullptr;
	float closestDistance = FLT_MAX;

	auto context = pGame.GetClientGameContext();
	auto mySoldier = pGame.GetMySoldierEntity();

	vector<ClientSoldierEntity*> readyPlayers;

	if (ValidPtr(context) && ValidPtr(context->pClientPlayerManager)) {
		for (int i = 0; i < 24; i++) {
			auto player = context->pClientPlayerManager->GetPlayer(i);
			
			if (!ValidPtr(player)) continue;
			if (player == pGame.GetLocalPlayer()) continue;

			bool stop = false;
			bool foundList = false;

			// handle whitelist
			for (int j = 0; j < WBList.size(); j++) {
				if (!strcmp(WBList[j].szName, player->szName)) {
					foundList = true;
					if (WBList[j].bWhitelist) {
						stop = true;
					}

					break;
				}
			}

			if (stop) continue;

			auto soldier = player->GetClientSoldier();

			if (!player->InVehicle()) {
				if (!pPlayer.IsValidClientSoldier(soldier)) continue;
				if (!player->IsAlive()) continue;
			}

			if (!ValidPtr(mySoldier)) continue;
			if (!ValidPtr(mySoldier->pClientPlayer)) continue;
			if (!mySoldier->pClientPlayer->IsAlive()) continue;

			if (soldier->pHealthComponent->fHealth <= 0.1f) continue;

			if (AimbotMenuVars::vars.iIngores) {
				bool ret = false;

				auto chute = soldier->GetParachuteCompOther();
				if (ValidPtr(chute)) {
					switch (AimbotMenuVars::vars.iIngores) {
						case 4:
						case 1: { // In Air
							ret = chute->IsInAir();

							if (AimbotMenuVars::vars.iIngores != 4)
								break;
						}

						case 2: { // Falling
							ret = chute->IsFalling();

							if (AimbotMenuVars::vars.iIngores != 4)
								break;
						}

						case 3: { // Parachuting
							ret = chute->IsParachuting();
							break;
						}
					}
				}

				if (ret) {
					continue;
				}
			}

			bool isFriend = pPlayer.IsFriendly(player);
			if (isFriend) {
				if (!AimbotMenuVars::vars.bAimOnFriendlies) continue;
			} else {
				if (!AimbotMenuVars::vars.bAimOnEnemies) continue;
			}

			if (player->InVehicle() && !AimbotMenuVars::vars.bAimOnVehicles) continue;

			Vec3 bonePosition;
			if (!pPlayer.GetBonePosition(soldier, AimbotMenuVars::TargetBone[AimbotMenuVars::vars.iTargetBone].Result, bonePosition)) continue;

			if (!ValidPtr(mySoldier->pPredictedController)) continue;
			if (pUtils.GetDistance(bonePosition, mySoldier->pPredictedController->Position) > (float)AimbotMenuVars::vars.iMaxDistance && !pGame.bIsUsingRailgun) continue;

			Vec3 screenPos;
			bool onscreen = pGame.WorldToScreen(bonePosition, screenPos);

			if (!AimbotMenuVars::vars.bOffscreenTargeting)
				if (!onscreen && !pGame.bIsUsingRailgun) continue;

			if (AimbotMenuVars::vars.bLimitToFOV) {
				Vec3 end(640.f, 360.f, 0.f);
				if (pUtils.GetDistance(screenPos, end) > (AimbotMenuVars::vars.fFOVRadius + (AimbotMenuVars::vars.fFOVRadius / 4))) continue;
			}

			target.pUsingBone = AimbotMenuVars::TargetBone[AimbotMenuVars::vars.iTargetBone].pName;
			bool visible = soldier->IsVisible() || szVisible[i] || (AimbotMenuVars::vars.bAutoWall && szWallbangable[i]);
			target.bHittable = szWallbangable[i] || visible;

			if (AimbotMenuVars::vars.bVisibilityCheck) {
				if (!visible && !pGame.bIsUsingRailgun) continue;
			}

			if (WBList.size() > 0)
				readyPlayers.push_back(soldier);

			if (!ValidPtr(mySoldier->pPredictedController)) continue;
			float distance = pUtils.GetDistance(mySoldier->pPredictedController->Position, soldier->pPredictedController->Position);
			if (distance < closestDistance) {
				ret = soldier;
				target.bVisible = visible;
				target.BonePosition = bonePosition;
				closestDistance = distance;
			}
		}
	}

	// handle blacklist
	if (WBList.size() > 0) {
		for (int i = 0; i < readyPlayers.size(); i++) {
			auto soldier = readyPlayers[i];

			if (!pPlayer.IsValidClientSoldier(soldier) || pPlayer.IsValidClientPlayer(soldier->pClientPlayer)) {
				continue;
			}

			bool stop = false;
			for (int j = 0; j < WBList.size(); j++) {
				if (!strcmp(WBList[j].szName, soldier->pClientPlayer->szName)) {
					if (!WBList[j].bBlacklist) {
						stop = true;
					}
					break;
				}
			}

			if (stop) continue;

			Vec3 bonePosition;
			if (!pPlayer.GetBonePosition(soldier, AimbotMenuVars::TargetBone[AimbotMenuVars::vars.iTargetBone].Result, bonePosition)) continue;

			if (!ValidPtr(mySoldier->pPredictedController)) continue;
			float distance = pUtils.GetDistance(mySoldier->pPredictedController->Position, soldier->pPredictedController->Position);
			if (distance < closestDistance) {
				ret = soldier;
				target.bVisible = soldier->IsVisible() || szVisible[i];
				target.BonePosition = bonePosition;
				closestDistance = distance;
			}
		}
	} readyPlayers.clear();

	return ret;
}

float Aimbot::GetAngleDelta(float firstAngle, float secondAngle) {
	float difference = secondAngle - firstAngle;
	while (difference < DegToRad(-180.f)) difference += DegToRad(360.f);
	while (difference > DegToRad(180.f)) difference -= DegToRad(360.f);
	return difference;
}

Vec2 Aimbot::SmoothAimbotAngles(Vec2 pClientActiveViewAngles, Vec2 &pAimbotAngles) {
	Vec2 FinishedAngles; float smoothdiff[3];

	pAimbotAngles.x -= pClientActiveViewAngles.x;
	pAimbotAngles.y -= pClientActiveViewAngles.y;

	pAimbotAngles.x = AngleNormalize(pAimbotAngles.x);
	pAimbotAngles.y = AngleNormalize(pAimbotAngles.y);

	float fSmoothValue = abs(GetAngleDelta(pClientActiveViewAngles.y, pGame.vOldAngles.y));
	float fSmoothAmount = AimbotMenuVars::vars.fSmoothScale;

	smoothdiff[0] = pAimbotAngles.x / fSmoothAmount;
	smoothdiff[1] = pAimbotAngles.y / fSmoothAmount;

	if (fSmoothValue > 9.0f) {
		smoothdiff[1] *= (fSmoothValue * 0.2f);
	}

	FinishedAngles.x = pClientActiveViewAngles.x + smoothdiff[0];
	FinishedAngles.y = pClientActiveViewAngles.y + smoothdiff[1];

	FinishedAngles.x = AngleNormalize(FinishedAngles.x);
	FinishedAngles.y = AngleNormalize(FinishedAngles.y);
	return FinishedAngles;
}

float Aimbot::AngleNormalize(float angle) {
	while (angle < DegToRad(-180.f)) angle += DegToRad(360.f);
	while (angle > DegToRad(180.f)) angle -= DegToRad(360.f);
	return angle;
}