#pragma once

struct GetNumberOf {
	int iShell;
	int iShot;
	int iBurst;
};

struct InstantHitRestore {
	bool bEnabled;
	bool bInstantHit[2];
	float fTimeToLive[2];
	float fInitialSpeed[3];
};

struct InstantKillRestore {
	bool bEnabled;
	float fStartDamage;
	float fEndDamage;
	float fDamageFalloffStartDistance;
	float fDamageFalloffEndDistance;
};

struct NoBreathRestore {
	bool bEnabled;
	float fEnabled;
	float fBreathControlTimer;
	float fBreathControlMultiplier;
	float fBreathControlPenaltyTimer;
	float fBreathControlPenaltyMultiplier;
	float fBreathControlActive;
	float fBreathControlInput;
	float fBreathActive;
};

struct NoSwayRestore {
	bool bEnabled;
	float fSwayPitchMultiplier;
	float fSwayYawMultiplier;
	float fDispersionMultiplier;
	float fRecoilMultiplier;
	float fRecoilFovMultiplier;
	float fCameraImpulseMultiplier;
	float fSupportedSwayPitchMagnitude;
	float fSupportedSwayYawMagnitude;
	float fSuppressedSwayPitchMagnitude;
	float fSuppressedSwayYawMagnitude;
};

struct LongerBulletLifeRestore {
	bool bEnabled;
	float fTimeToLive;
};

struct WeaponRestore {
	InstantHitRestore _InstantHitRestore;
	InstantKillRestore _InstantKillRestore;
	NoBreathRestore _NoBreathRestore;
	NoSwayRestore _NoSwayRestore[2];
	LongerBulletLifeRestore _LongerBulletLifeRestore;
};

class Weapon {
public:
	BulletEntityData* GetBulletEntityData();
	BulletEntityData** GetBulletEntityDataTwo();

	FiringFunctionData* GetFiringFunctionData();
	FiringFunctionData** GetFiringFunctionDataThree();

	GetNumberOf GetNumberOfValues();
	float GetDamagePerHit();

	void InstantHit(bool fix = false);
	void InstantKill(bool fix = false);
	void NoBulletDrop();
	void NoBreath(bool fix = false);
	void NoSway(bool fix = false);
	void LongerBulletLife(bool fix = false);
	void FastReload();
	void NoParachuteConstraints(bool fix = false);

	void SetDamagePerHit(float damage);
	void SetNumberOf(GetNumberOf what);

	map<DWORD, WeaponRestore> Restore;
};

extern Weapon pWeapon;