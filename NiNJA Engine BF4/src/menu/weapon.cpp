#include "stdafx.h"

Weapon pWeapon;

BulletEntityData** Weapon::GetBulletEntityDataTwo() {
	auto weapon = pGame.GetSoldierWeapon();

	BulletEntityData* ptr[2];

	if (ValidPtr(weapon)) {
		if (ValidPtr(weapon->pWeaponFiring)) {
			if (ValidPtr(weapon->pWeaponFiring->pWeaponFiringData)) {
				auto firingFunctionData = weapon->pWeaponFiring->pWeaponFiringData->pFiringData;
				if (ValidPtr(firingFunctionData)) {
					ptr[0] = firingFunctionData->ShotConfig.pProjectileData;
				}
			}
		}
	}

	if (ValidPtr(weapon)) {
		auto cwep = weapon->pClientWeapon;
		if (ValidPtr(cwep)) {
			auto ptrd = *(DWORD*)((DWORD)cwep + 0xC);
			if (ValidPtr((DWORD*)ptrd)) {
				auto another = *(DWORD*)(ptrd + 0x8);
				if (ValidPtr((DWORD*)another)) {
					ptr[1] = (BulletEntityData*)*(DWORD*)(another + 0x90);
				}
			}
		}
	}

	return ptr;
}

BulletEntityData* Weapon::GetBulletEntityData() {
	auto weapon = pGame.GetSoldierWeapon();

	if (ValidPtr(weapon)) {
		if (ValidPtr(weapon->pWeaponFiring)) {
			if (ValidPtr(weapon->pWeaponFiring->pWeaponFiringData)) {
				auto firingFunctionData = weapon->pWeaponFiring->pWeaponFiringData->pFiringData;
				if (ValidPtr(firingFunctionData)) {
					return firingFunctionData->ShotConfig.pProjectileData;
				}
			}
		}
	}

	return nullptr;
}

FiringFunctionData** Weapon::GetFiringFunctionDataThree() {
	FiringFunctionData* arr[3];

	auto weapon = pGame.GetSoldierWeapon();
	if (ValidPtr(weapon)) {
		if (ValidPtr(weapon->pWeaponFiring)) {
			if (ValidPtr(weapon->pWeaponFiring->pWeaponFiringData)) {
				arr[0] = weapon->pWeaponFiring->pWeaponFiringData->pFiringData;
			}
		}
	}

	if (ValidPtr(weapon)) {
		auto cwep = weapon->pClientWeapon;
		if (ValidPtr(cwep)) {
			auto ptr = *(DWORD*)((DWORD)cwep + 0xC);
			if (ValidPtr((DWORD*)ptr)) {
				auto another = *(DWORD*)(ptr + 0x8);
				if (ValidPtr((DWORD*)another)) {
					arr[1] = (FiringFunctionData*)another;
				}
			}
		}
	}

	if (ValidPtr(weapon)) {
		if (ValidPtr(weapon->pSoldierWeaponData)) {
			if (ValidPtr(weapon->pSoldierWeaponData->pWeaponFiringData)) {
				arr[2] = weapon->pSoldierWeaponData->pWeaponFiringData->pFiringData;
			}
		}
	}

	return arr;
}

FiringFunctionData* Weapon::GetFiringFunctionData() {
	auto weapon = pGame.GetSoldierWeapon();

	if (ValidPtr(weapon)) {
		auto cwep = weapon->pClientWeapon;
		if (ValidPtr(cwep)) {
			auto ptr = *(DWORD*)((DWORD)cwep + 0xC);
			if (ValidPtr((DWORD*)ptr)) {
				auto another = *(DWORD*)(ptr + 0x8);
				if (ValidPtr((DWORD*)another)) {
					return (FiringFunctionData*)another;
				}
			}
		}
	}

	return nullptr;
}

