#include "stdafx.h"
#include "menu/handler/submenus/mainMenu.h"

SubmenuHandler pSubmenuHandler;

void SubmenuHandler::Init() {
	DbgPrint("Initializing submenu handler");

	pMainSubmenu = pCurrentSubmenu = MainMenu::GetInstance();
	WeaponMenu::GetInstance();
	AimbotMenu::GetInstance();
	VisualMenu::GetInstance();
	ClientMenu::GetInstance();
	SettingsMenu::GetInstance();

	DbgPrint("Finished initializing submenus");

	if (pConfig.bCanLoadConfigOnceInitialized) {
		pConfig.bLoadCurrentSettings = true;
	}
}

void SubmenuHandler::Update() {
	pCurrentSubmenu->MenuUpdate();
	if (pNextSubmenu != nullptr) {
		pMenu.iCurrentOption = iNextCurrentOption;
		pMenu.iScrollOffset = iNextScrollOffset;
		pCurrentSubmenu = pNextSubmenu;
		pNextSubmenu = nullptr;
		pCurrentSubmenu->UpdateOnce();
	}
}

void SubmenuHandler::ASyncUpdate() {
	for (auto i = 0; i < pSubmenus.size(); i++) {
		pSubmenus.at(i)->FeatureUpdate();
	}
}

void SubmenuHandler::SetSubmenu(Submenu* submenu) {
	pCurrentSubmenu->iOldScrollOffset = pMenu.iScrollOffset;
	pCurrentSubmenu->iOldCurrentOption = pMenu.iCurrentOption;
	iNextCurrentOption = 0;
	iNextScrollOffset = 0;
	pNextSubmenu = submenu;
}

void SubmenuHandler::SetSubmenuMain() {
	pNextSubmenu = pMainSubmenu;
	iNextCurrentOption = 0;
	iNextScrollOffset = 0;
}

void SubmenuHandler::SetSubmenuPrevious(bool resetPos) {
	if (pCurrentSubmenu->GetParentSubmenu() == nullptr) {
		bool update = false;
		if (pMenu.IsOpen()) {
			update = true;
		}

		SetSubmenuMain();
		pMenu.iSelectedTab = 0;
		pMenu.iCurrentOption = 0;
		pMenu.iScrollOffset = 0;
		pMenu.iBreakScroll = 0;
		pMenu.iCurrentRenderingScroll = 0;
		pMenu.Close();

		if (update) {
			DbgPrint("Updating config...");
			pConfig.bSaveCurrentSettings = true;
		}
	} else {
		pNextSubmenu = pCurrentSubmenu->GetParentSubmenu();
		iNextCurrentOption = resetPos ? 0 : pNextSubmenu->iOldCurrentOption;
		iNextScrollOffset = resetPos ? 0 : pNextSubmenu->iOldScrollOffset;
	}
}