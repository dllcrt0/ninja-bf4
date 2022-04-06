#include "stdafx.h"

Player pPlayer;

void Player::Heal(ClientPlayer* pPlayer) {
	if (ValidPtr(pPlayer)) {
		if (ValidPtr(pPlayer->GetClientSoldier())) {
			if (ValidPtr(pPlayer->GetClientSoldier()->pHealthComponent)) {
				if (ValidPtr(pGame.GetSoldierWeapon())) {
					if (ValidPtr(pGame.GetSoldierWeapon()->pWeaponUnlockAsset)) {
						if (pPlayer->GetClientSoldier()->pHealthComponent->fHealth < pPlayer->GetClientSoldier()->pHealthComponent->fMaxHealth) {
							ClientDamageInfo* damage = new ClientDamageInfo();
							Invoke::Call<int>(0x88427508, damage);

							damage->pVtable = (void*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwClientDamageInfoVtable);
							damage->pClientDamageInfoExtra = (ClientDamageInfoExtra*)0x899B6F50;
							damage->pClientDamageInfoExtra->pDamageGiver = pGame.GetLocalPlayer();
							damage->pClientDamageInfoExtra->pSoldierUnlockAsset = pGame.GetSoldierWeapon()->pWeaponUnlockAsset;
							damage->pClientDamageInfoExtra->bUnk = true;

							pGame.SendClientDamageMessage(pPlayer->GetClientSoldier()->pHealthComponent, pGame.GetDamageID(), damage, 1, pPlayer, 0, -100.f);

							delete damage;
						}
					}
				}
			}
		}
	}
}

void Player::Kill(ClientPlayer* pPlayer) {
	if (pPlayer == pGame.GetLocalPlayer()) return;
	if (ValidPtr(pPlayer)) {
		if (ValidPtr(pPlayer->GetClientSoldier())) {
			if (ValidPtr(pPlayer->GetClientSoldier()->pHealthComponent)) {
				if (ValidPtr(pGame.GetSoldierWeapon())) {
					if (ValidPtr(pGame.GetSoldierWeapon()->pWeaponUnlockAsset)) {
						ClientDamageInfo* damage = new ClientDamageInfo();
						Invoke::Call<int>(0x88427508, damage);

						damage->pVtable = (void*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwClientDamageInfoVtable);
						damage->pClientDamageInfoExtra = (ClientDamageInfoExtra*)0x899B6F50;
						damage->pClientDamageInfoExtra->pDamageGiver = pGame.GetLocalPlayer();
						damage->pClientDamageInfoExtra->pSoldierUnlockAsset = pGame.GetSoldierWeapon()->pWeaponUnlockAsset;
						damage->pClientDamageInfoExtra->bUnk = true;

						pGame.SendClientDamageMessage(pPlayer->GetClientSoldier()->pHealthComponent, pGame.GetDamageID(), damage, 1, pPlayer, 0, 100.f);

						delete damage;
					}
				}
			}
		}
	}
}

void Player::Godmode(ClientPlayer* pShooter) {
	if (ValidPtr(pGame.GetLocalPlayer())) {
		if (ValidPtr(pGame.GetLocalPlayer()->GetClientSoldier())) {
			if (ValidPtr(pGame.GetLocalPlayer()->GetClientSoldier()->pHealthComponent)) {
				if (ValidPtr(pGame.GetSoldierWeapon())) {
					if (ValidPtr(pGame.GetSoldierWeapon()->pWeaponUnlockAsset)) {
						ClientDamageInfo* damage = new ClientDamageInfo();
						Invoke::Call<int>(0x88427508, damage);

						damage->pVtable = (void*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwClientDamageInfoVtable);
						damage->pClientDamageInfoExtra = (ClientDamageInfoExtra*)0x899B6F50;
						damage->pClientDamageInfoExtra->pDamageGiver = pShooter;
						damage->pClientDamageInfoExtra->pSoldierUnlockAsset = pGame.GetSoldierWeapon()->pWeaponUnlockAsset;
						damage->pClientDamageInfoExtra->bUnk = true;

						pGame.SendClientDamageMessage(pGame.GetLocalPlayer()->GetClientSoldier()->pHealthComponent, pGame.GetDamageID(), damage, 1, pGame.GetLocalPlayer(), 0, 100.f);

						delete damage;
					}
				}
			}
		}
	}
}

bool Player::IsFriendly(ClientPlayer* pPlayer) {
	if (ValidPtr(pPlayer)) {
		auto myPlayer = pGame.GetLocalPlayer();
		if (ValidPtr(myPlayer)) {
			if (*(DWORD*)pPlayer == decryptDWORD(Not(NiNJA.pPatches)->BF4.dwClientPlayerVtable)) {
				if (*(DWORD*)myPlayer == decryptDWORD(Not(NiNJA.pPatches)->BF4.dwClientPlayerVtable)) {
					return pPlayer->nTeamID == myPlayer->nTeamID;
				}
			}
		}
	}

	return false;
}