GetNumberOf Weapon::GetNumberOfValues() {
	GetNumberOf num;

	auto firing = GetFiringFunctionData();
	if (ValidPtr(firing)) {
		num.iBurst = firing->ShotConfig.nNumberOfBulletsPerBurst;
		num.iShell = firing->ShotConfig.nNumberOfBulletsPerShell;
		num.iShot = firing->ShotConfig.nNumberOfBulletsPerShot;
	}

	return num;
}

void Weapon::InstantHit(bool fix) {
	auto soldier = pGame.GetMySoldierEntity();
	if (ValidPtr(soldier)) {
		if (ValidPtr(soldier->pSoldierWeaponsComponent)) {
			if (soldier->pSoldierWeaponsComponent->ActiveSlot == M_PRIMARY || soldier->pSoldierWeaponsComponent->ActiveSlot == M_SECONDARY) {
				auto bullet = GetBulletEntityDataTwo();
				auto firing = GetFiringFunctionDataThree();

				if (ValidPtr(bullet) && ValidPtr(firing)) {
					if (!fix) {
						if (!Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.bEnabled) {
							Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.bEnabled = true;

							if (ValidPtr(bullet[0])) {
								Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.bInstantHit[0] = bullet[0]->bInstantHit;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.fTimeToLive[0] = bullet[0]->fTimeToLive;
							}

							if (ValidPtr(bullet[1])) {
								Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.fTimeToLive[1] = bullet[1]->fTimeToLive;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.bInstantHit[1] = bullet[1]->bInstantHit;
							}

							if (ValidPtr(firing[0])) {
								Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.fInitialSpeed[0] = firing[0]->ShotConfig.InitialSpeed.z;
							}

							if (ValidPtr(firing[1])) {
								Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.fInitialSpeed[1] = firing[1]->ShotConfig.InitialSpeed.z;
							}

							if (ValidPtr(firing[2])) {
								Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.fInitialSpeed[2] = firing[2]->ShotConfig.InitialSpeed.z;
							}
						}

						if (ValidPtr(bullet[0])) {
							bullet[0]->bInstantHit = true;
							bullet[0]->fTimeToLive = 20.f;
						}

						if (ValidPtr(bullet[1])) {
							bullet[1]->bInstantHit = true;
							bullet[1]->fTimeToLive = 20.f;
						}

						if (ValidPtr(firing[0])) {
							firing[0]->ShotConfig.InitialSpeed.z = 99999.0f;
						}

						if (ValidPtr(firing[1])) {
							firing[1]->ShotConfig.InitialSpeed.z = 99999.0f;
						}

						if (ValidPtr(firing[2])) {
							firing[2]->ShotConfig.InitialSpeed.z = 99999.0f;
						}
					} else {
						if (!Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.bEnabled) {
							return;
						}

						if (ValidPtr(bullet[0])) {
							bullet[0]->bInstantHit = Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.bInstantHit[0];
							bullet[0]->fTimeToLive = Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.fTimeToLive[0];
						}

						if (ValidPtr(bullet[1])) {
							bullet[1]->bInstantHit = Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.bInstantHit[1];
							bullet[1]->fTimeToLive = Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.fTimeToLive[1];
						}

						if (ValidPtr(firing[0])) {
							firing[0]->ShotConfig.InitialSpeed.z = Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.fInitialSpeed[0];
						}

						if (ValidPtr(firing[1])) {
							firing[1]->ShotConfig.InitialSpeed.z = Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.fInitialSpeed[1];
						}

						if (ValidPtr(firing[2])) {
							firing[2]->ShotConfig.InitialSpeed.z = Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantHitRestore.fInitialSpeed[2];
						}
					}
				}
			}
		}
	}
}

void Weapon::NoBulletDrop() {
	auto soldier = pGame.GetMySoldierEntity();
	if (ValidPtr(soldier)) {
		if (ValidPtr(soldier->pSoldierWeaponsComponent)) {
			if (soldier->pSoldierWeaponsComponent->ActiveSlot == M_PRIMARY || soldier->pSoldierWeaponsComponent->ActiveSlot == M_SECONDARY) {
				auto bullet = GetBulletEntityDataTwo();

				if (ValidPtr(bullet)) {
					if (!WeaponMenuVars::vars.bInstantHit) {
						if (ValidPtr(bullet[0])) {
							//bullet[0]->fGravity = -0.0001f;
						}

						if (ValidPtr(bullet[1])) {
							//bullet[1]->fGravity = -0.0001f;
						}
					}
				}
			}
		}
	}
}

