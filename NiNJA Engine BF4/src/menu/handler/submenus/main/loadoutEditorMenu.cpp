#include "stdafx.h"
#include "loadoutEditorMenu.h"
#include "../currentGameMenu.h"

using namespace LoadoutEditorMenuVars;

namespace LoadoutEditorMenuVars {
	Vars vars;

	ScrollStruct<DWORD> Types[] = {
		"Primary", 0x1218,
		"Secondary", 0x1288,
		"Gadget 1", 0x12F8,
		"Gadget 2", 0x1448,
		"Grenade", 0x14B8,
		"Knife", 0x1528
	};

	ScrollStruct<const char*> WeaponTypes_Special[] = {
		// "Railgun", "U_Railgun",
		"Icicle", "U_Icicle",
		"Mare's Leg", "U_SaddlegunSnp",
		"RAWR", "U_MAARSBot"
	};

	ScrollStruct<const char*> WeaponTypes_AssaultRifle[] = {
		"AK-12", "U_AK12",
		"SCAR-H", "U_SCAR-H",
		"M416", "U_M416",
		"SAR-21", "U_SAR21",
		"AEK-971", "U_AEK971",
		"FAMAS", "U_FAMAS",
		"AUG A3", "U_SteyrAug",
		"M16A4", "U_M16A4",
		"CZ-805", "U_CZ805",
		"QBZ-95-1", "U_QBZ951",
		"ACE 23", "U_GalilACE23",
		"F2000", "U_F2000",
		"L85A2", "U_L85A2",
		"AR160", "U_AR160",
		"Bulldog", "U_Bulldog",
		"AN-94", "U_AN94"
	};

	ScrollStruct<const char*> WeaponTypes_PersonalDefense[] = {
		"MX4", "U_MX4",
		"PP-2000", "U_PP2000",
		"CBJ-MS", "U_CBJ-MS",
		"PDW-R", "U_MagpulPDR",
		"CZ-3A1", "U_Scorpion",
		"JS2", "U_JS2",
		"P90", "U_P90",
		"UMP-9", "U_UMP9",
		"AS VAL", "U_ASVal",
		"MP7", "U_MP7",
		"SR-2", "U_SR2",
		"MPX", "U_MPX",
		"Groza-4", "U_Groza-4"
	};

	ScrollStruct<const char*> WeaponTypes_LightMachineGun[] = {
		"U-100 MK5", "U_Ultimax",
		"Type-88 LMG", "U_Type88",
		"LSAT", "U_LSAT",
		"PKP Pecheneg", "U_Pecheneg",
		"QBB-95-1", "U_QBB95",
		"M240B", "U_M240",
		"MG4", "U_MG4",
		"M249", "U_M249",
		"RPK-12", "U_RPK12",
		"M60-E4", "U_M60E4",
		"RPK", "U_RPK-74",
		"AWS", "U_AWS",
		"L86A2", "U_L86A1"
	};

	ScrollStruct<const char*> WeaponTypes_SniperRifle[] = {
		"CS-LR4", "U_CS-LR4",
		"M40A5", "U_M40A5",
		"Scout Elite", "U_Scout",
		"SV-98", "U_SV98",
		"JNG-90", "U_JNG90",
		"338-Recon", "U_SRS",
		"M98B", "U_M98B",
		"SRR-61", "U_M200",
		"FY-JS", "U_FY-JS",
		"GOL Magnum", "U_GOL",
		"L115", "U_L96A1",
		"SR338", "U_SR338",
		"CS5", "U_CS5"
	};

	ScrollStruct<const char*> WeaponTypes_CarbineRifle[] = {
		"AK 5C", "U_AK5C",
		"ACW-R", "U_ACR",
		"SG553", "U_SG553LB",
		"AKU-12", "U_AKU12",
		"A-91", "U_A91",
		"ACE 52 CQB", "U_GalilACE52",
		"G36C", "U_G36C",
		"M4A1", "U_M4A1",
		"ACE 21 CQB", "U_GalilACE",
		"Type-95B-1", "U_Type95B",
		"MTAR-21", "U_MTAR21",
		"GROZA-1", "U_Groza-1"
	};

	ScrollStruct<const char*> WeaponTypes_DesignatedMarksmanRifle[] = {
		"RFB", "U_RFB",
		"MK11 MOD 0", "U_MK11",
		"SKS", "U_SKS",
		"SVD-12", "U_SVD12",
		"QBU-88", "U_QBU88",
		"M39 EMR", "U_M39EBR",
		"ACE 53 SV", "U_GalilACE53",
		"SCAR-H SV", "U_SCAR-HSV",
	};

