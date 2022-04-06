#pragma once

struct ClientGameContext;
struct SoldierWeaponUnlockAsset;
struct ClientPlayer;
struct ClientSoldierEntity;
struct ITypedObject;
struct WeaponSway;
struct DamageData;
struct Message;
struct FixedVector;
struct ClientPlayerView;

struct FixedVector {
	DWORD dwStartOfList;
	DWORD dwEndOfList;
	char _0x0008[0xC];

	DWORD get(int index) {
		return *(DWORD*)(dwStartOfList + (index * 4));
	}

	template<typename type>
	type get(int index) {
		return (type)*(DWORD*)(dwStartOfList + (index * 4));
	}

	int size() {
		return (dwEndOfList - dwStartOfList) / 4;
	}
};

struct BlazeUserManagerUser {
	void* pVtable;
	BlazeUserManagerUser* pNextUser;
	BlazeUserManagerUser* pPreviousUser;
	char _0x0000[0x14];
	__int64 AccountID;
	char _0x0028[0x10];
	char szName[0x100];
	char szBlazePersonalId[0x20];
	DWORD* pBlazeUserSessionExtendedData;
	unsigned __int64 ExternalId;
	char bottomPadding[0x50];
};

struct TextRendererDataLeadingToGFX {
	char _0x0000[0xC];
	DWORD* pGFxTextDocView;
};

struct TextRendererData {
	const char* pFontType;
	char _0x0004[0x10];
	TextRendererDataLeadingToGFX* pTextRendererDataLeadingToGFX;

	float GetWidth() {
		return (((float(*)(DWORD*))0x890B5E40)(this->pTextRendererDataLeadingToGFX->pGFxTextDocView) / 20.f) + 4.f;
	}

	float GetHeight() {
		return (((float(*)(DWORD*))0x890B5EC8)(this->pTextRendererDataLeadingToGFX->pGFxTextDocView) / 20.f) + 4.f;
	}
};

struct Timer {
	unsigned __int64 Count;
	unsigned __int64 Counts;
	long double DeltaTime;
	long double Time;
	long double InvFrequency;
};

struct AntiSpam {
	virtual DWORD getSecondsBlocked();
	virtual bool isMessageCountOk(DWORD count);
	virtual void onBlocked();
	virtual void onStillBlocked();
};

template<typename Type>
struct ScrollStruct {
	const char* pName;
	Type Result;
};

struct Deviation {
	float fPitch;
	float fYaw;
	float fRoll;
	float fTransY;
};

struct PlayEffectAsset {
	char _0x0000[8];
	const char* pName;
};

struct SpotMessage {
	DWORD dwStart; // this + 0x10
	DWORD dwEnd; // end of comp + 4
	DWORD dwStartAddress;
	DWORD dwUnk;
	DWORD dwComponents[100];
};

struct _GColor {
	union {
		struct {
			BYTE A;
			BYTE R;
			BYTE G;
			BYTE B;
		};
		DWORD dwColor;
	};

	_GColor(DWORD _Color) {
		dwColor = _Color;
	}

	_GColor() {

	}

	_GColor(BYTE Red, BYTE Green, BYTE Blue, BYTE Alpha) {
		A = Alpha;
		R = Red;
		G = Green;
		B = Blue;
	}

	static _GColor White() {
		return _GColor(255, 255, 255, 255);
	}

	static _GColor Red() {
		return _GColor(255, 0, 0, 255);
	}

	static _GColor Blue() {
		return _GColor(0, 0, 255, 255);
	}
};

struct GColor {
	float R;
	float G;
	float B;
	float _;
	float A;

	GColor() {}

	GColor(float _r, float _g, float _b, float _a = 255.f, bool doUnder = false) {
		R = _r / 255.f;
		G = _g / 255.f;
		B = _b / 255.f;
		A = _a / 255.f;
		if (!doUnder) _ = _a / 255.f;
		//_ = _a / 255.f;
	}

	GColor WithOpacity(float opacity, bool doUnder = false) {
		static GColor col;
		col = *this;
		col.A = opacity / 255.f;
		if (!doUnder) col._ = opacity / 255.f;
		return col;
	}

	GColor(unsigned int hex, bool doUnder = false) {
		R = ((hex >> 24) & 0xFF) / 255.f;
		G = ((hex >> 16) & 0xFF) / 255.f;
		B = ((hex >> 8) & 0xFF) / 255.f;
		
		if ((hex & 0xFF) != 0)
			A = (hex & 0xFF) / 255.f;
		else A = 1.0f;

		if (!doUnder) _ = A;
	}
};

struct Vec2 {
	float x, y;

	Vec2() {
		x = 0.f;
		y = 0.f;
	}

	Vec2(float _x, float _y) {
		x = _x;
		y = _y;
	}

	Vec2 operator*(float other) {
		x *= other;
		y *= other;
		return *this;
	}