void Weapon::InstantKill(bool fix) {
	auto soldier = pGame.GetMySoldierEntity();
	if (ValidPtr(soldier)) {
		if (ValidPtr(soldier->pSoldierWeaponsComponent)) {
			if (soldier->pSoldierWeaponsComponent->ActiveSlot == M_PRIMARY || soldier->pSoldierWeaponsComponent->ActiveSlot == M_SECONDARY) {
				auto bullet = GetBulletEntityData();

				if (ValidPtr(bullet)) {
					if (!fix) {
						if (!Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantKillRestore.bEnabled) {
							Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantKillRestore.bEnabled = true;
							Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantKillRestore.fStartDamage = bullet->fStartDamage;
							Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantKillRestore.fEndDamage = bullet->fEndDamage;
							Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantKillRestore.fDamageFalloffStartDistance = bullet->fDamageFalloffStartDistance;
							Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantKillRestore.fDamageFalloffEndDistance = bullet->fDamageFalloffEndDistance;
						}

						bullet->fStartDamage = WeaponMenuVars::vars.fDamagePerHit < 1000.f ? 1000.f : WeaponMenuVars::vars.fDamagePerHit;
						bullet->fEndDamage = WeaponMenuVars::vars.fDamagePerHit < 1000.f ? 1000.f : WeaponMenuVars::vars.fDamagePerHit;
						bullet->fDamageFalloffStartDistance = -0.0001f;
						bullet->fDamageFalloffEndDistance = -0.0001f;
					} else {
						if (!Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantKillRestore.bEnabled) {
							return;
						}

						bullet->fStartDamage = Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantKillRestore.fStartDamage;
						bullet->fEndDamage = Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantKillRestore.fEndDamage;
						bullet->fDamageFalloffStartDistance = Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantKillRestore.fDamageFalloffStartDistance;
						bullet->fDamageFalloffEndDistance = Restore[(DWORD)pGame.GetSoldierWeapon()]._InstantKillRestore.fDamageFalloffEndDistance;
					}
				}
			}
		}
	}
}

void Weapon::NoBreath(bool fix) {
	auto soldier = pGame.GetMySoldierEntity();

	if (ValidPtr(soldier)) {
		if (ValidPtr(soldier->pBreathControlHandler)) {
			if (!fix) {
				if (!Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.bEnabled) {
					Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.bEnabled = true;
					Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fEnabled = soldier->pBreathControlHandler->fEnabled;
					Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fBreathControlTimer = soldier->pBreathControlHandler->fBreathControlTimer;
					Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fBreathControlMultiplier = soldier->pBreathControlHandler->fBreathControlMultiplier;
					Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fBreathControlPenaltyTimer = soldier->pBreathControlHandler->fBreathControlPenaltyTimer;
					Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fBreathControlPenaltyMultiplier = soldier->pBreathControlHandler->fBreathControlPenaltyMultiplier;
					Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fBreathControlActive = soldier->pBreathControlHandler->fBreathControlActive;
					Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fBreathControlInput = soldier->pBreathControlHandler->fBreathControlInput;
					Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fBreathActive = soldier->pBreathControlHandler->fBreathActive;
				}

				soldier->pBreathControlHandler->fEnabled = 0.f;
				soldier->pBreathControlHandler->fBreathControlTimer = 0.f;
				soldier->pBreathControlHandler->fBreathControlMultiplier = 0.f;
				soldier->pBreathControlHandler->fBreathControlPenaltyTimer = 0.f;
				soldier->pBreathControlHandler->fBreathControlPenaltyMultiplier = 0.f;
				soldier->pBreathControlHandler->fBreathControlActive = 0.f;
				soldier->pBreathControlHandler->fBreathControlInput = 0.f;
				soldier->pBreathControlHandler->fBreathActive = 0.f;
			} else {
				if (!Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.bEnabled) {
					return;
				}

				soldier->pBreathControlHandler->fEnabled = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fEnabled;
				soldier->pBreathControlHandler->fBreathControlTimer = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fBreathControlTimer;
				soldier->pBreathControlHandler->fBreathControlMultiplier = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fBreathControlMultiplier;
				soldier->pBreathControlHandler->fBreathControlPenaltyTimer = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fBreathControlPenaltyTimer;
				soldier->pBreathControlHandler->fBreathControlPenaltyMultiplier = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fBreathControlPenaltyMultiplier;
				soldier->pBreathControlHandler->fBreathControlActive = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fBreathControlActive;
				soldier->pBreathControlHandler->fBreathControlInput = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fBreathControlInput;
				soldier->pBreathControlHandler->fBreathActive = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoBreathRestore.fBreathActive;
			}
		}
	}
}