	ScrollStruct<const char*> WeaponTypes_Shotgun[] = {
		"QBS-09", "U_QBS09",
		"870 MCS", "U_870",
		"M1014", "U_M1014",
		"HAWK 12G", "U_HAWK",
		"Saiga 12K", "U_SAIGA_20K",
		"Spas-12", "U_SPAS12",
		"UTS 15", "U_UTAS",
		"DBV-12", "U_DBV12",
		"DAO-12", "U_DAO12",
		"SerbuShorty", "U_SerbuShorty"
	};

	ScrollStruct<const char*> WeaponTypes_Pistol[] = {
		"P226", "U_P226",
		"M9", "U_M9",
		"QSZ-92", "U_QSZ92",
		"MP443", "U_MP443",
		"G18", "U_Glock18",
		"FN57", "U_FN57",
		"M1911", "U_M1911",
		"93R", "U_M93R",
		"CZ-75", "U_CZ75",
		".44 Magnum", "U_Taurus44",
		"Compact 45", "U_HK45C",
		"MP412 Rex", "U_MP412Rex",
		"SW40", "U_SW40",
		"Unica 6", "U_Unica6",
		"Deagle 44", "U_DesertEagle"
	};

	ScrollStruct<const char*> WeaponTypes_Grenade[] = {
		"M67 FRAG", "U_M67",
		"V40 MINI", "U_V40",
		"RGO IMPACT", "U_Grenade_RGO",
		"M34 INCENDIARY", "U_M34",
		"M18 SMOKE", "U_M18",
		"M84 FLASHBANG", "U_Flashbang",
		"HAND FLARE", "U_Handflare"
	};

	ScrollStruct<const char*> WeaponTypes_Knife[] = {
		"BAYONET", "U_Knife",
		"ACB-90", "U_ACB-90",
		"SHANK", "U_Prisionshank",
		"MACHETE", "U_Machete",
		"BOWIE", "U_KnifeBowie",
		"CARBON FIBER", "U_KnifeCarbon",
		"SCOUT", "U_KnifeScout",
		"SURVIVAL", "U_KnifeSurvival",
		"IMPROVISED", "U_KnifeImprovised",
		"TRENCH", "U_KnifeTrench",
		"BOOT", "U_KnifeBoot",
		"SEAL", "U_KnifeSeal",
		"DIVE", "U_KnifeDivers",
		"TACTICAL", "U_Knife14100BT",
		"BJ-2", "U_Knife2142",
		"PRECISION", "U_KnifePrecision",
		"NECK", "U_KnifeNeck",
		"TANTO", "U_KnifeTanto",
		"WEAVER", "U_KnifeWeaver",
		"C100", "U_KnifeBipod"
	};

	ScrollStruct<const char*> WeaponTypes_ExplosiveAntiTank[] = {
		"M15 AT MINE", "U_M15",
		"M2 SLAM", "U_SLAM",
		"C4 EXPLOSIVE", "U_C4"
	};

	ScrollStruct<const char*> WeaponTypes_ExplosiveAntiAir[] = {
		"AA MINE", "U_AAMine"
	};

	ScrollStruct<const char*> WeaponTypes_ExplosiveAntiPersonnel[] = {
		"M18 CLAYMORE", "U_Claymore",
		"C4 EXPLOSIVE", "U_C4",
		"M67 FRAG", "U_M67",
		"V40 MINI", "U_V40",
		"RGO IMPACT", "U_Grenade_RGO",
		"M34 INCENDIARY", "U_M34"
	};

	ScrollStruct<const char*> WeaponTypes_AntiTank[] = {
		"MBT LAW", "U_NLAW",
		"RPG-7V2", "U_RPG7",
		"MK153 SMAW", "U_SMAW",
		"FGM-148 JAVELIN", "U_FGM148",
		"FGM-172 SRAW", "U_SRAW"
	};

	ScrollStruct<const char*> WeaponTypes_AntiAir[] = {
		"FIM-92 STINGER", "U_FIM92",
		"SA-18 IGLA", "U_Sa18IGLA",
		"HVM-II", "U_Starstreak"
	};

	ScrollStruct<int> JumpToList[] = {
		"Special", 3,
		"Assault Rifle", 8,
		"Personal Defense", 25,
		"Light Machine Gun", 39,
		"Sniper Rifle", 53,
		"Carbine Rifle", 67,
		"Designated Marksman Rifle", 80,
		"Shotgun", 89,
		"Pistol", 100,
		"Grenade", 116,
		"Knife", 124,
		"Explosive Anti-Tank", 145,
		"Explosive Anti-Air", 149,
		"Anti-Tank", 151,
		"Anti-Air", 158
	};

