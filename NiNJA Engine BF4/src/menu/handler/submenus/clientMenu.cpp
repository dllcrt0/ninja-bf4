#include "stdafx.h"
#include "clientMenu.h"
#include "client/selectedClientMenu.h"

using namespace ClientMenuVars;

namespace ClientMenuVars {
	Vars vars;
	
	void Callback(ClientPlayer* pEntity) {
		if (ValidPtr(pEntity)) {
			vars.Players.push_back(UserData(pEntity->iID, pEntity->szName));
		}
	}

	void ReloadPlayerList() {
		if (pGame.bCanRun) {
			vars.Players.clear();
			pPlayer.PlayerCallback(true, Callback, true);
		}

		vars.bNeedsToSort = true;
	}

	void UpdateSelectedClient() {
		if (ValidPtr(pGame.GetClientGameContext()->pClientPlayerManager->GetPlayer(pMenu.iCurrentOption))) {
			if (vars.Players.size() >= (pMenu.iCurrentOption + 1)) {
				vars.iSelectedClient = vars.Players[pMenu.iCurrentOption].iID;
			}
		} else {
			ReloadPlayerList();
		}
	}
}

void ClientMenu::Init() {
	SetName("Clients");
}

/*Update once when submenu is opened*/
void ClientMenu::UpdateOnce() {
	ReloadPlayerList();
}

/*Update while submenu is active*/
void ClientMenu::Update() {
	// update

	static int timer = 0;
	if (GetTickCount() - timer > 500) {
		if (pGame.bCanRun) {
			ClearOptionsOffset(0);

			vars.Players.clear();
			pPlayer.PlayerCallback(true, Callback, true, true);

			for (int i = 0; i < vars.Players.size(); i++) {
				addOption(SubmenuOption(pUtils.va("%s ", vars.Players[i].pName))
					.addSubmenu<SelectedClientMenu>()
					.addFunction(UpdateSelectedClient)
					.addTooltip("Manage settings pertaining to the currently \nselected player."));
			}

			for (int i = 0; i < (24 - vars.Players.size()); i++) {
				addOption(ButtonOption("Searching...")
					.addTooltip("Manage settings pertaining to the currently \nselected player."));
			}
		}
	}
}

/*Background update*/
void ClientMenu::FeatureUpdate() {}

/*Singleton*/
ClientMenu* _instance;
ClientMenu* ClientMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ClientMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
ClientMenu::~ClientMenu() { delete _instance; }