void Weapon::NoSway(bool fix) {
	auto weapon = pGame.GetSoldierWeapon();

	if (ValidPtr(weapon)) {
		if (ValidPtr(weapon->pAimingSimulation)) {
			if (ValidPtr(weapon->pAimingSimulation->pData)) {
				if (ValidPtr(weapon->pAimingSimulation->pData->pZoomLevels)) {
					if (ValidPtr(weapon->pAimingSimulation->pData->pZoomLevels[0])) {
						if (!fix) {
							if (!Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].bEnabled) {
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].bEnabled = true;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fSwayPitchMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[0]->fSwayPitchMultiplier;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fSwayYawMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[0]->fSwayYawMultiplier;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fDispersionMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[0]->fDispersionMultiplier;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fRecoilMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[0]->fRecoilMultiplier;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fRecoilFovMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[0]->fRecoilFovMultiplier;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fCameraImpulseMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[0]->fCameraImpulseMultiplier;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fSupportedSwayPitchMagnitude = weapon->pAimingSimulation->pData->pZoomLevels[0]->fSupportedSwayPitchMagnitude;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fSupportedSwayYawMagnitude = weapon->pAimingSimulation->pData->pZoomLevels[0]->fSupportedSwayYawMagnitude;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fSuppressedSwayPitchMagnitude = weapon->pAimingSimulation->pData->pZoomLevels[0]->fSuppressedSwayPitchMagnitude;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fSuppressedSwayYawMagnitude = weapon->pAimingSimulation->pData->pZoomLevels[0]->fSuppressedSwayYawMagnitude;
							}

							weapon->pAimingSimulation->pData->pZoomLevels[0]->fSwayPitchMultiplier = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fSwayYawMultiplier = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fDispersionMultiplier = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fRecoilMultiplier = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fRecoilFovMultiplier = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fCameraImpulseMultiplier = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fSupportedSwayPitchMagnitude = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fSupportedSwayYawMagnitude = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fSuppressedSwayPitchMagnitude = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fSuppressedSwayYawMagnitude = 0.0f;
						} else {
							if (!Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].bEnabled) {
								return;
							}

							weapon->pAimingSimulation->pData->pZoomLevels[0]->fSwayPitchMultiplier = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fSwayPitchMultiplier;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fSwayYawMultiplier = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fSwayYawMultiplier;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fDispersionMultiplier = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fDispersionMultiplier;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fRecoilMultiplier = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fRecoilMultiplier;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fRecoilFovMultiplier = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fRecoilFovMultiplier;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fCameraImpulseMultiplier = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fCameraImpulseMultiplier;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fSupportedSwayPitchMagnitude = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fSupportedSwayPitchMagnitude;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fSupportedSwayYawMagnitude = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fSupportedSwayYawMagnitude;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fSuppressedSwayPitchMagnitude = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fSuppressedSwayPitchMagnitude;
							weapon->pAimingSimulation->pData->pZoomLevels[0]->fSuppressedSwayYawMagnitude = Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[0].fSuppressedSwayYawMagnitude;
						}
					}

					if (ValidPtr(weapon->pAimingSimulation->pData->pZoomLevels[1])) {
						if (!fix) {
							if (!Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].bEnabled) {
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].bEnabled = true;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fSwayPitchMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[1]->fSwayPitchMultiplier;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fSwayYawMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[1]->fSwayYawMultiplier;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fDispersionMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[1]->fDispersionMultiplier;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fRecoilMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[1]->fRecoilMultiplier;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fRecoilFovMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[1]->fRecoilFovMultiplier;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fCameraImpulseMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[1]->fCameraImpulseMultiplier;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fSupportedSwayPitchMagnitude = weapon->pAimingSimulation->pData->pZoomLevels[1]->fSupportedSwayPitchMagnitude;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fSupportedSwayYawMagnitude = weapon->pAimingSimulation->pData->pZoomLevels[1]->fSupportedSwayYawMagnitude;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fSuppressedSwayPitchMagnitude = weapon->pAimingSimulation->pData->pZoomLevels[1]->fSuppressedSwayPitchMagnitude;
								Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fSuppressedSwayYawMagnitude = weapon->pAimingSimulation->pData->pZoomLevels[1]->fSuppressedSwayYawMagnitude;
							}

							weapon->pAimingSimulation->pData->pZoomLevels[1]->fSwayPitchMultiplier = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[1]->fSwayYawMultiplier = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[1]->fDispersionMultiplier = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[1]->fRecoilMultiplier = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[1]->fRecoilFovMultiplier = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[1]->fCameraImpulseMultiplier = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[1]->fSupportedSwayPitchMagnitude = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[1]->fSupportedSwayYawMagnitude = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[1]->fSuppressedSwayPitchMagnitude = 0.0f;
							weapon->pAimingSimulation->pData->pZoomLevels[1]->fSuppressedSwayYawMagnitude = 0.0f;
						} else {
							if (!Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].bEnabled) {
								return;
							}

							Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fSwayPitchMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[1]->fSwayPitchMultiplier;
							Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fSwayYawMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[1]->fSwayYawMultiplier;
							Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fDispersionMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[1]->fDispersionMultiplier;
							Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fRecoilMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[1]->fRecoilMultiplier;
							Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fRecoilFovMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[1]->fRecoilFovMultiplier;
							Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fCameraImpulseMultiplier = weapon->pAimingSimulation->pData->pZoomLevels[1]->fCameraImpulseMultiplier;
							Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fSupportedSwayPitchMagnitude = weapon->pAimingSimulation->pData->pZoomLevels[1]->fSupportedSwayPitchMagnitude;
							Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fSupportedSwayYawMagnitude = weapon->pAimingSimulation->pData->pZoomLevels[1]->fSupportedSwayYawMagnitude;
							Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fSuppressedSwayPitchMagnitude = weapon->pAimingSimulation->pData->pZoomLevels[1]->fSuppressedSwayPitchMagnitude;
							Restore[(DWORD)pGame.GetSoldierWeapon()]._NoSwayRestore[1].fSuppressedSwayYawMagnitude = weapon->pAimingSimulation->pData->pZoomLevels[1]->fSuppressedSwayYawMagnitude;
						}
					}
				}
			}
		}
	}
}

