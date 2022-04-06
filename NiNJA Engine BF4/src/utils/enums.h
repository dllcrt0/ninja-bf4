#pragma once

enum SpotType {
	SpotType_None = 0,
	SpotType_Active = 1,
	SpotType_Passive = 2,
	SpotType_Radar = 3,
	SpotType_Unspottable = 4,
	SpotType_Auto = 5,
	SpotType_Count = 6
};

enum eServerType {
	ServerType_Official,
	ServerType_Ranked,
	ServerType_Unranked,
	ServerType_Private,
};

enum UIPopupType {
	PopupType_MemCard = 0,
	PopupType_ControllerDisconnected = 1,
	PopupType_Login = 2,
	PopupType_TermsOfService = 3,
	PopupType_PrivacyPolicy = 4,
	PopupType_RevivePrompt = 5,
	PopupType_Generic = 6,
	PopupType_ServerReminder = 7,
	PopupType_Error = 8,
	PopupType_ManagePlayer = 9,
	PopupType_Invalid = 10,
	PopupType_IngamePlayer = 11,
	PopupType_Origin = 12,
	PopupType_UpdateAvailable = 13,
	PopupType_Dynamic = 14,
	PopupType_GameQueue = 15,
	PopupType_COOPMatchmaking = 16,
	PopupType_PromoInfo = 17,
	PopupType_Store = 18,
	PopupType_ItemRedeemed = 19,
	PopupType_BattlelogNotification = 20,
	PopupType_LicenseAdded = 21,
	PopupType_StatsMigrationPrompt = 22,
	PopupType_StatsMigrationConfirmAccept = 23,
	PopupType_StatsMigrationConfirmDecline = 24,
	PopupType_StatsMigrationEndFlow = 25,
	PopupType_PlayStationPlus = 26
};

enum FontStyle {
	FS_BelowNormal,
	FS_Normal, // bold
	FS_AboveNormal // italic
};

enum ESPTypes {
	ESP_Off,
	ESP_Friendly,
	ESP_Enemy,
	ESP_Both
};

enum RayCastFlags {
	NoCheck = 0,
	CheckDetailMesh = 0x0DB,
	IsAsyncRaycast = 0x2,
	DontCheckWater = 0x0FD,
	DontCheckTerrain = 0x07A,
	DontCheckRagdoll = 0x10,
	DontCheckCharacter = 0x20,
	DontCheckGroup = 0x40,
	DontCheckPhantoms = 0x80,
};

enum HitReactionType {
	HRT_Body = 0,
	HRT_Head = 1,
	HRT_RightArm = 2,
	HRT_LeftArm = 3,
	HRT_RightLeg = 4,
	HRT_LeftLeg = 5,
	HRT_Special1 = 6,
	HRT_Special2 = 7,
	HRT_Special3 = 8,
	HRT_Special4 = 9,
	HRT_Special5 = 10,
	HRT_Special6 = 11,
	HRT_Special7 = 12,
	HRT_Special8 = 13,
	HRT_Count = 14
};

enum FilterModes {
	FM_Blur = 1,
	FM_Shadow = 2,
	FM_Highlight = 4,
	FM_Knockout = 0x100,
	FM_Inner = 0x200,
	FM_HideObject = 0x400,
	FM_UserModes = 0xFFFF,
	FM_SkipLastPass = 0x10000,
	FM_LastPassOnly = 0x20000
};

enum ScrollOptionType {
	SCROLL,
	SELECT,
	TOGGLE,
	SCROLLSELECT
};

enum Justification {
	JustifyLeft,
	JustifyCenter,
	JustifyRight
};

enum ChatChannelType {
	CctSayAll = 0,
	CctTeam = 1,
	CctSquad = 2,
	CctSquadLeader = 3,
	CctAdmin = 4,
	CctGame = 5,
	CctSize = 6
};

enum Alignment {
	HorizontalLeftAlign,
	HorizontalRightAlign,
	HorizontalCenterAlign
};

enum VAlignment {
	VerticalNoAlign,
	VerticalTopAlign,
	VerticalBottomAlign,
	VerticalCenterAlign
};

enum FOVTransitionType {
	FOVTransitionType_Linear = 0,
	FOVTransitionType_Smooth = 1,
	FOVTransitionType_Count = 2,
};

enum ZoomLevelActivateEventType {
	ZoomLevelActivateEventType_Disable = 0,
	ZoomLevelActivateEventType_Enable = 1,
	ZoomLevelActivateEventType_ToggleOnLightSwitch = 2,
};

enum eWeaponSlot {
	M_PRIMARY = 0,
	M_SECONDARY = 1,
	M_GADGET = 2,
	M_GADGET_2 = 5,
	M_GRENADE = 6,
	M_KNIFE = 7
};