	Vec2 operator*(Vec2 other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Vec2 operator+(Vec2 other) {
		Vec2 v;
		v.x = x + other.x;
		v.y = y + other.y;
		return v;
	}

	Vec2 operator-(Vec2 other) {
		return Vec2(x - other.x, y - other.y);
	}

	Vec2 operator+=(Vec2 other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vec2 operator*=(Vec2 other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}

	Vec2 operator*=(float other) {
		x *= other;
		y *= other;
		return *this;
	}

	Vec2 operator-=(Vec2 other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}
};

struct Vec3 {
	float x;
	float y;
	float z;
	float w;

	Vec3() {
		x = 0.f;
		y = 0.f;
		z = 0.f;
		w = 0.f;
	}

	Vec3(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vec3(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	Vec3 operator*(float other) {
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	Vec3 operator/(float other) {
		x /= other;
		y /= other;
		z /= other;
		return *this;
	}

	Vec3 operator*(Vec3 other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Vec3 operator+(Vec3 other) {
		Vec3 v;
		v.x = x + other.x;
		v.y = y + other.y;
		v.z = z + other.z;
		return v;
	}

	Vec3 operator-(Vec3 other) {
		Vec3 k;
		k.x = x - other.x;
		k.y = y - other.y;
		k.z = z - other.z;
		return k;
	}

	Vec3 operator+=(Vec3 other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vec3 operator*=(Vec3 other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Vec3 operator*=(float other) {
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	Vec3 operator-=(Vec3 other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	bool operator==(Vec3 const &other) {
		if (this->x == other.x) {
			if (this->y == other.y) {
				if (this->z == other.z) {
					return true;
				}
			}
		}
		return false;
	}

	float Length() {
		return sqrt(x * x + y * y + z * z);
	}

	void Normalize() {
		float _l = Length();
		this->x /= _l;
		this->y /= _l;
		this->z /= _l;
	}

	float VectorLength2() {
		return sqrt((x * x) + (z * z));
	}

	void Negate() {
		this->x = -this->x;
		this->y = -this->y;
		this->z = -this->z;
		this->w = -this->w;
	}
};

struct Vec3_ {
	float x;
	float y;
	float z;

	Vec3_() {
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}

	Vec3_(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vec3_(float _x, float _y, float _z, float _w) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vec3_ operator*(float other) {
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	Vec3_ operator*(Vec3_ other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Vec3_ operator+(Vec3_ other) {
		Vec3_ v;
		v.x = x + other.x;
		v.y = y + other.y;
		v.z = z + other.z;
		return v;
	}

	Vec3_ operator-(Vec3_ other) {
		Vec3_ k;
		k.x = x - other.x;
		k.y = y - other.y;
		k.z = z - other.z;
		return k;
	}

	Vec3_ operator+=(Vec3_ other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	Vec3_ operator*=(Vec3_ other) {
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	Vec3_ operator*=(float other) {
		x *= other;
		y *= other;
		z *= other;
		return *this;
	}

	Vec3_ operator-=(Vec3_ other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	bool operator==(Vec3_ const &other) {
		if (this->x == other.x) {
			if (this->y == other.y) {
				if (this->z == other.z) {
					return true;
				}
			}
		}
		return false;
	}

	float Length() {
		return sqrt(x * x + y * y + z * z);
	}

	void Normalize() {
		float _l = Length();
		this->x /= _l;
		this->y /= _l;
		this->z /= _l;
	}

	float VectorLength2() {
		return sqrt((x * x) + (z * z));
	}
};

struct Matrix {
	union {
		struct {
			Vec3 left;
			Vec3 up;
			Vec3 forward;
			Vec3 translation;
		};

		float m[4][4];
	};

	Matrix() {}

	float& operator () (int Row, int Col) {
		return m[Row][Col];
	}
};

struct TargetInformation {
	const char* pUsingBone;
	bool bVisible;
	bool bHittable;
	Vec3 BonePosition;

	TargetInformation() {}
};

class GString {
public:
	struct DataDesc {
		DWORD dwSize;
		volatile int nRefCount;
		char bData[1];
	};

	union {
		DataDesc* pData;
		DWORD dwHeapTypeBits;
	};

public:
	GString() {
		dwHeapTypeBits = 0x80000000;
	}

	const char* Get() {
		return (char*)(dwHeapTypeBits + 8);
	}

	void Set(const char* str) {
		((void(*)(GString*, const char*))0x89030120)(this, str);
	}
};

template<typename T>
struct GRect {
	T Left;
	T Top;
	T Right;
	T Bottom;

	GRect() {}

	GRect(T left, T top, T right, T bottom) {
		Left = left; Top = top; Right = right; Bottom = bottom;
	}
};

struct EditBlur {
	float flBlurX;
	float flBlurY;
	float flStrength;
};

struct UIRectInfo {
	char _0x0000[0x7BC];
	void* pRenderer;
};

struct EditGlow {
	float flBlurX;
	float flBlurY;
	float flStrength;
	DWORD dwColor;
	char bFlags;
};

struct EditDropShadow {
	float flBlurX;
	float flBlurY;
	float flStrength;
	DWORD dwColor;
	char bFlags;
	float flAngle;
	float flDistance;
};

union uEdits {
	EditBlur Blur;
	EditGlow Glow;
	EditDropShadow DropShadow;
};

struct Filter {
	FilterModes FilterMode;
	uEdits Edits;
};

struct GFxDrawTextManagerTextParams {
	_GColor TextColor;
	Alignment HAlignment;
	VAlignment VAlignment;
	FontStyle FontStyle;
	float flFontSize;
	GString FontName;
	bool bUnderline;
	bool bMultiline;
	bool bWordWrap;
	int nLeading;
	int nIndent;
	DWORD dwLeftMargin;
	DWORD dwRightMargin;
};

struct AxisAlignedBox {
	Vec3 Min;
	Vec3 Max;

	AxisAlignedBox() {}
};

struct LinearTransform {
	Vec3 left;
	Vec3 up;
	Vec3 forward;
	Vec3 trans;
};

struct TransformAABB {
	LinearTransform Transform;
	AxisAlignedBox AABB;
	Matrix pad;
};

struct ClientVehicleEntity {
	void* pVtable;
	// virtual: 0x58 = getAABB, 0x5c = getTransform
};

struct HealthComponent {
	char _0x0000[0x10];
	float fHealth;
	float fMaxHealth;
	char _0x0014[0x8];
	float fHealth2;
};

struct ClientSoldierPrediction {
	char _0x0000[0x10];
	Vec3 ParachuteRotation;
	Vec3 Position;
	Vec3 SurfaceVelocity;
	Vec3 Velocity;
	Vec3 GroundNormal;
	int nInAir; // 0 = in air, 2 = on ground
	int nGroundType; // changes depending on the surface you're on (water, etc)
	char _0x0068[0x4];
	int nStance1; // 0 = standing, 1 = crouching, 2 = proned
	int nStance2; // 0 = standing, 1 = crouching, 2 = proned
};

struct WeaponSuppressionData {
	char _0x0000[0x10];
	float fMinMultiplier;
	float fMaxMultiplier;
	float fMinDistance;
	float fMaxDistance;
};

struct BulletEntityData {
	// 0x80 = initial speed (float)
	// 0x84 = time to live (float)
	// 0x88 = max count (int)
	// 0xa8 = weaponsuppressiondata*
	// 0xac = name (char*)
	// 0xb4 = AntHitReactionWeaponType 
	// 0xb8 = detonate on timeout (bool)
	// 0xb9 = server projectile disabled (bool)
	// 0xf8 = gravity
	// 0x10c = fly by sound radius (float)
	// 0x110 = fly by sound speed (float)
	// 0x11c = start damage
	// 0x120 = end damage
	// 0x124 = damage falloff start distance (float)
	// 0x128 = damage falloff end distance (float)
	// 0x12c = time to arm explosion (float)
	// 0x130 = first frame travel distance (float)
	// 0x134 = has vehicle detonation (bool)
	// 0x135 = instant hit (bool)
	// 0x136 = stop trail effect on unspawn (bool)
	// 0x174 = weapon bullet name [0x40?]

	char _0x0080[0x80];
	float fInitialSpeed;
	float fTimeToLive;
	int nMaxCount;
	char _0x008C[0x1C];
	WeaponSuppressionData* pWeaponSuppressionData;
	const char* pName;
	char _0x00b0[0x4];
	AntHitReactionWeaponType HitReactionWeaponType;
	bool bDetonateOnTimeout;
	bool bServerProjectileDisabled;
	char _0x00ba[0x3E];
	float fGravity;
	char _0x00fc[0x10];
	float fFlyBySoundRadius;
	float fFlyBySoundSpeed;
	char _0x0000[0x8];
	float fStartDamage;
	float fEndDamage;
	float fDamageFalloffStartDistance;
	float fDamageFalloffEndDistance;
	float fTimeToArmExplosion;
	float fFirstFrameTravelDistance;
	bool bHasVehicleDetonation;
	bool bInstantHit;
	bool bStopTrailEffectOnUnspawn;
	char _0x0137[0x3D];
	char szBulletName[0x50];
};

struct ShotConfigData {
	Vec3 InitialPosition;
	Vec3 InitialDirection;
	Vec3 InitialSpeed;
	Vec3 InitialDirectionScaleByPitch;
	BulletEntityData* pProjectileData;
	BulletEntityData* pSecondaryProjectileData;
	char _0x0048[0x10];
	int nNumberOfBulletsPerShell;
	int nNumberOfBulletsPerShot;
	int nNumberOfBulletsPerBurst;
};

struct FiringFunctionData {
	char _0x0000[0x50];
	ShotConfigData ShotConfig;
};

struct WeaponFiringData {
	char _0x0000[0x8];
	FiringFunctionData* pFiringData;
};

struct WeaponFiring {
	char _0x0000[0x94];
	WeaponFiringData* pWeaponFiringData;
	char _0x0098[0x8];
	WeaponSway* pWeaponSway;
	eWeaponState WeaponState;
	eWeaponState LastWeaponState;
	eWeaponState NextWeaponState;
	float fSwitchCooldownTimer;
	float fAutoReplenishTime;
	float fTimeToWait;
	float fStateData;
	float fHoldReleaseMinDelay;
	float fRecoilTimer;
	float fRecoilAngleX;
	float fRecoilAngleY;
	float fRecoilAngleZ;
	float fRecoilFovAngle;
	float fPrimaryAmmoReplenishDelay;
	float fReloadTimeMultiplier;
	float fOverheatDropMultiplier;
	int nPrimaryAmmoToFill;
	DWORD* pCharacterMeleeEntity;
	int nExternalPrimaryMagazineCapacity;
	int nAmmoInClip;
	int nAmmoInReserve;
	int nNumberOfProjectilesToFire;
	bool bHasStoppedFiring;
	bool bPrimaryFireTriggeredLastFrame;
	bool bIsOverheated;
	bool bUnk;
	float fHeat;
	float fOverheatTimer;
	DWORD dwTicks;
	eFireMode FireMode;
};

struct FOVTransitionData {
	char _0x0000[8];
	FOVTransitionType TransitionType;
	float fShape;
	float fStartDelay;
	float fStartJump;
	float fEndEarly;
	bool bInvert;
};

struct ZoomLevels {
	void* pVtable;
	char _0x0004[4];
	float fFieldOfView;
	float fFieldOfViewSP;
	FOVTransitionData* FieldOfViewTransition;
	float fLookSpeedMultiplier;
	float fSprintLookSpeedMultiplier;
	float fMoveSpeedMultiplier;
	float fSwayPitchMultiplier;
	float fSwayYawMultiplier;
	float fSupportedSwayPitchMagnitude;
	float fSupportedSwayYawMagnitude;
	float fSuppressedSwayPitchMagnitude;
	float fSuppressedSwayYawMagnitude;
	float fSuppressedSwayMinFactor;
	float fTimePitchMultiplier;
	float fTimeYawMultiplier;
	float fDispersionMultiplier;
	float fDispersionRotation;
	float fRecoilMultiplier;
	float fRecoilFovMultiplier;
	float fCameraImpulseMultiplier;
	float fStartFadeToBlackAtTime;
	float fFadeToBlackDuration;
	float fStartFadeFromBlackAtTime;
	float fFadeFromBlackDuration;
	float fScreenExposureAreaScale;
	ZoomLevelActivateEventType OnActivateEventType;
	float fAttractYawStrength;
	float fAttractPitchStrength;
	bool bAllowFieldOfViewScaling;
	bool bFadeToBlackInZoomTransition;
	bool bUseFovSpecialisation;
	bool bUseWeaponMeshZoom1p;
};

struct SoldierAimingSimulationData {
	char _0x0000[0x8];
	ZoomLevels** pZoomLevels;
	char _0x000C[0x5C];
	float fRayDistance;
};

struct SoldierAimingEnvironment {
	char _0x0000[8];
	ClientSoldierEntity* pSoldier;
};

struct AimingConstraints {
	float fMinYaw;
	float fMaxYaw;
	float fMinPitch;
	float fMaxPitch;
	float fPitchOffset;
	float fYawOffset;
	float fMinYawDefault;
	float fMaxYawDefault;
};

struct AimAssist {
	void* pData;
	AimingConstraints* pAimingConstraints;
	DWORD dwLastZoomLevel;
	Vec2 Angles;
};

struct ClientSoldierAimingSimulation {
	SoldierAimingSimulationData* pData;
	SoldierAimingEnvironment* pEnvironment;
	AimAssist* pAimAssist;
	float flYaw;
	float flPitch;
	float flYawTimer;
	float flPitchTimer;
	Vec2 Sway;
	char _0x0024[0x44];
	float flZoomLevel;
	char _0x006C[0xE4];
	Vec3 RayPosition;
	int iRayMaxLen;
};

struct SoldierWeaponData {
	char _0x0000[0x74];
	WeaponFiringData* pWeaponFiringData;
};

struct WeaponModifierUnk {
	char _0x0000[0x14];
	DWORD dwUnk;
};

struct WeaponModifier {
	char _0x0000[0x24];
	WeaponModifierUnk* pUnk;
};

struct ClientWeapon {
	char _0x0000[0x10];
	WeaponModifier* pWeaponModifier;
	char _0x0014[0x1C];
	Vec3 MoveSpeed;
	Matrix ShootSpace;
	Matrix ShootSpaceIdentity;
};

struct WeaponSwayHead {
	char _0x0000[0x3C];
	WeaponSway* pWeaponSway;
};

struct ClientSoldierWeapon {
	char _0x0000[0x18];
	SoldierWeaponData* pSoldierWeaponData;
	char _0x001C[0x2418];
	SoldierWeaponUnlockAsset* pWeaponUnlockAsset;
	char _0x2438[0x9C];
	ClientSoldierAimingSimulation* pAimingSimulation;
	char _0x24D8[0xC];
	ClientWeapon* pClientWeapon; // 
	char _0x24E8[0x8];
	WeaponSwayHead* pWeaponSwayHead;
	WeaponFiring* pWeaponFiring; // 24f4
};

struct Random {
	DWORD dwValue;
	DWORD dwTableIndex;
	float flNextNormal;
	bool bNextNormalIsValid;
};

struct WeaponSway {
	char _0x0000[0xC8];
	Deviation CurrentRecoilDeviation;
	char _0x00D8[0x68];
	Deviation CurrentDispersionDeviation;
	float fTimeSinceLastShot;
	DWORD dwCameraRecoilDeviation;
	char _0x0001[24];
	float fDispersionAngle;
	float fMinDispersionAngle;
	float fCrossHairDispersionFactor;
	Deviation CurrentDispersion;
};

struct WeaponHandler {
	ClientSoldierWeapon* pWeaponList[7];
};

struct ClientSoldierWeaponsComponent {
	char _0x0000[0x70];
	Matrix WeaponTransform;
	char _0x00B0[0x460];
	WeaponHandler* pWeaponHandler;
	char _0x0514[0x100];
	eWeaponSlot ActiveSlot;
	eWeaponSlot LastActiveSlot;
	eWeaponSlot LastActiveWeaponSlot;
	char _0x0620[0x264];
	float fTimeSinceLastShot;

	ClientSoldierWeapon* GetWeaponAtIndex(int index) {
		if (!pWeaponHandler)
			return nullptr;
		if (ActiveSlot == 0xFFFFFFFF)
			return nullptr;
		return pWeaponHandler->pWeaponList[index];
	}

	ClientSoldierWeapon* GetActiveSoldierWeapon() {
		if (!pWeaponHandler)
			return nullptr;
		if (ActiveSlot == 0xFFFFFFFF)
			return nullptr;
		return pWeaponHandler->pWeaponList[ActiveSlot];
	}
};

struct QuatTransform {
	Vec3 TransAndScale;
	Vec3 Rotation;
};

struct UpdatePoseResultData {
	QuatTransform* pLocalTransform;
	QuatTransform* pWorldTransform;
	QuatTransform* pUnk1;
	QuatTransform* pUnk2;
	QuatTransform* pUnk3;
	QuatTransform* pActiveWorldTransforms;
	QuatTransform* pActiveLocalTransforms;
	int nSlot;
	int nReaderIndex;
	bool bValidTransforms;
	bool bPoseUpdateEnabled;
	bool bPoseNeeded;
};

struct ClientRagdollComponent {
	char _0x0000[0x4C];
	UpdatePoseResultData RagdollTransforms;
};

struct BreathControlHandler {
	char _0x0000[0x18];
	float fBreathControlTimer;
	float fBreathControlMultiplier;
	float fBreathControlPenaltyTimer;
	float fBreathControlPenaltyMultiplier;
	float fBreathControlActive;
	float fBreathControlInput;
	float fBreathActive;
	char _0x0034[0x4];
	float fEnabled;
};

struct SprintInputHandler {
	int nMovingState; // 1 = walking, 3 = running
	char _0x0004[0xC];
};

struct ClientSoldierReplication {
	char _0x0000[0x10];
	Vec3 ParachuteRotation;
	Vec3 Position;
	Vec3 SurfaceVelocity;
	Vec3 Velocity;
	Vec3 GroundNormal;
	int nGroundSupported;
	int nGroundMaterialIndex;
	int nState;
	int nPose; // 0 = standing, 1 = crouch, 2 = lay down
	int nChangingToPose;
	char _0x0001[0x4];
	float flGroundHeight;
	float flLadderHeightClimbed;
	float flLadderHeightLeft;
	char _0x0002[0x4];
	int nActionState;
};

struct Animatable {
	char _0x0000[0x18];
	QuatTransform* pLocalTransforms;
	QuatTransform* pWorldTransforms;
	Matrix* pRenderTransforms;
	QuatTransform* pInterpolatedLocalTransforms;
	QuatTransform* pInterpolatedWorldTransforms;
	QuatTransform* pActiveWorldTransforms;
	QuatTransform* pActiveLocalTransforms;
	int nSlot;
	int nReaderIndex;
	bool bValidTransforms;
};

struct ClientAntAnimatableComponent {
	char _0x0000[0xF4];
	Animatable* pAnimatable;
	char _0x00F8[0x4];
	DWORD dwAnimableInstanceID;
	bool bHadVisualUpdate;
};

struct ClientParachuteComponent {
	char _0x0000[0xB4];
	AimingConstraints* pAimingConstraints;
	char _0x00B8[0x48];
	DWORD dwFlags; // 0 = on_ground, 7 = in_air, 4 = falling, 5 = parachuting

	bool IsInAir() {
		return dwFlags == 7;
	}

	bool IsParachuting() {
		return dwFlags == 5;
	}

	bool IsFalling() {
		return dwFlags == 4;
	}
};

struct ClientVaultPhysics {
	char _0x0000[0x20];
	Vec3 VaultVector;
};

struct ClientVaultComponent {
	char _0x0000[0x1D0];
	float fVaultHeight;
	float fUnk[3];
	Vec3 VaultCoords;
	int iState;
};

struct ClientSpottingComponent {
	char _0x0000[0x50];
	SpotType ActiveSpotType;
};

struct ClientSoldierEntity {
	char _0x0000[0xB8];
	HealthComponent* pHealthComponent;
	char _0x00BC[0x58];
	ClientPlayer* pClientPlayer;
	ClientAntAnimatableComponent* pAnimatableComponent0;
	ClientAntAnimatableComponent* pAnimatableComponent1;
	char _0x000asa[0x204];
	ClientSoldierReplication* pReplicatedController; // 324
	ClientSoldierPrediction* pPredictedController; // 328
	char _0x0330[0x28];
	float fAuthorativeYaw; // 354
	float fAuthorativePitch;
	int iAimingEnabled;
	float fCachedPitch;
	int nClientSound;
	int iPoseType;
	BYTE bRenderFlags; // 0x36C
	char _0x0002[0x58 + 3];
	ClientSoldierWeaponsComponent* pSoldierWeaponsComponent;
	void* pBodyComponent;
	ClientRagdollComponent* pRagdollComponent;
	BreathControlHandler* pBreathControlHandler;
	char _0x03d4[0x8];
	SprintInputHandler* pSpringInputHandler;
	char _0x3dc[0x4];
	float fTimeSinceSprinted;
	bool bSprinting;
	bool bOccluded; // 3EE
	char _0x03EF[0x2FD];
	ClientParachuteComponent* pClientParachuteComponent;
	char _0x0003[0x4C];
	ClientSpottingComponent* pClientSpottingComponent;
	char _0x0004[0x9C];
	ClientVaultComponent* pClientVaultComponent;

	//ClientParachuteComponent* 0x6EC
	//ClientSpottingComponent* 0x73C
	//ClientVaultComponent* 0x7DC

	ClientParachuteComponent* GetParachuteCompOther() {
		return (ClientParachuteComponent*)*(DWORD*)((DWORD)this + 0x6DC);
	}

	bool IsVisible() {
		return !bOccluded && fTimeSinceSprinted >= 1.5f;
	}
};

struct EntryInputState {
	char _0x0000[0x100];
	float fAnalogInput[11];
	float fDownTimes[70];
	char _0x0248[32];
	__int64 dwDigitalBitFlags;
	DWORD dwCustomBitFlags;
	float fDeltaTime;
	float fTimeBehind;
	Vec2 AuthoritiveViewAngles;
	Vec2 AuthoritiveViewAngles2;
	float fAuthoritiveMovementPenalty;
	Vec3 CollisionDirection;
	Vec3 UnkVec; //2A0
	Vec3 VaultDestination;
	Vec3 VaultVelocity;
	Vec3 UnkVec2;
	DWORD idk;
	DWORD dwTicks;
	char _0x02E4[12];
	DWORD dwVaultFlags;
	char _0x02F4[52];
};

struct EntryInputStateNetworkMove {
	char _0x0000[0x10];
	EntryInputState State;
};

struct Asset {
	DWORD dwVTable;
};

struct CustomizationUnlockParts {
	char _0x0000[0x10];
	DWORD** pVisualAssets;
};

struct CustomizationTable {
	void* pVtable;
	DWORD dwRef;
	CustomizationUnlockParts** pParts;
};

struct VeniceSoldierCustomizationAsset {
	char _0x0000[0x18];
	CustomizationTable* pWeaponTable;
	Asset* pAsset[1000];
};

struct ClientPlayer {
	void* pVtable;
	char _0x0000[0x24];
	char szName[14];
	char _0x0036[0x6CA];
	VeniceSoldierCustomizationAsset* pVeniceSoldierCustomizationAsset;
	char _0x0704[0x750];
	int nTeamID; // e54
	char _0x0E58[0x90];
	ClientVehicleEntity* pAttachedControllable; // EE8
	char _0x0EEC[0x4];
	ClientSoldierEntity* pSoldierEntity; // ef0
	char _0x0EF4[4];
	EntryInputState* pInputState; // EF8
	EntryInputState* pExternalInputState; // EFC
	char _0x0F00[0x4];
	int iID;
	char _0x0F08[0x14];
	ClientPlayerView* pOwnPlayerView;
	ClientPlayerView* pPlayerView;

	// 0x720 = squadID (int)

	void SetIsAdmin(bool is) {
		if (*(DWORD*)((DWORD)this + 0xf40)) {
			*(bool*)(*(DWORD*)((DWORD)this + 0xf40) + 0x4F) = is;
		}
	}

	ClientSoldierEntity* GetClientSoldier() {
		if (ValidPtr(pSoldierEntity))
			return pSoldierEntity;
		return nullptr;
	}

	bool InVehicle() {
		return pAttachedControllable != nullptr;
	}

	ClientVehicleEntity *GetVehicleEntity() {
		if (InVehicle())
			return pAttachedControllable;
		return nullptr;
	}

	bool IsAlive() {
		return !ValidPtr((DWORD*)(this + 0xedc));
	}
};

struct ClientPlayerManager {
	char _0x0000[8];
	DWORD dwMaxPlayerCount;
	char _0x000C[0xF0];
	void* pSpectatorMap; // 0xFC
	char _0x003C[0x78];
	ClientPlayer* pLocalPlayer; // 0x178
	ClientPlayer** pPlayerMap; // 0x17C

	ClientPlayer* GetPlayer(int index) {
		return pPlayerMap[index];
	}
};

struct InputCache {
	bool bDisableCache;
	char _0x0001[0x3]; // usually all FF
	float fInputBuffer[0x7B];
};

struct BorderInputNode {
	void* pVtable;
	InputCache* pInputCache;
};

struct LevelSetup {
	char _0x0000[0x4];
	const char* pGameType;
};

struct ClientLevel {
	char _0x0000[0x98];
	const char* pLevelName;
	LevelSetup* pLevelSetup;
};

struct GameTime {
	DWORD dwTicks;
	DWORD dwTickFrequency;
};

struct ClientSettings {

};

struct Client {
	char _0x0000[0x18];
	ClientGameContext* pClientGameContext;
	ClientSettings* pClientSettings;
	char _0x00020[0x4];
	ClientPlayerManager* pClientPlayerManager;
	char _0x0028[0x10 - 4];
	GameTime* pGameTime;
	BorderInputNode* pBorderInputNode; // 0x38
	char _0x003C[0x4];
	ClientLevel* pClientLevel;
	char _0x0044[0x220];
	float fFPS;
	float fFPS2;
};

struct ChatManagerFirst {
	char _0x0000[0x3644];
	DWORD dwPtr;
};

struct DamageStream {
	DWORD* pUnk;
	DamageData* Data;
	DWORD dwLength;
	DWORD dwFirst;
	DWORD dwLast;
};

struct ClientDamageStream {
	char _0x0000[4];
	DamageStream Stream;
};

struct ClientConnection {
	char _0x0000[0x1C5C];
	ClientDamageStream* pClientDamageStream;
};

struct OnlineManager {
	char _0x0000[4];
	Client* pClient;
	char _0x0008[0x10];
	ChatManagerFirst* pChatManagerFirst;
	ClientConnection* pClientConnection;
};

struct ClientPlayerScore {
	char _0x0000[0x2A8]; // 0x144 0x2A8
	int	iRank;
	int iKills;
	int	iDeaths;
	int	iScore;
	int	iGlobalScoreOrginal;
	int	iGlobalScoreUpdated;
	int	iVeteran;
	float fTime;
};

struct ClientPlayerScoreManager {
	ClientPlayerScore* GetPlayerScore(ClientPlayer* player) {
		return ((ClientPlayerScore*(*)(ClientPlayerScoreManager*, ClientPlayer*))0x88583598)(this, player);
	}
};

struct GlobalEntMask {
	char _0x0000[0xA0];
	DWORD dwUnk;
};

struct ReaderWriterLock {
	void* pSrwLock;
	char _0x0004[0xC];
};

struct MessageListener {
	void* pVtable;
	int iRefCountListener;
};

struct CriticalSection {
	char szData[32];
};

struct MessageManagerImpl {
	ReaderWriterLock Lock;
	CriticalSection MessageQueueCS;
	CriticalSection MessageListenerCS;
	int iCurrentProcessMessageCounter;
	Message* pFirstMessage;
};

struct MessageManager {
	MessageManagerImpl* pImpl;
	bool bVTableOwner;
};

struct ClientGameContext {
	char _0x0000[0x4];
	MessageManager* pMessageManager;
	char _0x0008[0x8];
	void* pUnk;
	GlobalEntMask* pGlobalEntMask;
	char _0x0018[0x18];
	ClientPlayerManager* pClientPlayerManager; // 0x30
	OnlineManager* pOnlineManager;
	void* pGameView;
	void* pInterpolationManager;
	ClientPlayerScoreManager* pClientPlayerScoreManager;
	Client* pClient;
};

struct UIElementTextFilterGlow {
	void* pVtable;
	char _0x0004[0x4];
	float fX;
	float fY;
	GColor Color;
	char _0x0024[0xC];
	float fGlowStrength;
	char _0x0034[0x1];
	char bHideObject;
	char bFineBlur;
	char bUnkGlow;
	UIElementTextFilterGlow* pSelfPtr;
};

struct UIElementFontDefinition {
	void* pVtable;
	char _0x0004[0x4];
	const char* pFontSizeStr;
	float fScale;
	GColor Color;
	char _0x0024[0xC];
	float fLetterSpacing;
	float fRowSpacing;
	UIElementTextFilterGlow* pGlow;
};

// 'UI/Fonts/StylesCasablanca/BF-Text_Regular_18px_XS'
// 'UI/Fonts/StylesCasablanca/BF-Text_Medium_18px_XS'
// 'UI/Fonts/StylesCasablanca/BF-Text_Medium_18px_XS'
struct UIElementFontStyle {
	void* pVtable; //same
	char _0x0004[0x4];
	const char* pFont; // 0x18 - 
	UIElementFontDefinition* pFontDefinition; // 0x20
};

struct GFxDrawTextManager {
	char _0x0000[0x2a8];
	DWORD* pPtr;
};

struct UIEngine {
	char _0x0000[0x3C];
	GFxDrawTextManager* pGFxDrawTextManager;
};

struct TypeInfoData {
	const char* pName;
};

struct TypeInfo {
	TypeInfoData* pInfoData;
};

struct ITypedObject {
	virtual TypeInfo* GetType();
};

struct Message {
	void* pVtable;
	int nCategory;
	int nType;
	const char* pSenderCallstack;
	long double DispatchTime;
	Message* pNext;
	int nPostedAtProcessMessageCounter;
	bool bOwnedByMessageManager;
};

struct NetworkableMessage : Message {
	void* pServerConnection;
	void* pClientConnection;
	DWORD* dwSyncObject;
	ValidationResult ValidationResult_;
};

struct UINetworkTextInfo {
	const char* pMessage;
	float fDisplayTime;
};

struct UINetworkPlayerTextMessage : NetworkableMessage {
	UINetworkTextInfo TextInfo;
};

struct UINetworkPlayerRankUpMessage : NetworkableMessage {
	int nRank;
	bool bIsSoloRankUp;
};

struct UINetworkPlayerDeserterTextMessage : NetworkableMessage {
	DWORD dwTimeToReturn;
};

struct NetworkSelectTeamMessage : NetworkableMessage {
	int nTeamId;
};

struct NetworkSetActiveWeaponSlotMessage : NetworkableMessage {
	int nPlayerId;
	int nSlot;
};

struct UINetworkPlayerConnectMessage : NetworkableMessage {
	int nTeamId;
	const char* pName;
};

struct ClientWeaponPlayerPrimaryWeaponFireMessage : Message {
	ClientPlayer* pShooter;
};

struct SyncedBFSettings {
	char _0x0000[0x34];
	eServerType ServerType;
	bool bAllUnlocksUnlocked;
	bool bHideMinimap;
	bool bHideHUD;
	bool bNoMinimapSpotting;
	bool bNo3DSpotting;
	bool bNoNameTags;
	bool bOnlySquadLeaderSpawn;
	bool bTeamSwitchingAllowed;
	bool bRevertBackToKillerCamera;
	bool bDisableHitIndicators;
	bool bDisableVehicleCommanderActions;
	bool bIsCompetitive;
	bool bCommanderEnabled;
	bool bFieldUpgradeSystemActive;
	bool bNotifyServerWhenIgnoringDamage;
	bool bForceCrashWhenIgnoringDamage;
};

struct ClientDamageInfoExtra {
	ClientPlayer* pDamageGiver;
	SoldierWeaponUnlockAsset* pSoldierUnlockAsset;
	bool bUnk;
};

struct ClientDamageInfo {
	void* pVtable;
	char _0x0004[0xC];
	Vec3 Position;
	Vec3 Direction;
	Vec3 Origin;
	DWORD UnkVectors[3][4];
	float fDamage;
	char _0x0074[0x20];
	DWORD dwTick;
	char _0x0078[0x8];
	ClientDamageInfoExtra* pClientDamageInfoExtra;
	float fUnk;
	byte bUnk1;
	byte bUnk2;
};

struct SoldierWeaponUnlockAsset {
	char _0x0000[0x10];
	const char* pName;
	char _0x0014[0x2C];
	int nIdentifier;
	int nCategory;
};

struct DamageData {
	// 0xC = BulletEntityData*

	void* pVtable;
	char _0x0000[140];
	int iVictimID;
	int iVictimInstanceID;
	int iDamageGiverPlayerID;
	DWORD dwAntiCheatDetectedModuleBase;
	SoldierWeaponUnlockAsset* pWeaponUnlockAsset;
	char _0x00A4[0x4];
	DWORD dwAntiCheatDetectedIntrusionAddress;
	char _0x0001[0x14];
	float fDamage;
};

struct RenderView {
	char _0x0000[64];
	Matrix Transform;
	char _0x0080[52];
	float fFovY;
	float fDefualtFovY;
	float fNearPlane;
	float fFarPlane;
	float fAspect;
	float fOrthoWidth;
	float fOrthoHeight;
	char _0x00D0[384];
	float fFovX;
	float fDepthHeightRatio;
	float fFovScale;
	float fFovScaleSqr;
	Matrix ViewMatrix;
	Matrix ViewMatrixTranspose;
	Matrix ViewMatrixInverse;
	Matrix ProjectionMatrix;
	Matrix ViewMatrixAtOrigin;
	Matrix ProjectionTranspose;
	Matrix ProjectionInverse;
	Matrix ViewProjectionMatrix;
	Matrix ViewProjectionTranspose;
	Matrix ViewProjectionInverse;
	Matrix UnknMatrix;
	Matrix Transform2;
	Matrix UnknMatrix2;
};

struct GameRenderer {
	char _0x0000[0x24];
	void* pGameRenderSettings;
	char _0x0028[0x8];
	RenderView* pRenderView;
};

struct ButtonMonitor {
	char _0x0000[0x14];
	int nButton;
	char _0x0018[0x4];
	int nLastButton;
};

struct ButtonSetup {
	char _0x0000[0x34];
	ButtonMonitor* pButtonMonitor;
};

struct ServerPeer {};

struct ServerPeerManager {
	char _0x0000[0x34];
	ServerPeer* pServerPeer;
};

struct ClientPlayerKilledMessage : Message {
	ClientPlayer* pVictim;
};

struct ClientPlayerSwitchTeamMessage : Message {
	ClientPlayer* pPlayer;
};

struct NetworkChatMessage {
	wchar_t* pBuffer;
	wchar_t* pPos;
	wchar_t* pMax;
	wchar_t wszBuffer[128];
	DWORD dwPlayerID;
	unsigned __int64 Mask;
	DWORD dwChannelID;
	bool bSenderIsDead;
};

struct UIHudChatMessage : Message {
	ClientPlayer* pSender;
	NetworkChatMessage* pMessage;
};

struct UINetworkHudScoringMessage : NetworkableMessage {
	char* pDescriptionSid;
	float fAmount;
	float fActualAmount;
};

struct VeniceNetworkSetPingMessage : NetworkableMessage {
	WORD wPing;
};

class MaterialContainerPair {
public:

	enum MaterialFlag {
		MFSeeThrough = 0x1,
		MFClientDestructable = 0x40000000,
		MFPenetrable = 0x8
	};

	DWORD dwFlagsAndIndex;

	bool IsPenetrable() {
		return (bool)(dwFlagsAndIndex & MFPenetrable);
	};

	bool IsSeeThrough() {
		return (bool)(dwFlagsAndIndex & MFSeeThrough);
	};

	bool IsClientDestructable() {
		return (bool)(dwFlagsAndIndex & MFClientDestructable);
	};
};

struct RayCastHit {
	Vec3 Position;
	Vec3 Normal;
	void*  pRigidBody;
	MaterialContainerPair Material;
	DWORD dwPart;
	float fLambda;
	char _pad[200];
};

struct DataContainer {
	void* pVtable;
	void* pPpartition;
	DWORD dwRefCnt;
	unsigned __int16 m_flags;
};

struct CtrRefBase {
	DataContainer* pPtr;
};

struct NetworkPlayerSelectedWeaponMessage : NetworkableMessage {
	eWeaponSlot Slot;
	DataContainer* pSoldierWeaponUnlockAsset;
	void* pUnlockAssets;
	bool bSpawning;
	bool bUnk;
};

struct UIHudWarningMessage : Message {
	const char* pText;
};

struct ServerConnectionExitMessage : Message {
	void* pConnection;
	int iReason;
};

struct SessionPlayerLeftMessage : Message {
	DWORD dwOnlineID;
};

struct UINetworkRoundWarningMessage : NetworkableMessage {
	int iTeamID;
};

struct Compartment {
	char _0x0000[0x94];
	DWORD dwPtrToFirstList;
	DWORD dwPtrToEndList;
};

struct ResourceManager {
	DWORD dwBundleLoadInProgress;
	Compartment* pCompartments[50];
};

struct ServerSoldierSelfHealMessage : Message {
	DWORD dwPlayerID;
};

struct ClientPlayerView {
	char _0x0000[0x90];
	ClientPlayer* pOwner;
	FixedVector Spectators;
};

struct ServerCollisionGrenadeThrowMessage : Message {
	DWORD* pGrenade; // servergrenadeentity
	Vec3 ThrowPosition;
	bool bCreated;
	DWORD* pShooter; // serverplayer
};

struct ServerAdminBanPlayerMessage : Message {
	const char* pPlayerName;
	void* pOnlineID;
};