void Weapon::LongerBulletLife(bool fix) {
	auto soldier = pGame.GetMySoldierEntity();
	if (ValidPtr(soldier)) {
		if (ValidPtr(soldier->pSoldierWeaponsComponent)) {
			if (soldier->pSoldierWeaponsComponent->ActiveSlot == M_PRIMARY || soldier->pSoldierWeaponsComponent->ActiveSlot == M_SECONDARY) {
				auto bullet = GetBulletEntityData();
				auto firing = GetFiringFunctionData();

				if (ValidPtr(bullet) && ValidPtr(firing)) {
					if (!fix) {
						if (!Restore[(DWORD)pGame.GetSoldierWeapon()]._LongerBulletLifeRestore.bEnabled) {
							Restore[(DWORD)pGame.GetSoldierWeapon()]._LongerBulletLifeRestore.bEnabled = true;
							Restore[(DWORD)pGame.GetSoldierWeapon()]._LongerBulletLifeRestore.fTimeToLive = bullet->fTimeToLive;
						}

						bullet->fTimeToLive = 20.f;
					} else {
						if (!Restore[(DWORD)pGame.GetSoldierWeapon()]._LongerBulletLifeRestore.bEnabled) {
							return;
						}

						bullet->fTimeToLive = Restore[(DWORD)pGame.GetSoldierWeapon()]._LongerBulletLifeRestore.fTimeToLive;
					}
				}
			}
		}
	}
}

