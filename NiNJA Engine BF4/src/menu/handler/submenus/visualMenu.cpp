#include "stdafx.h"
#include "visualMenu.h"
#include "visuals/visualCustomizationMenu.h"

using namespace VisualMenuVars;

namespace VisualMenuVars {
	Vars vars;

	struct GlobalNodeList {
		GlobalNodeList* pNextNode;
	};

	GlobalNodeList* GetLinkedNodeListFromTypeInfo(DWORD dwTypeInfo) {
		// decided to use a reversed version of it, going to be reversing some of the small funcs that i call to
		// speed shit up, even if it's un-noticable, it'd be nice not having tonnes of calls to functions that do 
		// a few lines of asm. 0x89916E74 is ClientGameWorld.
		
		return (GlobalNodeList*)*(DWORD*)(dwTypeInfo + ((*(DWORD*)((*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwClientGameWorld)) + 0x18) + 6) << 3));
	}

	struct ClientExplosionPackEntity {
		char _0x0000[0x2E0];
		int iTeamID;
		int iDamageGiverPlayerID;
		bool bIsSpotted;
	};

	ScrollStruct<ESPTypes> DrawSelectionType[] = {
		"Off", ESP_Off,
		"Friendly", ESP_Friendly,
		"Enemy", ESP_Enemy,
		"Both", ESP_Both
	};

	ScrollStruct<int> SnaplinePositionType[] = {
		"Bottom", 0,
		"Center", 1,
		"Top", 2
	};

	ScrollStruct<int> CrosshairType[] = {
		"Off", 0,
		"+", 1,
		"x", 2,
		"*", 3
	};

	ScrollStruct<int> BoxTypes[] = {
		"Corner", 0,
		"2D", 1,
		"2D Filled", 2,
		"3D", 3
	};

	void DrawAABB(Vec3 org, Vec3 vMin, Vec3 vMax, GColor color) {
		Vec3 _min = org + vMin;
		Vec3 _max = org + vMax;

		Vec3 crnr2 = Vec3(_max.x, _min.y, _min.z);
		Vec3 crnr3 = Vec3(_max.x, _min.y, _max.z);
		Vec3 crnr4 = Vec3(_min.x, _min.y, _max.z);
		Vec3 crnr5 = Vec3(_min.x, _max.y, _max.z);
		Vec3 crnr6 = Vec3(_min.x, _max.y, _min.z);
		Vec3 crnr7 = Vec3(_max.x, _max.y, _min.z);

		if (pGame.WorldToScreen(_min, _min) &&
			pGame.WorldToScreen(_max, _max) &&
			pGame.WorldToScreen(crnr2, crnr2) &&
			pGame.WorldToScreen(crnr3, crnr3) &&
			pGame.WorldToScreen(crnr4, crnr4) &&
			pGame.WorldToScreen(crnr5, crnr5) &&
			pGame.WorldToScreen(crnr6, crnr6) &&
			pGame.WorldToScreen(crnr7, crnr7)) {
			pRenderer.DrawLine(_min.x, _min.y, crnr2.x, crnr2.y, color, 1);
			pRenderer.DrawLine(_min.x, _min.y, crnr4.x, crnr4.y, color, 1);
			pRenderer.DrawLine(_min.x, _min.y, crnr6.x, crnr6.y, color, 1);
			pRenderer.DrawLine(_max.x, _max.y, crnr5.x, crnr5.y, color, 1);
			pRenderer.DrawLine(_max.x, _max.y, crnr7.x, crnr7.y, color, 1);
			pRenderer.DrawLine(_max.x, _max.y, crnr3.x, crnr3.y, color, 1);
			pRenderer.DrawLine(crnr2.x, crnr2.y, crnr7.x, crnr7.y, color, 1);
			pRenderer.DrawLine(crnr2.x, crnr2.y, crnr3.x, crnr3.y, color, 1);
			pRenderer.DrawLine(crnr4.x, crnr4.y, crnr5.x, crnr5.y, color, 1);
			pRenderer.DrawLine(crnr4.x, crnr4.y, crnr3.x, crnr3.y, color, 1);
			pRenderer.DrawLine(crnr6.x, crnr6.y, crnr5.x, crnr5.y, color, 1);
			pRenderer.DrawLine(crnr6.x, crnr6.y, crnr7.x, crnr7.y, color, 1);
		}
	}

	auto map = [](float minDistance, float maxDistance, float minScale, float maxScale, float value) {
		return minScale + ((maxScale - minScale) / (maxDistance - minDistance)) * (value - minDistance);
	};

	void BonesESPCallback(ClientPlayer* pEntity) {
		if (ValidPtr(pEntity)) {
			auto soldier = pEntity->GetClientSoldier();
			if (ValidPtr(soldier)) {
				switch (vars.iBones) {
					case ESP_Both:
					case ESP_Friendly: {
						if (pPlayer.IsFriendly(pEntity)) {
							GColor color;

							if (AimbotMenuVars::vars.bAimOnFriendlies) {
								bool foundWhiteList = false;
								WhiteBlackList whitelistData;
								for (int i = 0; i < pAimbot.WBList.size(); i++) {
									if (!strcmp(pAimbot.WBList[i].szName, pEntity->szName)) {
										whitelistData = pAimbot.WBList[i];
										foundWhiteList = true;
										break;
									}
								}

								if (foundWhiteList && whitelistData.bWhitelist) {
									// whitelisted
									color = vars.cWhitelisted;
								} else if (pAimbot.szWallbangable[pEntity->iID] && !soldier->IsVisible()) {
									// autowallable
									color = vars.cWallbangable;
								} else if (soldier->IsVisible() || pAimbot.szVisible[pEntity->iID]) {
									// visible
									color = vars.cVisible;
								} else  if (foundWhiteList && whitelistData.bBlacklist) {
									// preferred
									color = vars.cPreferred;
								} else {
									// if none
									color = vars.cFriendly;
								}
							} else {
								color = vars.cFriendly;
							}

							pPlayer.BoneToBone(soldier, Head, Neck, color);
							pPlayer.BoneToBone(soldier, Neck, Spine2, color);
							pPlayer.BoneToBone(soldier, Spine2, Spine1, color);
							pPlayer.BoneToBone(soldier, Spine1, Spine, color);
							pPlayer.BoneToBone(soldier, Neck, LeftShoulder, color);
							pPlayer.BoneToBone(soldier, Neck, RightShoulder, color);
							pPlayer.BoneToBone(soldier, LeftShoulder, LeftElbowRoll, color);
							pPlayer.BoneToBone(soldier, RightShoulder, RightElbowRoll, color);
							pPlayer.BoneToBone(soldier, LeftElbowRoll, LeftHand, color);
							pPlayer.BoneToBone(soldier, RightElbowRoll, RightHand, color);
							pPlayer.BoneToBone(soldier, Spine, RightKneeRoll, color);
							pPlayer.BoneToBone(soldier, Spine, LeftKneeRoll, color);
							pPlayer.BoneToBone(soldier, RightKneeRoll, RightFoot, color);
							pPlayer.BoneToBone(soldier, LeftKneeRoll, LeftFoot, color);
						}

						if (vars.iBones != ESP_Both)
							break;
					}
					case ESP_Enemy: {
						if (!pPlayer.IsFriendly(pEntity)) {
							GColor color;

							if (AimbotMenuVars::vars.bAimOnEnemies) {
								bool foundWhiteList = false;
								WhiteBlackList whitelistData;
								for (int i = 0; i < pAimbot.WBList.size(); i++) {
									if (!strcmp(pAimbot.WBList[i].szName, pEntity->szName)) {
										whitelistData = pAimbot.WBList[i];
										foundWhiteList = true;
										break;
									}
								}

								if (foundWhiteList && whitelistData.bWhitelist) {
									// whitelisted
									color = vars.cWhitelisted;
								} else if (pAimbot.szWallbangable[pEntity->iID] && !soldier->IsVisible()) {
									// autowallable
									color = vars.cWallbangable;
								} else if (soldier->IsVisible() || pAimbot.szVisible[pEntity->iID]) {
									// visible
									color = vars.cVisible;
								} else  if (foundWhiteList && whitelistData.bBlacklist) {
									// preferred
									color = vars.cPreferred;
								} else {
									// if none
									color = vars.cEnemy;
								}
							} else {
								color = vars.cEnemy;
							}

							pPlayer.BoneToBone(soldier, Head, Neck, color);
							pPlayer.BoneToBone(soldier, Neck, Spine2, color);
							pPlayer.BoneToBone(soldier, Spine2, Spine1, color);
							pPlayer.BoneToBone(soldier, Spine1, Spine, color);
							pPlayer.BoneToBone(soldier, Neck, LeftShoulder, color);
							pPlayer.BoneToBone(soldier, Neck, RightShoulder, color);
							pPlayer.BoneToBone(soldier, LeftShoulder, LeftElbowRoll, color);
							pPlayer.BoneToBone(soldier, RightShoulder, RightElbowRoll, color);
							pPlayer.BoneToBone(soldier, LeftElbowRoll, LeftHand, color);
							pPlayer.BoneToBone(soldier, RightElbowRoll, RightHand, color);
							pPlayer.BoneToBone(soldier, Spine, RightKneeRoll, color);
							pPlayer.BoneToBone(soldier, Spine, LeftKneeRoll, color);
							pPlayer.BoneToBone(soldier, RightKneeRoll, RightFoot, color);
							pPlayer.BoneToBone(soldier, LeftKneeRoll, LeftFoot, color);
						}

						break;
					}
				}
			}
		}
	}

	void SnaplineESPCallback(ClientPlayer* pEntity) {
		if (ValidPtr(pEntity)) {
			auto soldier = pEntity->GetClientSoldier();
			if (ValidPtr(soldier)) {
				switch (vars.iSnapline) {
					case ESP_Both:
					case ESP_Friendly: {
						if (pPlayer.IsFriendly(pEntity)) {
							Vec3 position, screen;
							if (pPlayer.GetBonePosition(soldier, Spine, position)) {
								if (pGame.WorldToScreen(position, screen)) {
									GColor color;

									if (AimbotMenuVars::vars.bAimOnFriendlies) {
										bool foundWhiteList = false;
										WhiteBlackList whitelistData;
										for (int i = 0; i < pAimbot.WBList.size(); i++) {
											if (!strcmp(pAimbot.WBList[i].szName, pEntity->szName)) {
												whitelistData = pAimbot.WBList[i];
												foundWhiteList = true;
												break;
											}
										}

										if (foundWhiteList && whitelistData.bWhitelist) {
											// whitelisted
											color = vars.cWhitelisted;
										} else if (pAimbot.szWallbangable[pEntity->iID] && !soldier->IsVisible()) {
											// autowallable
											color = vars.cWallbangable;
										} else if (soldier->IsVisible() || pAimbot.szVisible[pEntity->iID]) {
											// visible
											color = vars.cVisible;
										} else  if (foundWhiteList && whitelistData.bBlacklist) {
											// preferred
											color = vars.cPreferred;
										} else {
											// if none
											color = vars.cFriendly;
										}
									} else {
										color = vars.cFriendly;
									}

									switch (vars.iSnaplinePosition) {
									case 0:
										pRenderer.DrawLine(640, 720, screen.x, screen.y, color, 1);
										break;
									case 1:
										pRenderer.DrawLine(640, 360, screen.x, screen.y, color, 1);
										break;
									case 2:
										pRenderer.DrawLine(640, 0, screen.x, screen.y, color, 1);
										break;
									}
								}
							}
						}

						if (vars.iSnapline != ESP_Both)
							break;
					}
					case ESP_Enemy: {
						if (!pPlayer.IsFriendly(pEntity)) {
							Vec3 position, screen;
							if (pPlayer.GetBonePosition(soldier, Spine, position)) {
								if (pGame.WorldToScreen(position, screen)) {
									GColor color;

									if (AimbotMenuVars::vars.bAimOnEnemies) {
										bool foundWhiteList = false;
										WhiteBlackList whitelistData;
										for (int i = 0; i < pAimbot.WBList.size(); i++) {
											if (!strcmp(pAimbot.WBList[i].szName, pEntity->szName)) {
												whitelistData = pAimbot.WBList[i];
												foundWhiteList = true;
												break;
											}
										}

										if (foundWhiteList && whitelistData.bWhitelist) {
											// whitelisted
											color = vars.cWhitelisted;
										} else if (pAimbot.szWallbangable[pEntity->iID] && !soldier->IsVisible()) {
											// autowallable
											color = vars.cWallbangable;
										} else if (soldier->IsVisible() || pAimbot.szVisible[pEntity->iID]) {
											// visible
											color = vars.cVisible;
										} else  if (foundWhiteList && whitelistData.bBlacklist) {
											// preferred
											color = vars.cPreferred;
										} else {
											// if none
											color = vars.cEnemy;
										}
									} else {
										color = vars.cEnemy;
									}

									switch (vars.iSnaplinePosition) {
									case 0:
										pRenderer.DrawLine(640, 720, screen.x, screen.y, color, 1);
										break;
									case 1:
										pRenderer.DrawLine(640, 360, screen.x, screen.y, color, 1);
										break;
									case 2:
										pRenderer.DrawLine(640, 0, screen.x, screen.y, color, 1);
										break;
									}
								}
							}
						}
						break;
					}
				}
			}
		}
	}

	void BoxESPCallback(ClientPlayer* pEntity) {
		auto DrawESPLine = [] (Vec3 center, float x1, float y1, float z1, float x2, float y2, float z2, GColor color) {
			Vec3 pointPos1 = Vec3(center.x + x1, center.y + y1, center.z + z1);
			Vec3 pointPos2 = Vec3(center.x + x2, center.y + y2, center.z + z2);
			Vec3 xy1, xy2;

			if (pGame.WorldToScreen(pointPos1, xy1) && pGame.WorldToScreen(pointPos2, xy2)) {
				pRenderer.DrawLine(xy1.x, xy1.y, xy2.x, xy2.y, color, 1);
			}
		};

		if (ValidPtr(pEntity)) {
			auto soldier = pEntity->GetClientSoldier();
			if (ValidPtr(soldier)) {
				if (ValidPtr(soldier->pPredictedController)) {
					switch (vars.iBoxes) {
						case ESP_Both:
						case ESP_Friendly: {
							if (pPlayer.IsFriendly(pEntity)) {
								GColor color;

								if (AimbotMenuVars::vars.bAimOnFriendlies) {
									bool foundWhiteList = false;
									WhiteBlackList whitelistData;
									for (int i = 0; i < pAimbot.WBList.size(); i++) {
										if (!strcmp(pAimbot.WBList[i].szName, pEntity->szName)) {
											whitelistData = pAimbot.WBList[i];
											foundWhiteList = true;
											break;
										}
									}

									if (foundWhiteList && whitelistData.bWhitelist) {
										// whitelisted
										color = vars.cWhitelisted;
									} else if (pAimbot.szWallbangable[pEntity->iID] && !soldier->IsVisible()) {
										// autowallable
										color = vars.cWallbangable;
									} else if (soldier->IsVisible() || pAimbot.szVisible[pEntity->iID]) {
										// visible
										color = vars.cVisible;
									} else  if (foundWhiteList && whitelistData.bBlacklist) {
										// preferred
										color = vars.cPreferred;
									} else {
										// if none
										color = vars.cFriendly;
									}
								} else {
									color = vars.cFriendly;
								}

								Vec3 head, feet, headScreen, feetScreen;
								if (pPlayer.GetBonePosition(soldier, Head, head)
									&& pPlayer.GetBonePosition(soldier, LeftFoot, feet)) {
									if (pGame.WorldToScreen(head, headScreen)
										&& pGame.WorldToScreen(soldier->pPredictedController->Position, feetScreen)) {
										float width = fabs((headScreen.y - feetScreen.y) / 4);

										switch (vars.iBoxType) {
											case 0: { // Corner
												float height = feetScreen.y - headScreen.y;
												width = fabs((headScreen.y - feetScreen.y) / 2);

												pRenderer.DrawRect((feetScreen.x - (width / 2)), (feetScreen.y - height), (width / 4), 1, JustifyLeft, color);
												pRenderer.DrawRect((feetScreen.x - (width / 2)), feetScreen.y, (width / 4), 1, JustifyLeft, color);
												pRenderer.DrawRect((feetScreen.x - (width / 2)), (feetScreen.y - height), 1, (width / 4), JustifyLeft, color);
												pRenderer.DrawRect((feetScreen.x + (width / 2)), (feetScreen.y - height), 1, (width / 4), JustifyLeft, color);

												pRenderer.DrawRect((feetScreen.x + ((width / 2) - (width / 4))), (feetScreen.y - height), (width / 4), 1, JustifyLeft, color);
												pRenderer.DrawRect((feetScreen.x + ((width / 2) - (width / 4))), feetScreen.y, (width / 4) + 1, 1, JustifyLeft, color);
												pRenderer.DrawRect((feetScreen.x - (width / 2)), (feetScreen.y - (width / 4)), 1, (width / 4), JustifyLeft, color);
												pRenderer.DrawRect((feetScreen.x + (width / 2)), (feetScreen.y - (width / 4)), 1, (width / 4), JustifyLeft, color);
												break;
											}

											case 1: { // 2D
												pRenderer.DrawOutlinedRect(feetScreen.x - width, headScreen.y, width * 2 - 1, feetScreen.y - headScreen.y, 1, GColor(0, 0, 0, 0, false), color, true);
												break;
											}

											case 2: { // 2D Filled
												pRenderer.DrawOutlinedRect(feetScreen.x - width, headScreen.y, width * 2 - 1, feetScreen.y - headScreen.y, 1, GColor(255, 255, 255, 255, false).WithOpacity(80), color, true);
												break;
											}

											case 3: { // 3D
												TransformAABB _AABB;
												Invoke::Call<int>(*(DWORD*)(*(DWORD*)((DWORD)soldier)+0x58), soldier, &_AABB);

												DrawAABB(soldier->pPredictedController->Position, _AABB.AABB.Min, _AABB.AABB.Max, color);
												break;
											}
										}
									}
								}
							}

							if (vars.iBoxes != ESP_Both)
								break;
						}
						case ESP_Enemy: {
							if (!pPlayer.IsFriendly(pEntity)) {
								GColor color;

								if (AimbotMenuVars::vars.bAimOnEnemies) {
									bool foundWhiteList = false;
									WhiteBlackList whitelistData;
									for (int i = 0; i < pAimbot.WBList.size(); i++) {
										if (!strcmp(pAimbot.WBList[i].szName, pEntity->szName)) {
											whitelistData = pAimbot.WBList[i];
											foundWhiteList = true;
											break;
										}
									}

									if (foundWhiteList && whitelistData.bWhitelist) {
										// whitelisted
										color = vars.cWhitelisted;
									} else if (pAimbot.szWallbangable[pEntity->iID] && !soldier->IsVisible()) {
										// autowallable
										color = vars.cWallbangable;
									} else if (soldier->IsVisible() || pAimbot.szVisible[pEntity->iID]) {
										// visible
										color = vars.cVisible;
									} else  if (foundWhiteList && whitelistData.bBlacklist) {
										// preferred
										color = vars.cPreferred;
									} else {
										// if none
										color = vars.cEnemy;
									}
								} else {
									color = vars.cEnemy;
								}

								Vec3 head, feet, headScreen, feetScreen;
								if (pPlayer.GetBonePosition(soldier, Head, head)
									&& pPlayer.GetBonePosition(soldier, LeftFoot, feet)) {
									if (pGame.WorldToScreen(head, headScreen)
										&& pGame.WorldToScreen(soldier->pPredictedController->Position, feetScreen)) {
										float width = fabs((headScreen.y - feetScreen.y) / 4);

										switch (vars.iBoxType) {
											case 0: { // Corner
												float height = feetScreen.y - headScreen.y;
												width = fabs((headScreen.y - feetScreen.y) / 2);

												pRenderer.DrawRect((feetScreen.x - (width / 2)), (feetScreen.y - height), (width / 4), 1, JustifyLeft, color);
												pRenderer.DrawRect((feetScreen.x - (width / 2)), feetScreen.y, (width / 4), 1, JustifyLeft, color);
												pRenderer.DrawRect((feetScreen.x - (width / 2)), (feetScreen.y - height), 1, (width / 4), JustifyLeft, color);
												pRenderer.DrawRect((feetScreen.x + (width / 2)), (feetScreen.y - height), 1, (width / 4), JustifyLeft, color);

												pRenderer.DrawRect((feetScreen.x + ((width / 2) - (width / 4))), (feetScreen.y - height), (width / 4), 1, JustifyLeft, color);
												pRenderer.DrawRect((feetScreen.x + ((width / 2) - (width / 4))), feetScreen.y, (width / 4) + 1, 1, JustifyLeft, color);
												pRenderer.DrawRect((feetScreen.x - (width / 2)), (feetScreen.y - (width / 4)), 1, (width / 4), JustifyLeft, color);
												pRenderer.DrawRect((feetScreen.x + (width / 2)), (feetScreen.y - (width / 4)), 1, (width / 4), JustifyLeft, color);
												break;
											}

											case 1: { // 2D
												pRenderer.DrawOutlinedRect(feetScreen.x - width, headScreen.y, width * 2 - 1, feetScreen.y - headScreen.y, 1, GColor(0, 0, 0, 0, false), color, true);
												break;
											}

											case 2: { // 2D Filled
												pRenderer.DrawOutlinedRect(feetScreen.x - width, headScreen.y, width * 2 - 1, feetScreen.y - headScreen.y, 1, GColor(255, 255, 255, 255, false).WithOpacity(80), color, true);
												break;
											}

											case 3: { // 3D
												TransformAABB _AABB;
												Invoke::Call<int>(*(DWORD*)(*(DWORD*)((DWORD)soldier) + 0x58), soldier, &_AABB);

												DrawAABB(soldier->pPredictedController->Position, _AABB.AABB.Min, _AABB.AABB.Max, color);
												break;
											}
										}
									}
								}
							}

							break;
						}
					}
				}
			}
		}
	}

	void HealthDistanceESPCallback(ClientPlayer* pEntity) {
		if (ValidPtr(pEntity)) {
			auto soldier = pEntity->GetClientSoldier();
			if (ValidPtr(soldier)) {
				if (ValidPtr(soldier->pHealthComponent)) {
					if (ValidPtr(soldier->pPredictedController)) {
						Vec3 feetScreen;
						if (pGame.WorldToScreen(soldier->pPredictedController->Position, feetScreen)) {
							char renderName[250];
							strcpy(renderName, "");

							switch (vars.iHealth) {
								case ESP_Both:
								case ESP_Friendly: {
									if (pPlayer.IsFriendly(pEntity)) {
										strcat(renderName, pUtils.va("%.0f%% Health", ((soldier->pHealthComponent->fHealth * soldier->pHealthComponent->fMaxHealth) / 100)));
									}

									if (vars.iHealth != ESP_Both)
										break;
								}
								case ESP_Enemy: {
									if (!pPlayer.IsFriendly(pEntity)) {
										strcat(renderName, pUtils.va("%.0f%% Health", ((soldier->pHealthComponent->fHealth * soldier->pHealthComponent->fMaxHealth) / 100)));
									}

									break;
								}
							}

							switch (vars.iDistance) {
								case ESP_Both:
								case ESP_Friendly: {
									if (pPlayer.IsFriendly(pEntity) 
										&& ValidPtr(pGame.GetMySoldierEntity()) 
										&& ValidPtr(pGame.GetMySoldierEntity()->pPredictedController)) {
										strcat(renderName, pUtils.va("\n%.2fm", pUtils.GetDistance(pGame.GetMySoldierEntity()->pPredictedController->Position, soldier->pPredictedController->Position)));
									}

									if (vars.iDistance != ESP_Both)
										break;
								}
								case ESP_Enemy: {
									if (!pPlayer.IsFriendly(pEntity) 
										&& ValidPtr(pGame.GetMySoldierEntity()) 
										&& ValidPtr(pGame.GetMySoldierEntity()->pPredictedController)) {
										strcat(renderName, pUtils.va("\n%.2fm", pUtils.GetDistance(pGame.GetMySoldierEntity()->pPredictedController->Position, soldier->pPredictedController->Position)));
									}

									break;
								}
							}

							switch (vars.iWeapon) {
								case ESP_Both:
								case ESP_Friendly: {
									if (pPlayer.IsFriendly(pEntity)
										&& ValidPtr(soldier->pSoldierWeaponsComponent)
										&& ValidPtr(soldier->pSoldierWeaponsComponent->GetActiveSoldierWeapon())
										&& ValidPtr(soldier->pSoldierWeaponsComponent->GetActiveSoldierWeapon()->pWeaponUnlockAsset)) {
										strcat(renderName, pUtils.va("\n%s", (char*)(soldier->pSoldierWeaponsComponent->GetActiveSoldierWeapon()->pWeaponUnlockAsset->pName + 2)));
									}

									if (vars.iWeapon != ESP_Both)
										break;
								}
								case ESP_Enemy: {
									if (!pPlayer.IsFriendly(pEntity)
										&& ValidPtr(soldier->pSoldierWeaponsComponent)
										&& ValidPtr(soldier->pSoldierWeaponsComponent->GetActiveSoldierWeapon())
										&& ValidPtr(soldier->pSoldierWeaponsComponent->GetActiveSoldierWeapon()->pWeaponUnlockAsset)) {
										strcat(renderName, pUtils.va("\n%s", (char*)(soldier->pSoldierWeaponsComponent->GetActiveSoldierWeapon()->pWeaponUnlockAsset->pName + 2)));
									}

									break;
								}
							}

							if (ValidPtr(pGame.GetMySoldierEntity())
								&& ValidPtr(pGame.GetMySoldierEntity()->pPredictedController)) {
								float fontScale = map(5.f, 4000.f, 7.f, 13.f, pUtils.GetDistance(pGame.GetMySoldierEntity()->pPredictedController->Position, soldier->pPredictedController->Position));

								float xpos = 0.f;
								if (feetScreen.x >= 640.f) {
									xpos = (feetScreen.x - 640.f) * 2;
								} else {
									xpos = -((640.f - feetScreen.x) * 2);
								}

								pDrawText.DrawOptionTextBoldCenterAlign(renderName, xpos, feetScreen.y, fontScale, GColor(255, 255, 255, 255, true));
							}
						}
					}
				}
			}
		}
	}

	void ChamsESPCallback(ClientPlayer* pEntity) {
		return;
		if (ValidPtr(pEntity)) {
			auto soldier = pEntity->GetClientSoldier();
			if (ValidPtr(soldier)) {
				switch (vars.iChams) {
					case ESP_Both:
					case ESP_Friendly: {
						if (pPlayer.IsFriendly(pEntity)) {
							soldier->bRenderFlags = 0x20;
						} else {
							if (vars.iChams != ESP_Both) {
								soldier->bRenderFlags = 0x90;
							}
						}

						if (vars.iChams != ESP_Both)
							break;
					}
					case ESP_Enemy: {
						if (!pPlayer.IsFriendly(pEntity)) {
							soldier->bRenderFlags = 0x20;
						} else {
							if (vars.iChams != ESP_Both) {
								soldier->bRenderFlags = 0x90;
							}
						}

						break;
					}
				}
			}
		}
	}
	
	void NameESPCallback(ClientPlayer* pEntity) {
		if (ValidPtr(pEntity)) {
			auto soldier = pEntity->GetClientSoldier();
			if (ValidPtr(pEntity)) {
				if (ValidPtr(soldier->pPredictedController)) {
					switch (vars.iName) {
					case ESP_Both:
					case ESP_Friendly: {
						if (pPlayer.IsFriendly(pEntity) && ValidPtr(pGame.GetMySoldierEntity()) && ValidPtr(pGame.GetMySoldierEntity()->pPredictedController)) {
							Vec3 head, headScreen;
							if (pPlayer.GetBonePosition(soldier, Head, head)) {
								if (pGame.WorldToScreen(head, headScreen)) {
									float fontScale = map(5.f, 4000.f, 7.f, 13.f, pUtils.GetDistance(pGame.GetMySoldierEntity()->pPredictedController->Position, soldier->pPredictedController->Position));

									headScreen.y -= 5.f;

									float xpos = 0.f;
									if (headScreen.x >= 640.f) {
										xpos = (headScreen.x - 640.f) * 2;
									} else {
										xpos = -((640.f - headScreen.x) * 2);
									}

									pDrawText.DrawOptionTextBoldCenterAlign(pEntity->szName, xpos, headScreen.y, fontScale, GColor(255, 255, 255, 255, true));
								}
							}
						}

						if (vars.iName != ESP_Both)
							break;
					}
					case ESP_Enemy: {
						if (!pPlayer.IsFriendly(pEntity) && ValidPtr(pGame.GetMySoldierEntity()) && ValidPtr(pGame.GetMySoldierEntity()->pPredictedController)) {
							Vec3 head, headScreen;
							if (pPlayer.GetBonePosition(soldier, Head, head)) {
								if (pGame.WorldToScreen(head, headScreen)) {
									float fontScale = map(5.f, 4000.f, 7.f, 13.f, pUtils.GetDistance(pGame.GetMySoldierEntity()->pPredictedController->Position, soldier->pPredictedController->Position));

									headScreen.y -= 10.f;

									float xpos = 0.f;
									if (headScreen.x >= 640.f) {
										xpos = (headScreen.x - 640.f) * 2;
									} else {
										xpos = -((640.f - headScreen.x) * 2);
									}

									pDrawText.DrawOptionTextBoldCenterAlign(pEntity->szName, xpos, headScreen.y, fontScale, GColor(255, 255, 255, 255, true));
								}
							}
						}

						break;
					}
					}
				}
			}
		}
	}

	void OccupiedVehicleESPCallback(ClientPlayer* pEntity) {
		if (ValidPtr(pEntity)) {
			auto soldier = pEntity->GetClientSoldier();
			if (ValidPtr(soldier)) {
				auto vehicle = pEntity->GetVehicleEntity();
				if (ValidPtr(vehicle)) {
					switch (vars.iOccupiedVehicles) {
						case ESP_Both:
						case ESP_Friendly: {
							if (pPlayer.IsFriendly(pEntity)) {
								TransformAABB _AABB;
								Invoke::Call<int>(*(DWORD*)(*(DWORD*)((DWORD)vehicle) + 0x58), vehicle, &_AABB);

								Matrix _Transform;
								Invoke::Call<int>(*(DWORD*)(*(DWORD*)((DWORD)vehicle) + 0x5C), vehicle, &_Transform);

								DrawAABB(_Transform.translation, _AABB.AABB.Min, _AABB.AABB.Max, vars.cFriendly);
							}

							if (vars.iOccupiedVehicles != ESP_Both)
								break;
						}

						case ESP_Enemy: {
							if (!pPlayer.IsFriendly(pEntity)) {
								TransformAABB _AABB;
								Invoke::Call<int>(*(DWORD*)(*(DWORD*)((DWORD)vehicle) + 0x58), vehicle, &_AABB);

								Matrix _Transform;
								Invoke::Call<int>(*(DWORD*)(*(DWORD*)((DWORD)vehicle) + 0x5C), vehicle, &_Transform);

								DrawAABB(_Transform.translation, _AABB.AABB.Min, _AABB.AABB.Max, vars.cEnemy);
							}
							break;
						}
					}
				}
			}
		}
	}

	void DrawSupply() {
		auto list = (GlobalNodeList*)*(DWORD*)0x89A39F58;
		//auto list = GetLinkedNodeListFromTypeInfo(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwGlobalNodeSupply));

		if (ValidPtr(list)) {
			for (; list; list = list->pNextNode) {
				auto start = (DWORD)list - 0x20; // reversed from 8849ED20
				if (ValidPtr((void*)start)) {
					// i got this offset by bruteforcing the structure, searching for a pointer that had a vtable with
					// the specific address of the matching typeinfo. For this class, it happened to be 0x8815D408.
					auto entity = *(DWORD*)(start + 0x2F0);
					if (ValidPtr((void*)entity)) {
						TransformAABB _AABB;
						Invoke::Call<int>(*(DWORD*)(*(DWORD*)(entity)+0x58), entity, &_AABB);

						Matrix _Transform;
						Invoke::Call<int>(*(DWORD*)(*(DWORD*)(entity)+0x5C), entity, &_Transform);

						DrawAABB(_Transform.translation, _AABB.AABB.Min, _AABB.AABB.Max, GColor(255, 255, 255));
					}
				}
			}
		}
	}

	void DrawExplosives() {
		auto list = (GlobalNodeList*)*(DWORD*)0x89A39D78;
		//auto list = GetLinkedNodeListFromTypeInfo(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwGlobalNodeExplosives));
		
		if (ValidPtr(list)) {
			for (; list; list = list->pNextNode) {
				auto start = (DWORD)list - 0x20; // reversed from 8849ED20
				if (ValidPtr((void*)start)) {
					// i got this offset by bruteforcing the structure, searching for a pointer that had a vtable with
					// the specific address of the matching typeinfo. For this class, it happened to be 0x8815D0F0.
					auto entity = (ClientExplosionPackEntity*)*(DWORD*)(start + 0x2F0);
					if (ValidPtr(entity)) {
						TransformAABB _AABB;
						Invoke::Call<int>(*(DWORD*)(*(DWORD*)(entity)+0x58), entity, &_AABB);

						Matrix _Transform;
						Invoke::Call<int>(*(DWORD*)(*(DWORD*)(entity)+0x5C), entity, &_Transform);

						if (entity->iTeamID == pGame.GetLocalPlayer()->nTeamID) {
							// my teams explosive
							DrawAABB(_Transform.translation, _AABB.AABB.Min, _AABB.AABB.Max, GColor(0, 255, 0));
						} else {
							DrawAABB(_Transform.translation, _AABB.AABB.Min, _AABB.AABB.Max, GColor(255, 0, 0));
						}
					}
				}
			}
		}
	}

	void DrawGrenade() {
		auto list = (GlobalNodeList*)*(DWORD*)0x89A39E18;
		//auto list = GetLinkedNodeListFromTypeInfo(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwGlobalNodeGrenade));
		
		if (ValidPtr(list)) {
			for (; list; list = list->pNextNode) {
				// i got this by bruteforcing the structure, searching for a pointer that had a vtable with
				// the specific address of the matching typeinfo. When i got no results, i assumed it was inlined.
				// I then scanned for the vtable address, and got a hit. For this class, it happened to be 0x8815C970.
				auto start = (DWORD)list - 0x20; // reversed from 8849ED20
				if (ValidPtr((void*)start)) {
					TransformAABB _AABB;
					Invoke::Call<int>(*(DWORD*)(*(DWORD*)(start)+0x58), start, &_AABB);

					Matrix _Transform;
					Invoke::Call<int>(*(DWORD*)(*(DWORD*)(start)+0x5C), start, &_Transform);

					DrawAABB(_Transform.translation, _AABB.AABB.Min, _AABB.AABB.Max, GColor(255, 0, 0));
				}
			}
		}
	}

	void DrawPickups() {
		auto list = (GlobalNodeList*)*(DWORD*)0x89A38D68;
		//auto list = GetLinkedNodeListFromTypeInfo(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwGlobalNodePickups));
		
		if (ValidPtr(list)) {
			for (; list; list = list->pNextNode) {
				// i got this by bruteforcing the structure, searching for a pointer that had a vtable with
				// the specific address of the matching typeinfo. When i got no results, i assumed it was inlined.
				// I then scanned for the vtable address, and got a hit. For this class, it happened to be 0x88158448.
				auto start = (DWORD)list - 0x20; // reversed from 8849ED20
				if (ValidPtr((void*)start)) {
					TransformAABB _AABB;
					Invoke::Call<int>(*(DWORD*)(*(DWORD*)(start)+0x58), start, &_AABB);

					Matrix _Transform;
					Invoke::Call<int>(*(DWORD*)(*(DWORD*)(start)+0x5C), start, &_Transform);

					DrawAABB(_Transform.translation, _AABB.AABB.Min, _AABB.AABB.Max, GColor(255, 255, 255));
				}
			}
		}
	}

	void DrawEmptyVehicles() {
		auto list = (GlobalNodeList*)*(DWORD*)0x89A10B40;
		//auto list = GetLinkedNodeListFromTypeInfo(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwGlobalNodeVehicles));
		
		if (ValidPtr(list)) {
			for (; list; list = list->pNextNode) {
				// i got this by bruteforcing the structure, searching for a pointer that had a vtable with
				// the specific address of the matching typeinfo. When i got no results, i assumed it was inlined.
				// I then scanned for the vtable address, and got a hit. For this class, it happened to be 0x880CFF48.
				auto start = (DWORD)list - 0x20; // reversed from 8849ED20
				if (ValidPtr((void*)start)) {
					TransformAABB _AABB;
					Invoke::Call<int>(*(DWORD*)(*(DWORD*)(start)+0x58), start, &_AABB);

					Matrix _Transform;
					Invoke::Call<int>(*(DWORD*)(*(DWORD*)(start)+0x5C), start, &_Transform);

					DrawAABB(_Transform.translation, _AABB.AABB.Min, _AABB.AABB.Max, GColor(255, 255, 255));
				}
			}
		}
	}
}

