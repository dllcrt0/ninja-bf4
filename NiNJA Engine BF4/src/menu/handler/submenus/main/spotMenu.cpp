#include "stdafx.h"
#include "spotMenu.h"
#include "../mainMenu.h"

using namespace SpotMenuVars;

namespace SpotMenuVars {
	Vars vars;

	ScrollStruct<int> SpotEntities[] = {
		"Players", 0,
		"Vehicles", 1,
		"Both", 2
	};
	
	void Callback(ClientPlayer* pEntity) {
		if (ValidPtr(pEntity)) {
			if (!pPlayer.IsFriendly(pEntity)) {
				auto soldier = pEntity->GetClientSoldier();
				if (ValidPtr(soldier)) {
					auto spot = soldier->pClientSpottingComponent;
					if (ValidPtr(spot)) {
						switch (vars.iSpotType) {
							case 2:
							case 0:
								if (!pEntity->InVehicle()) {

								}

								if (vars.iSpotType != 2)
									break;
							case 1:
								if (pEntity->InVehicle()) {

								}

								break;
						}
					}
				}
			}
		}
	}

	void Spot() {
		if (ValidPtr(pGame.GetMySoldierEntity()) && ValidPtr(pGame.GetMySoldierEntity()->pClientSpottingComponent)) {
			pPlayer.PlayerCallback(true, Callback);
		}
	}
}

void SpotMenu::Init() {
	SetParentSubmenu<MainMenu>();
	SetName("Spot");

	addOption(ScrollOption<int>(SCROLL, "Spot Entity").canLoop()
		.addScroll(vars.iSpotType, 0, NUMOF(SpotEntities) - 1, SpotEntities)
		.addTooltip("Choose which entities to spot."));

	addOption(ButtonOption("Send Spot")
		.addFunction(Spot)
		.addTooltip("Spot the selected entities once."));

	addOption(ToggleOption("Always Spot")
		.addToggle(vars.bAlwaysSpot)
		.addTooltip("Spot the selected entities always."));
}

/*Update once when submenu is opened*/
void SpotMenu::UpdateOnce() {}

/*Update while submenu is active*/
void SpotMenu::Update() {}

/*Background update*/
void SpotMenu::FeatureUpdate() {
	if (vars.bAlwaysSpot) {
		static int timer = 0;
		if (GetTickCount() - timer > 1000) {
			timer = GetTickCount();

			Spot();
		}
	}
}

/*Singleton*/
SpotMenu* _instance;
SpotMenu* SpotMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new SpotMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
SpotMenu::~SpotMenu() { delete _instance; }