enum AntHitReactionWeaponType {
	AntHitReactionWeaponType_Pistol = 0,
	AntHitReactionWeaponType_SMG = 1,
	AntHitReactionWeaponType_AssaultRifle = 2,
	AntHitReactionWeaponType_LMG = 3,
	AntHitReactionWeaponType_Shotgun = 4,
	AntHitReactionWeaponType_SniperRifle = 5,
	AntHitReactionWeaponType_Explosion = 6,
	AntHitReactionWeaponType_Flashbang = 7,
	AntHitReactionWeaponType_Melee = 8
};

enum ValidationResult {
	ValidationResult_Success = 0x0,
	ValidationResult_FailDiscard = 0x1,
	ValidationResult_FailDisconnect = 0x2,
};

enum BONES {
	Reference = 0,
	AITrajectory = 1,
	Trajectory = 2,
	TrajectoryEnd = 3,
	Hips = 4,
	Spine = 5,
	Spine1 = 6,
	Spine2 = 7,
	LeftShoulder = 8,
	LeftArm = 9,
	LeftArmRoll = 10,
	LeftForeArm = 11,
	LeftForeArmRoll = 12,
	LeftForeArmRoll1 = 13,
	LeftHand = 14,
	LeftHandMiddle0 = 15,
	LeftHandMiddle1 = 16,
	LeftHandMiddle2 = 17,
	LeftHandMiddle3 = 18,
	LeftHandMiddle4 = 19,
	LeftHandRing0 = 20,
	LeftHandRing1 = 21,
	LeftHandRing2 = 22,
	LeftHandRing3 = 23,
	LeftHandRing4 = 24,
	LeftHandPinky0 = 25,
	LeftHandPinky1 = 26,
	LeftHandPinky2 = 27,
	LeftHandPinky3 = 28,
	LeftHandPinky4 = 29,
	LeftHandIndex0 = 30,
	LeftHandIndex1 = 31,
	LeftHandIndex2 = 32,
	LeftHandIndex3 = 33,
	LeftHandIndex4 = 34,
	LeftHandThumb1 = 35,
	LeftHandThumb2 = 36,
	LeftHandThumb3 = 37,
	LeftHandThumb4 = 38,
	LeftHandProp1 = 39,
	LeftElbowRoll = 40,
	LeftArmCloth = 41,
	LeftShoulderPhys1 = 42,
	Neck = 43,
	Neck1 = 44,
	Head = 45,
	HeadEnd = 46,
	Face = 47,
	Jaw = 48,
	LeftLowerLip = 49,
	LeftLowerInnerLip = 50,
	LowerLip = 51,
	LowerInnerLip = 52,
	RightLowerLip = 53,
	RightLowerInnerLip = 54,
	Tongue = 55,
	TongueTip = 56,
	Chin = 57,
	LeftLowCheek = 58,
	RightLowCheek = 59,
	LeftEye = 60,
	LeftIris = 61,
	RightEye = 62,
	RightIris = 63,
	LeftUpCheek = 64,
	LeftUpInnerCheek = 65,
	RightUpInnerCheek = 66,
	RightUpCheek = 67,
	LeftCheek = 68,
	RightCheek = 69,
	LeftMouth = 70,
	LeftInnerMouth = 71,
	LeftMiddleCrease = 72,
	LeftUpperLip = 73,
	LeftUpperInnerLip = 74,
	UpperLip = 75,
	UpperInnerLip = 76,
	RightUpperLip = 77,
	RightUpperInnerLip = 78,
	RightMouth = 79,
	RightInnerMouth = 80,
	RightMiddleCrease = 81,
	LeftUpEyelid = 82,
	RightUpEyelid = 83,
	LeftLowEyelid = 84,
	RightLowEyelid = 85,
	LeftInnerEyebrow = 86,
	LeftOuterEyebrow = 87,
	RightInnerEyebrow = 88,
	RightOuterEyebrow = 89,
	LeftNose = 90,
	RightNose = 91,
	LeftCrease = 92,
	RightCrease = 93,
	LeftLowMiddleEyebrow = 94,
	RightMiddleEyebrow = 95,
	LeftLowEyelidCrease = 96,
	LeftLowOuterEyebrow = 97,
	NoseTip = 98,
	RightLowOuterEyebrow = 99,
	LeftMiddleEyebrow = 100,
	RightLowMiddleEyebrow = 101,
	RightLowEyelidCrease = 102,
	LowNose = 103,
	HeadCloth = 104,
	Throat = 105,
	Spine2Phys1 = 106,
	Spine2Prop1 = 107,
	RightShoulder = 108,
	RightArm = 109,
	RightArmRoll = 110,
	RightForeArm = 111,
	RightForeArmRoll = 112,
	RightForeArmRoll1 = 113,
	RightHand = 114,
	RightHandMiddle0 = 115,
	RightHandMiddle1 = 116,
	RightHandMiddle2 = 117,
	RightHandMiddle3 = 118,
	RightHandMiddle4 = 119,
	RightHandRing0 = 120,
	RightHandRing1 = 121,
	RightHandRing2 = 122,
	RightHandRing3 = 123,
	RightHandRing4 = 124,
	RightHandPinky0 = 125,
	RightHandPinky1 = 126,
	RightHandPinky2 = 127,
	RightHandPinky3 = 128,
	RightHandPinky4 = 129,
	RightHandIndex0 = 130,
	RightHandIndex1 = 131,
	RightHandIndex2 = 132,
	RightHandIndex3 = 133,
	RightHandIndex4 = 134,
	RightHandThumb1 = 135,
	RightHandThumb2 = 136,
	RightHandThumb3 = 137,
	RightHandThumb4 = 138,
	RightHandProp1 = 139,
	RightElbowRoll = 140,
	RightArmCloth = 141,
	RightShoulderPhys1 = 142,
	Wep_Root = 143,
	Wep_Trigger = 144,
	Wep_Slide = 145,
	Wep_Grenade1 = 146,
	Wep_Grenade2 = 147,
	Wep_Mag = 148,
	Wep_Mag_Ammo = 149,
	Wep_Mag_Extra1 = 150,
	Wep_Scope1 = 151,
	Wep_Scope2 = 152,
	Wep_Belt1 = 153,
	Wep_Belt2 = 154,
	Wep_Belt3 = 155,
	Wep_Belt4 = 156,
	Wep_Belt5 = 157,
	Wep_Belt6 = 158,
	Wep_Belt7 = 159,
	Wep_Belt8 = 160,
	Wep_Belt9 = 161,
	Wep_Belt10 = 162,
	Wep_Bipod1 = 163,
	Wep_Bipod2 = 164,
	Wep_Bipod3 = 165,
	IK_Joint_LeftHand = 166,
	IK_Joint_RightHand = 167,
	Wep_Physic1 = 168,
	Wep_Physic2 = 169,
	Wep_Physic3 = 170,
	Wep_Physic4 = 171,
	Wep_Physic5 = 172,
	Wep_Extra1 = 173,
	Wep_Extra2 = 174,
	Wep_Extra3 = 175,
	Wep_Extra4 = 176,
	Wep_Extra5 = 177,
	Wep_Aim = 178,
	LeftSpine1Phys1 = 179,
	RightSpine1Phys1 = 180,
	LeftUpLeg = 181,
	LeftUpLegRoll = 182,
	LeftLeg = 183,
	LeftFoot = 184,
	LeftToeBase = 185,
	LeftToe = 186,
	LeftLegCloth = 187,
	LeftKneeRoll = 188,
	LeftHipsRoll = 189,
	LeftUpLegCloth = 190,
	HipsCloth = 191,
	LeftHipsFrontPhys = 192,
	LeftHipsBackPhys = 193,
	LeftHipsSidePhys = 194,
	RightUpLeg = 195,
	RightUpLegRoll = 196,
	RightLeg = 197,
	RightFoot = 198,
	RightToeBase = 199,
	RightToe = 200,
	RightLegCloth = 201,
	RightKneeRoll = 202,
	RightHipsRoll = 203,
	RightUpLegCloth = 204,
	RightHipsFrontPhys = 205,
	RightHipsBackPhys = 206,
	RightHipsSidePhys = 207,
	CameraBase = 208,
	CameraJoint = 209,
	Connect = 210,
	ConnectEnd = 211,
	FacePosesAnimation = 212
};