void VisualMenu::Init() {
	SetName("Visuals");

	vars.cEnemy = GColor(255, 255, 0, 255, false);
	vars.cFriendly = GColor(0, 255, 0, 255, false);
	vars.cPreferred = GColor(255, 255, 255, 255, false);
	vars.cWallbangable = GColor(73, 0, 130, 255, false);
	vars.cWhitelisted = GColor(0, 204, 255, 255, false);
	vars.cVisible = GColor(255, 0, 0, 255, false);

	addOption(SubmenuOption("Customization")
		.addSubmenu<VisualCustomizationMenu>()
		.addTooltip("Don't like the ESP colors? Customize them \nto look however you want!"));

	addOption(ScrollOption<ESPTypes>(SCROLL, "Bones").canLoop()
		.addScroll(vars.iBones, 0, NUMOF(DrawSelectionType) - 1, DrawSelectionType)
		.addTooltip("Draws the skeletons of players."));

	addOption(ScrollOption<ESPTypes>(SCROLL, "Boxes").canLoop()
		.addScroll(vars.iBoxes, 0, NUMOF(DrawSelectionType) - 1, DrawSelectionType)
		.addTooltip("Draws boxes around players."));

	addOption(ScrollOption<int>(SCROLL, "Box Type").canLoop()
		.addScroll(vars.iBoxType, 0, NUMOF(BoxTypes) - 1, BoxTypes)
		.addTooltip("Sets the type of box to be drawn around \nplayers."));

	addOption(ScrollOption<int>(SCROLL, "Crosshair Type").canLoop()
		.addScroll(vars.iCrosshair, 0, NUMOF(CrosshairType) - 1, CrosshairType)
		.addTooltip("Sets the type of custom crosshair to be \ndrawn ingame."));

	addOption(ScrollOption<ESPTypes>(SCROLL, "Distance").canLoop()
		.addScroll(vars.iDistance, 0, NUMOF(DrawSelectionType) - 1, DrawSelectionType)
		.addTooltip("Draws the current distance between you \nand players."));

	addOption(ScrollOption<ESPTypes>(SCROLL, "Name").canLoop()
		.addScroll(vars.iName, 0, NUMOF(DrawSelectionType) - 1, DrawSelectionType)
		.addTooltip("Draws the name of players."));

	addOption(ScrollOption<ESPTypes>(SCROLL, "Weapon").canLoop()
		.addScroll(vars.iWeapon, 0, NUMOF(DrawSelectionType) - 1, DrawSelectionType)
		.addTooltip("Draws the name of a players current \nweapon."));

	addOption(ScrollOption<ESPTypes>(SCROLL, "Chams").canLoop()
		.addScroll(vars.iChams, 0, NUMOF(DrawSelectionType) - 1, DrawSelectionType)
		.addTooltip("Draws a filled in version of players."));

	addOption(ScrollOption<ESPTypes>(SCROLL, "Health").canLoop()
		.addScroll(vars.iHealth, 0, NUMOF(DrawSelectionType) - 1, DrawSelectionType)
		.addTooltip("Draws the health of players."));

	addOption(ScrollOption<ESPTypes>(SCROLL, "Occupied Vehicles").canLoop()
		.addScroll(vars.iOccupiedVehicles, 0, NUMOF(DrawSelectionType) - 1, DrawSelectionType)
		.addTooltip("Draws a box around occupied vehicles."));

	addOption(ScrollOption<ESPTypes>(SCROLL, "Snaplines").canLoop()
		.addScroll(vars.iSnapline, 0, NUMOF(DrawSelectionType) - 1, DrawSelectionType)
		.addTooltip("Draws a line between a player and the \nselected snapline position."));

	addOption(ScrollOption<int>(SCROLL, "Snapline Position").canLoop()
		.addScroll(vars.iSnaplinePosition, 0, NUMOF(SnaplinePositionType) - 1, SnaplinePositionType)
		.addTooltip("Sets the position from which our snaplines \nwill originate."));

	addOption(BreakOption("Entity"));

	addOption(ToggleOption("Supply Crates")
		.addToggle(vars.bSupplyCrates)
		.addTooltip("Draws a 3D box for supply crates."));

	addOption(ToggleOption("Explosives")
		.addToggle(vars.bExplosives)
		.addTooltip("Draws a 3D box for explosives."));

	addOption(ToggleOption("Grenades")
		.addToggle(vars.bGrenades)
		.addTooltip("Draws a 3D box for grenades."));

	addOption(ToggleOption("Pickups")
		.addToggle(vars.bPickups)
		.addTooltip("Draws a 3D box for pickups."));

	addOption(ToggleOption("Vehicles")
		.addToggle(vars.bEmptyVehicles)
		.addTooltip("Draws a 3D box for vehicles."));

	addOption(BreakOption("Aimbot"));

	addOption(ToggleOption("Show Aimbot Information")
		.addToggle(vars.bShowAimbotInfo)
		.addTooltip("Shows info on the aimed target."));
}