	SoldierWeaponUnlockAsset* SoldierWeaponUnlockAssetFromName(const char* pName) {
		for (int i = 0; i < 400; i++) {
			if (vars.szWeaponAssets[i] != 0) {
				auto asset = (SoldierWeaponUnlockAsset*)vars.szWeaponAssets[i];
				if (ValidPtr(asset)) {
					if (asset->pName) {
						if (!strcmp(asset->pName, pName)) {
							return asset;
						}
					}
				}
			}
		}

		return nullptr;
	}

	bool IsUsingRailgun() {
		auto wep = pGame.GetSoldierWeapon();
		if (ValidPtr(wep)) {
			if (ValidPtr(wep->pWeaponUnlockAsset)) {
				if (ValidPtr(wep->pWeaponUnlockAsset->pName)) {
					return (bool)strstr(wep->pWeaponUnlockAsset->pName, "Railgun");
				}
			}
		}

		return false;
	}

	void ChangeLoadoutItem(const char* pName) {
		if (pName) {
			bool canRefresh = false;

			if (ValidPtr(pGame.GetLocalPlayer())) {
				auto venice = pGame.GetLocalPlayer()->pVeniceSoldierCustomizationAsset;
				if (ValidPtr(venice)) {
					// this size seems to be fine.
					for (int i = 0; i < 1000; i++) {
						auto asset = venice->pAsset[i];
						if (ValidPtr(asset)) {
							// if the assets vtable is SoldierWeaponUnlockAsset
							if (asset->dwVTable == 0x88162C90) {
								if (asset == (Asset*)*(DWORD*)((DWORD)pGame.GetLocalPlayer() + Types[vars.iType].Result)) {
									// cba to modify my struct again for the loadout offset types. This is essentially going
									// thru a list of assets finding my current loadout item SoldierWeaponUnlockAsset, once
									// it finds it, it changes it to the ptr of the spoof weapon.
									// After that, it does the same stuff unlimited ammo does which for this ends up hotswapping the weapon.

									auto p = SoldierWeaponUnlockAssetFromName(pName);
									if (p) {
										canRefresh = true;
										venice->pAsset[i] = (Asset*)p;
									}
								}
							}
						}
					}
				}
			}

			if (canRefresh) {
				DWORD r3 = Invoke::Call<DWORD>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwUnlimitedAmmo[0]), *(DWORD*)(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwUnlimitedAmmo[1])) + 0x150, "SelectedKit", 0);
				DWORD r4 = Invoke::Call<DWORD>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwUnlimitedAmmo[2])); // might be active slot?
				bool bIsSpawning = false;

				Invoke::Call<DWORD>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwUnlimitedAmmo[3]), r3, r4, bIsSpawning);
			}
		}
	}

	void Jump() {
		vars.bCanJump = true;
		vars.iJumpTimeout = GetTickCount();
	}
}