enum eWeaponState {
	None = 0,
	Idle = 3,
	Shooting = 9,
	Reloading = 11,
	FinishReload = 12
};

enum eFireMode {
	Auto = 0,
	Single = 1,
	Burst = 2
};

enum OrderType {
	OrderType_None, //0x0000
	OrderType_RequestHeal, //0x0001
	OrderType_LetMeHeal, //0x0002
	OrderType_RequestAmmo, //0x0003
	OrderType_LetMeGiveAmmo, //0x0004
	OrderType_LetMeRepair, //0x0005
	OrderType_RequestRepair, //0x0006
	OrderType_RequestRide, //0x0007
	OrderType_GoGoGo, //0x0008
	OrderType_FollowMe, //0x0009
	OrderType_Negative, //0x000A
	OrderType_Sorry, //0x000B
	OrderType_ThankYou, //0x000C
	OrderType_RogerThat, //0x000D
	OrderType_NeedBackup, //0x000E
	OrderType_GetIn, //0x000F
	OrderType_GetOut, //0x0010
	OrderType_Attack, //0x0011
	OrderType_RequestSquadOrder, //0x0012
	OrderType_RequestCommanderOrder, //0x0013
	OrderType_RequestSupply, //0x0014
	OrderType_RequestVehicle, //0x0015
	OrderType_RequestScan, //0x0016
	OrderType_AcceptOrder, //0x0017
	OrderType_DenyOrder, //0x0018
	OrderType_RequestComm, //0x0019
	OrderType_Count //0x001A
};