void Player::BoneToBone(ClientSoldierEntity* pEntity, BONES from, BONES to, GColor color) {
	if (!ValidPtr(pEntity))
		return;

	Vec3 bonePos1, bonePos2;
	Vec3 boneScreenPos1, boneScreenPos2;

	if (!GetBonePosition(pEntity, from, bonePos1))
		return;

	if (!GetBonePosition(pEntity, to, bonePos2))
		return;

	if (!pGame.WorldToScreen(bonePos1, boneScreenPos1))
		return;

	if (!pGame.WorldToScreen(bonePos2, boneScreenPos2))
		return;

	pRenderer.DrawLine(boneScreenPos1.x, boneScreenPos1.y, boneScreenPos2.x, boneScreenPos2.y, color, 1);
}

bool Player::GetBonePosition(ClientSoldierEntity* pSoldier, BONES bone, Vec3& out) {
	if (ValidPtr(pSoldier)) {
		if (ValidPtr(pSoldier->pAnimatableComponent1)) {
			if (ValidPtr(pSoldier->pAnimatableComponent1->pAnimatable)) {
				pSoldier->pAnimatableComponent1->bHadVisualUpdate = true;
				if (pSoldier->pAnimatableComponent1->pAnimatable->bValidTransforms) {
					if (ValidPtr(pSoldier->pAnimatableComponent1->pAnimatable->pActiveWorldTransforms)) {
						out = pSoldier->pAnimatableComponent1->pAnimatable->pActiveWorldTransforms[bone].TransAndScale;
						return true;
					}
				}
			}
		}
	}

	return false;
}

ClientPlayer* Player::GetClientPlayerFromID(int iID) {
	auto context = pGame.GetClientGameContext();

	if (ValidPtr(context) && ValidPtr(context->pClientPlayerManager)) {
		for (int i = 0; i < 24; i++) {
			auto player = context->pClientPlayerManager->GetPlayer(i);
			if (!ValidPtr(player)) continue;

			if (player->iID == iID)
				return player;
		}
	}

	return nullptr;
}

void Player::PlayerCallback(bool excludeMe, void(*func)(ClientPlayer*), bool bypassHealth, bool onlyClientPlayer) {
	if (!pGame.bCanRun) {
		return;
	}

	auto localPlayer = pGame.GetLocalPlayer();

	if (ValidPtr(localPlayer)) {
		auto mySoldier = localPlayer->GetClientSoldier();
		auto context = pGame.GetClientGameContext();

		if (ValidPtr(context) && ValidPtr(context->pClientPlayerManager)) {
			for (int i = 0; i < 24; i++) {
				if (!ValidPtr(localPlayer->GetClientSoldier()) && !ValidPtr(localPlayer->GetVehicleEntity())) continue;
				if (!ValidPtr(localPlayer->GetClientSoldier()->pSoldierWeaponsComponent) && !ValidPtr(localPlayer->GetVehicleEntity())) continue;

				auto player = context->pClientPlayerManager->GetPlayer(i);
				if (!ValidPtr(player)) continue;

				if (onlyClientPlayer) {
					if (ValidPtr(player->szName)) {
						func(player);
						continue;
					}
				}

				if (player == localPlayer && excludeMe) continue;
				auto soldier = player->GetClientSoldier();

				if (!player->InVehicle()) {
					if (!ValidPtr(soldier)) continue;

					if (bypassHealth) {
						func(player);
						continue;
					}

					if (!ValidPtr(soldier->pPredictedController)) continue;
					if (!ValidPtr(soldier->pHealthComponent)) continue;
					if (!player->IsAlive()) continue;
				}

				func(player);
			}
		}
	}
}

bool Player::IsValidClientPlayer(ClientPlayer* pPlayer) {
	if (ValidPtr(pPlayer)) {
		if (*(DWORD*)((DWORD)pPlayer) == decryptDWORD(Not(NiNJA.pPatches)->BF4.dwClientPlayerVtable)) {
			if (ValidPtr(pPlayer->pSoldierEntity)) {
				return true;
			}
		}
	}

	return false;
}

bool Player::IsValidClientSoldier(ClientSoldierEntity* pEntity) {
	if (ValidPtr(pEntity)) {
		if (*(DWORD*)((DWORD)pEntity) == decryptDWORD(Not(NiNJA.pPatches)->BF4.dwClientSoldierEntityVtable)) {
			if (ValidPtr(pEntity->pClientPlayer)) {
				if (ValidPtr(pEntity->pHealthComponent)) {
					return true;
				}
			}
		}
	}

	return false;
}