void LoadoutEditorMenu::Init() {
	SetParentSubmenu<CurrentGameMenu>();
	SetName("Loadout Editor");

	addOption(ScrollOption<DWORD>(SCROLL, "Loadout Type")
		.addScroll(vars.iType, 0, NUMOF(Types) - 1, Types).canLoop()
		.addTooltip("Selects the item type."));

	addOption(ScrollOption<int>(SCROLLSELECT, "Jump To")
		.addScroll(vars.iJumpTo, 0, NUMOF(JumpToList) - 1, JumpToList).canLoop()
		.addFunction(Jump)
		.addTooltip("Jumps to a list."));

	addOption(BreakOption("Weapons - Special"));

	for (int i = 0; i < NUMOF(WeaponTypes_Special); i++) {
		addOption(ButtonOption(WeaponTypes_Special[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_Special[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}

	addOption(BreakOption("Weapons - Assault Rifle"));

	for (int i = 0; i < NUMOF(WeaponTypes_AssaultRifle); i++) {
		addOption(ButtonOption(WeaponTypes_AssaultRifle[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_AssaultRifle[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}

	addOption(BreakOption("Weapons - Personal Defense"));

	for (int i = 0; i < NUMOF(WeaponTypes_PersonalDefense); i++) {
		addOption(ButtonOption(WeaponTypes_PersonalDefense[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_PersonalDefense[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}

	addOption(BreakOption("Weapons - Light Machine Gun"));

	for (int i = 0; i < NUMOF(WeaponTypes_LightMachineGun); i++) {
		addOption(ButtonOption(WeaponTypes_LightMachineGun[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_LightMachineGun[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}

	addOption(BreakOption("Weapons - Sniper Rifle"));

	for (int i = 0; i < NUMOF(WeaponTypes_SniperRifle); i++) {
		addOption(ButtonOption(WeaponTypes_SniperRifle[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_SniperRifle[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}

	addOption(BreakOption("Weapons - Carbine Rifle"));

	for (int i = 0; i < NUMOF(WeaponTypes_CarbineRifle); i++) {
		addOption(ButtonOption(WeaponTypes_CarbineRifle[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_CarbineRifle[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}

	addOption(BreakOption("Weapons - Designated Marksman Rifle"));

	for (int i = 0; i < NUMOF(WeaponTypes_DesignatedMarksmanRifle); i++) {
		addOption(ButtonOption(WeaponTypes_DesignatedMarksmanRifle[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_DesignatedMarksmanRifle[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}

	addOption(BreakOption("Weapons - Shotgun"));

	for (int i = 0; i < NUMOF(WeaponTypes_Shotgun); i++) {
		addOption(ButtonOption(WeaponTypes_Shotgun[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_Shotgun[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}
	
	addOption(BreakOption("Weapons - Pistol"));

	for (int i = 0; i < NUMOF(WeaponTypes_Pistol); i++) {
		addOption(ButtonOption(WeaponTypes_Pistol[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_Pistol[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}

	addOption(BreakOption("Weapons - Grenade"));

	for (int i = 0; i < NUMOF(WeaponTypes_Grenade); i++) {
		addOption(ButtonOption(WeaponTypes_Grenade[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_Grenade[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}

	addOption(BreakOption("Weapons - Knife"));

	for (int i = 0; i < NUMOF(WeaponTypes_Knife); i++) {
		addOption(ButtonOption(WeaponTypes_Knife[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_Knife[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}

	addOption(BreakOption("Weapons - Explosive Anti Tank"));

	for (int i = 0; i < NUMOF(WeaponTypes_ExplosiveAntiTank); i++) {
		addOption(ButtonOption(WeaponTypes_ExplosiveAntiTank[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_ExplosiveAntiTank[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}

	addOption(BreakOption("Weapons - Explosive Anti Air"));

	for (int i = 0; i < NUMOF(WeaponTypes_ExplosiveAntiAir); i++) {
		addOption(ButtonOption(WeaponTypes_ExplosiveAntiAir[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_ExplosiveAntiAir[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}

	addOption(BreakOption("Weapons - Explosive Anti Personnel"));

	for (int i = 0; i < NUMOF(WeaponTypes_ExplosiveAntiPersonnel); i++) {
		addOption(ButtonOption(WeaponTypes_ExplosiveAntiPersonnel[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_ExplosiveAntiPersonnel[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}

	addOption(BreakOption("Weapons - Anti Tank"));

	for (int i = 0; i < NUMOF(WeaponTypes_AntiTank); i++) {
		addOption(ButtonOption(WeaponTypes_AntiTank[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_AntiTank[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}

	addOption(BreakOption("Weapons - Anti Air"));

	for (int i = 0; i < NUMOF(WeaponTypes_AntiAir); i++) {
		addOption(ButtonOption(WeaponTypes_AntiAir[i].pName)
			.addFunctionArg(ChangeLoadoutItem, WeaponTypes_AntiAir[i].Result)
			.addTooltip("Changes the loadout item to this."));
	}
}

/*Update once when submenu is opened*/
void LoadoutEditorMenu::UpdateOnce() {
	int index = 0;

	auto ptr = pGame.GetResourceManager();
	if (ptr) {
		for (int i = 0; i < 50; i++) {
			auto comp = ptr->pCompartments[i];
			if (ValidPtr(comp)) {
				// it uses an eastl vector, so we take the address of the first instance
				// and minus it from the last (because it's in a list). Divide it by 4
				// (obv, ptr = 4 bytes) and that's our size. Ezpz.
				DWORD size = (comp->dwPtrToEndList - comp->dwPtrToFirstList) / 4;
				for (int j = 0; j < size; j++) {
					DWORD dwAddress = *(DWORD*)(comp->dwPtrToFirstList + (j * 4));
					if (dwAddress) {
						// if the assets vtable is SoldierWeaponUnlockAsset
						if (*(DWORD*)dwAddress == 0x88162C90) {
							vars.szWeaponAssets[index] = dwAddress;
							index++;
						}
					}
				}
			}
		}
	}
}

/*Update while submenu is active*/
void LoadoutEditorMenu::Update() {}

/*Background update*/
void LoadoutEditorMenu::FeatureUpdate() {
	if (vars.bCanJump) {
		if (GetTickCount() - vars.iJumpTimeout > 250) {
			vars.bCanJump = false;

			for (int i = 0; i < JumpToList[vars.iJumpTo].Result - 1; i++) {
				Down();
			}
		}
	}
}

/*Singleton*/
LoadoutEditorMenu* _instance;
LoadoutEditorMenu* LoadoutEditorMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new LoadoutEditorMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
LoadoutEditorMenu::~LoadoutEditorMenu() { delete _instance; }