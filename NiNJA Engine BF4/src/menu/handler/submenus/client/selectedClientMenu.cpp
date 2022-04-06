#include "stdafx.h"
#include "selectedClientMenu.h"
#include "../clientMenu.h"

using namespace SelectedClientMenuVars;

namespace SelectedClientMenuVars {
	Vars vars;

	ScrollStruct<int> Callouts[] = {
		"Being a bitch", 0,
		"Is a bitch", 1,
		"Modding", 2,
		"Being a fgt", 3,
		"Using trash cheats", 4,
		"Being a NiNJA fag", 5,
		"Why you heff to be mad", 6,
		"Railgun too op", 7,
		"Dad works at Microsoft", 8,
		"Eats shoes", 9,
		"Clearly using OP cheats", 10,
		"NiNJA is better", 11
	};

	void Callout() {
		auto player = pGame.GetClientGameContext()->pClientPlayerManager->GetPlayer(ClientMenuVars::vars.iSelectedClient);
		if (ValidPtr(player)) {
			switch (vars.iCallout) {
			case 0:
				pMessages.SendTextMessage(CctSayAll, CctSayAll, pUtils.va("%s is being a bitch.", player->szName));
				break;
			case 1:
				pMessages.SendTextMessage(CctSayAll, CctSayAll, pUtils.va("%s is a bitch.", player->szName));
				break;
			case 2:
				pMessages.SendTextMessage(CctSayAll, CctSayAll, pUtils.va("%s is modding yo.", player->szName));
				break;
			case 3:
				pMessages.SendTextMessage(CctSayAll, CctSayAll, pUtils.va("%s is being a fgt.", player->szName));
				break;
			case 4:
				pMessages.SendTextMessage(CctSayAll, CctSayAll, pUtils.va("%s is using trash cheats lul.", player->szName));
				break;
			case 5:
				pMessages.SendTextMessage(CctSayAll, CctSayAll, pUtils.va("I suspect %s is a fellow NiNJA fgt.", player->szName));
				break;
			case 6:
				pMessages.SendTextMessage(CctSayAll, CctSayAll, pUtils.va("Is just game, why you heff to be mad %s.", player->szName));
				break;
			case 7:
				pMessages.SendTextMessage(CctSayAll, CctSayAll, pUtils.va("Railgun too op %s.", player->szName));
				break;
			case 8:
				pMessages.SendTextMessage(CctSayAll, CctSayAll, pUtils.va("My dad works at Microsoft %s, prepare for an XBL ban.", player->szName));
				break;
			case 9:
				pMessages.SendTextMessage(CctSayAll, CctSayAll, pUtils.va("%s eats shoes.", player->szName));
				break;
			case 10:
				pMessages.SendTextMessage(CctSayAll, CctSayAll, pUtils.va("\"OP\" cheats huh? nah, you suck %s.", player->szName));
				break;
			case 11:
				pMessages.SendTextMessage(CctSayAll, CctSayAll, pUtils.va("NiNJA is clearly the better choice, %s.", player->szName));
				break;
			}
		}
	}
}

void SelectedClientMenu::Init() {
	SetParentSubmenu<ClientMenu>();
	SetName("Selected Client");
}

/*Update once when submenu is opened*/
void SelectedClientMenu::UpdateOnce() {
	ClearOptionsOffset(0);

	auto player = pGame.GetClientGameContext()->pClientPlayerManager->GetPlayer(ClientMenuVars::vars.iSelectedClient);
	if (ValidPtr(player)) {
		WhiteBlackList* ptr = nullptr;
		auto vit = find(pAimbot.WBList.begin(), pAimbot.WBList.end(), player->szName);
		if (vit == pAimbot.WBList.end()) {
			pAimbot.WBList.push_back(WhiteBlackList(player->szName, false, false));
			ptr = &pAimbot.WBList.at(pAimbot.WBList.size() - 1);
		} else {
			ptr = &(*vit);
		}

		addOption(ToggleOption("Blacklist")
			.addToggle(ptr->bBlacklist)
			.addTooltip("Whether or not the player should be \npreferred by the aimbot."));

		addOption(ToggleOption("Whitelist")
			.addToggle(ptr->bWhitelist)
			.addTooltip("Whether or not the player should be \nignored by the aimbot."));

		addOption(ScrollOption<int>(SCROLLSELECT, "Callouts")
			.addScroll(vars.iCallout, 0, NUMOF(Callouts) - 1, Callouts)
			.addFunction(Callout)
			.addTooltip("Calls out the user in the chat."));
	}
}

/*Update while submenu is active*/
void SelectedClientMenu::Update() {
	static int timer = 0;
	if (GetTickCount() - timer > 500) {
		timer = GetTickCount();

		auto player = pGame.GetClientGameContext()->pClientPlayerManager->GetPlayer(ClientMenuVars::vars.iSelectedClient);
		if (!ValidPtr(player)) {
			pSubmenuHandler.SetSubmenuPrevious(false);
		}
	}
}

/*Background update*/
void SelectedClientMenu::FeatureUpdate() {}

/*Singleton*/
SelectedClientMenu* _instance;
SelectedClientMenu* SelectedClientMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new SelectedClientMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
SelectedClientMenu::~SelectedClientMenu() { delete _instance; }