void Weapon::NoParachuteConstraints(bool fix) {
	auto soldier = pGame.GetMySoldierEntity();
	if (pPlayer.IsValidClientSoldier(soldier)) {
		auto parachute = soldier->pClientParachuteComponent;
		if (ValidPtr(parachute)) {
			auto constraint = parachute->pAimingConstraints;
			if (ValidPtr(constraint)) {
				if (fix) {
					constraint->fMinYaw = -45.0f;
					constraint->fMaxYaw = 45.0f;
					constraint->fMinPitch = -85.0f;
					constraint->fMaxPitch = 70.0f;
					constraint->fMinYawDefault = -45.0f;
					constraint->fMaxYawDefault = 45.0f;
				} else {
					constraint->fMinYaw = -180.0f;
					constraint->fMaxYaw = 180.0f;
					constraint->fMinPitch = -180.0f;
					constraint->fMaxPitch = 180.0f;
					constraint->fMinYawDefault = -180.0f;
					constraint->fMaxYawDefault = 180.0f;
				}
			}
		}
	}
}

void Weapon::FastReload() {
	auto weapon = pGame.GetSoldierWeapon();

	if (ValidPtr(weapon)) {
		if (ValidPtr(weapon->pWeaponFiring)) { // needs to use the other weapon firing
			weapon->pWeaponFiring->fSwitchCooldownTimer = 0.f;
			weapon->pWeaponFiring->fTimeToWait = 0.f;
			weapon->pWeaponFiring->fHoldReleaseMinDelay = 0.f;
			weapon->pWeaponFiring->fReloadTimeMultiplier = 0.f;
			weapon->pWeaponFiring->fPrimaryAmmoReplenishDelay = 0.f;
		}
	}
}

float Weapon::GetDamagePerHit() {
	auto soldier = pGame.GetMySoldierEntity();
	if (ValidPtr(soldier)) {
		if (ValidPtr(soldier->pSoldierWeaponsComponent)) {
			if (soldier->pSoldierWeaponsComponent->ActiveSlot == M_PRIMARY || soldier->pSoldierWeaponsComponent->ActiveSlot == M_SECONDARY) {
				auto bullet = GetBulletEntityData();

				if (ValidPtr(bullet)) {
					return bullet->fStartDamage;
				}
			}
		}
	}

	return 0.f;
}

void Weapon::SetDamagePerHit(float damage) {
	auto soldier = pGame.GetMySoldierEntity();
	if (ValidPtr(soldier)) {
		if (ValidPtr(soldier->pSoldierWeaponsComponent)) {
			if (soldier->pSoldierWeaponsComponent->ActiveSlot == M_PRIMARY || soldier->pSoldierWeaponsComponent->ActiveSlot == M_SECONDARY) {
				auto bullet = GetBulletEntityData();

				if (ValidPtr(bullet)) {
					bullet->fStartDamage = damage;
				}
			}
		}
	}
}

void Weapon::SetNumberOf(GetNumberOf what) {
	auto firing = GetFiringFunctionData();
	if (ValidPtr(firing)) {
		firing->ShotConfig.nNumberOfBulletsPerBurst = what.iBurst;
		firing->ShotConfig.nNumberOfBulletsPerShell = what.iShell;
		firing->ShotConfig.nNumberOfBulletsPerShot = what.iShot;
	}
}