/*Update once when submenu is opened*/
void VisualMenu::UpdateOnce() {}

/*Update while submenu is active*/
void VisualMenu::Update() {}

/*Background update*/
void VisualMenu::FeatureUpdate() {
	if (vars.bPickups) {
		DrawPickups();
	}

	if (vars.bSupplyCrates) {
		DrawSupply();
	}

	if (vars.bExplosives) {
		DrawExplosives();
	}

	if (vars.bGrenades) {
		DrawGrenade();
	}

	if (vars.bEmptyVehicles) {
		DrawEmptyVehicles();
	}

	if (vars.iOccupiedVehicles != ESP_Off) {
		pPlayer.PlayerCallback(false, OccupiedVehicleESPCallback);
	}

	if (vars.iSnapline != ESP_Off) {
		pPlayer.PlayerCallback(true, SnaplineESPCallback);
	}

	if (vars.iBoxes != ESP_Off) {
		pPlayer.PlayerCallback(true, BoxESPCallback);
	}

	if (vars.iHealth != ESP_Off || vars.iDistance != ESP_Off || vars.iWeapon != ESP_Off) {
		pPlayer.PlayerCallback(true, HealthDistanceESPCallback);
	}

	if (vars.iChams != ESP_Off) {
		pPlayer.PlayerCallback(true, ChamsESPCallback);
	}

	if (vars.iName != ESP_Off) {
		pPlayer.PlayerCallback(true, NameESPCallback);
	}

	if (vars.iCrosshair > 0) {
		float width = 30;
		switch (vars.iCrosshair) {
		case 1: // +
			pRenderer.DrawLine(640 - (width / 2), 360, 640 + (width / 2), 360, GColor(255, 255, 255, 255, false), 2);
			pRenderer.DrawLine(640, 360 - (width / 2), 640, 360 + (width / 2), GColor(255, 255, 255, 255, false), 2);
			break;
		case 2: // x
			pRenderer.DrawLine(640 - (width / 2), 360 - (width / 2), 640 + (width / 2), 360 + (width / 2), GColor(255, 255, 255, 255, false), 2);
			pRenderer.DrawLine(640 + (width / 2), 360 - (width / 2), 640 - (width / 2), 360 + (width / 2), GColor(255, 255, 255, 255, false), 2);
			break;
		case 3: // *
			pRenderer.DrawLine(640 - (width / 2), 360, 640 + (width / 2), 360, GColor(255, 255, 255, 255, false), 2);
			pRenderer.DrawLine(640, 360 - (width / 2), 640, 360 + (width / 2), GColor(255, 255, 255, 255, false), 2);

			width /= 2;
			pRenderer.DrawLine(640 - (width / 2), 360 - (width / 2), 640 + (width / 2), 360 + (width / 2), GColor(255, 255, 255, 255, false), 2);
			pRenderer.DrawLine(640 + (width / 2), 360 - (width / 2), 640 - (width / 2), 360 + (width / 2), GColor(255, 255, 255, 255, false), 2);
			break;
		}
	}

	if (vars.iBones != ESP_Off) {
		pPlayer.PlayerCallback(true, BonesESPCallback);
	}

	if (vars.bShowAimbotInfo) {
		if (ValidPtr(pGame.pAimbotSoldier) && pPlayer.IsValidClientSoldier(pGame.pAimbotSoldier)) {
			if (ValidPtr(pGame.pAimbotSoldier) && pPlayer.IsValidClientPlayer(pGame.pAimbotSoldier->pClientPlayer)) {
				int index = 0;

				pDrawText.DrawOptionTextWithBox("Target Details:", 20.f, 160.f + (index * 24.f), 16.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150), false); index++;
				
				pDrawText.DrawOptionTextWithBox(pUtils.va("Name: %s", pGame.pAimbotSoldier->pClientPlayer->szName), 20.f, 160.f + (index * 24.f), 16.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150), false); index++;

				if ((pPlayer.IsValidClientSoldier(pGame.pAimbotSoldier) && ValidPtr(pGame.pAimbotSoldier->pSoldierWeaponsComponent))
					&& (pPlayer.IsValidClientSoldier(pGame.pAimbotSoldier) && ValidPtr(pGame.pAimbotSoldier->pSoldierWeaponsComponent->GetActiveSoldierWeapon()))
					&& (pPlayer.IsValidClientSoldier(pGame.pAimbotSoldier) && ValidPtr(pGame.pAimbotSoldier->pSoldierWeaponsComponent->GetActiveSoldierWeapon()->pWeaponUnlockAsset))) {
					pDrawText.DrawOptionTextWithBox(pUtils.va("Weapon: %s", (char*)(pGame.pAimbotSoldier->pSoldierWeaponsComponent->GetActiveSoldierWeapon()->pWeaponUnlockAsset->pName + 2)), 20.f, 160.f + (index * 24.f), 16.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150), false); index++;
				}

				if (pPlayer.IsValidClientSoldier(pGame.pAimbotSoldier) && ValidPtr(pGame.pAimbotSoldier->pPredictedController)) {
					auto coords = pGame.pAimbotSoldier->pPredictedController->Position;
					pDrawText.DrawOptionTextWithBox(pUtils.va("Position: x=%.2f y=%.2f z=%.2f", coords.x, coords.y, coords.z), 20.f, 160.f + (index * 24.f), 16.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150), false); index++;
				}

				pDrawText.DrawOptionTextWithBox(pUtils.va("Aimed Tag: %s", pGame.AimbotTargetInfo.pUsingBone), 20.f, 160.f + (index * 24.f), 16.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150), false); index++;
				pDrawText.DrawOptionTextWithBox(pUtils.va("Is Visible: %s", pGame.AimbotTargetInfo.bVisible ? "YES" : "NO"), 20.f, 160.f + (index * 24.f), 16.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150), false); index++;
				pDrawText.DrawOptionTextWithBox(pUtils.va("Is Hittable: %s", pGame.AimbotTargetInfo.bHittable ? "YES" : "NO"), 20.f, 160.f + (index * 24.f), 16.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150), false); index++;

				if (pPlayer.IsValidClientSoldier(pGame.pAimbotSoldier) && ValidPtr(pGame.pAimbotSoldier->pHealthComponent)) {
					pDrawText.DrawOptionTextWithBox(pUtils.va("Health: %.0f/%.0f", pGame.pAimbotSoldier->pHealthComponent->fHealth, pGame.pAimbotSoldier->pHealthComponent->fMaxHealth), 20.f, 160.f + (index * 24.f), 16.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150), false); index++;
				}

				if (pPlayer.IsValidClientSoldier(pGame.pAimbotSoldier) && ValidPtr(pGame.pAimbotSoldier->pPredictedController)) {
					if (pPlayer.IsValidClientSoldier(pGame.GetMySoldierEntity())) {
						if (pPlayer.IsValidClientSoldier(pGame.pAimbotSoldier) && ValidPtr(pGame.GetMySoldierEntity()->pPredictedController)) {
							pDrawText.DrawOptionTextWithBox(pUtils.va("Distance: %.2f", pUtils.GetDistance(pGame.GetMySoldierEntity()->pPredictedController->Position, pGame.pAimbotSoldier->pPredictedController->Position)), 20.f, 160.f + (index * 24.f), 16.f, GColor(255, 255, 255, 255), GColor(0, 0, 0, 150), false); index++;
						}
					}
				}

				/*auto score = Invoke::Call<ClientPlayerScore*>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwGetPlayerScore), pGame.pClientPlayerScoreManager, pGame.pAimbotSoldier->pClientPlayer);
				if (ValidPtr(score)) {
					float kd = 0.f;
					if (score->iDeaths == 0) {
						kd = score->iKills;
					} else {
						kd = score->iKills / score->iDeaths;
					}

					pDrawText.DrawOptionTextShadow(pUtils.va("KD: %.1f", pGame.pAimbotSoldier->pPredictedController->Velocity.z), 5.0f, 90.f + (index * 18.f), 15.f, GColor(255, 255, 255), GColor(0, 0, 0)); index++;
				}*/
			}
		}
	}
}

/*Singleton*/
VisualMenu* _instance;
VisualMenu* VisualMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VisualMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
VisualMenu::~VisualMenu() { delete _instance; }