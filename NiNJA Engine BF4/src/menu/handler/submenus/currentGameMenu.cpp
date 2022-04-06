#include "stdafx.h"
#include "currentGameMenu.h"
#include "main/serverSettingsMenu.h"
#include "main/loadoutEditorMenu.h"
#include "main/ordersMenu.h"

using namespace CurrentGameMenuVars;

namespace CurrentGameMenuVars {
	Vars vars;
	
	void Invisibility() {
		static DWORD* stored = nullptr;
		if (ValidPtr(pGame.GetLocalPlayer())) {
			auto venice = pGame.GetLocalPlayer()->pVeniceSoldierCustomizationAsset;
			if (ValidPtr(venice)) {
				auto table = venice->pWeaponTable;
				if (ValidPtr(table)) {
					if (ValidPtr(table->pParts)) {
						auto part = table->pParts[0];
						if (ValidPtr(part)) {
							if (ValidPtr(part->pVisualAssets)
								&& ValidPtr(part->pVisualAssets[0])
								&& ValidPtr(part->pVisualAssets[1])) {
								if (vars.bInvisibility) {
									if (!stored) stored = part->pVisualAssets[0];
									part->pVisualAssets[0] = part->pVisualAssets[1];
								} else {
									if (stored) {
										part->pVisualAssets[0] = stored;
										stored = 0;
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

void CurrentGameMenu::Init() {
	SetName("Current Game");

	addOption(SubmenuOption("Server Settings")
		.addSubmenu<ServerSettingsMenu>()
		.addTooltip("Edit server settings (local)."));

	addOption(SubmenuOption("Loadout Editor")
		.addSubmenu<LoadoutEditorMenu>()
		.addTooltip("Changes your loadout."));

	addOption(SubmenuOption("Orders")
		.addSubmenu<OrdersMenu>()
		.addTooltip("Send orders for your team."));

	// deployment

	addOption(ToggleOption("Invisibility")
		.addToggle(vars.bInvisibility)
		.addFunction(Invisibility)
		.addTooltip("Makes you invisible to normal users."));
}

/*Update once when submenu is opened*/
void CurrentGameMenu::UpdateOnce() {}

/*Update while submenu is active*/
void CurrentGameMenu::Update() {}

/*Background update*/
void CurrentGameMenu::FeatureUpdate() {}

/*Singleton*/
CurrentGameMenu* _instance;
CurrentGameMenu* CurrentGameMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new CurrentGameMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
CurrentGameMenu::~CurrentGameMenu() { delete _instance; }