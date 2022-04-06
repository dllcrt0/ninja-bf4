#pragma once
#include "menu/handler/submenu.h"

class LoadoutEditorMenu : public Submenu {
public:
	static LoadoutEditorMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	LoadoutEditorMenu() : Submenu() {}
	~LoadoutEditorMenu();
};

namespace LoadoutEditorMenuVars
{
	struct Vars {
		int iType;
		int iJumpTo;
		int iJumpTimeout;

		bool bCanJump;
		bool bHasGrabbedAssets;

		DWORD szWeaponAssets[400];
	};

	void ChangeLoadoutItem(const char* pName);
	bool IsUsingRailgun();
	SoldierWeaponUnlockAsset* SoldierWeaponUnlockAssetFromName(const char* pName);
	extern Vars vars;
}