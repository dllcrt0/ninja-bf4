#include "stdafx.h"
#include "developerMenu.h"
#include "../mainMenu.h"

using namespace DeveloperMenuVars;

namespace DeveloperMenuVars {
	Vars vars;

	void Test() {
		
	}
}

void DeveloperMenu::Init() {
	SetParentSubmenu<MainMenu>();
	SetName("Developer");

	addOption(ButtonOption("Test")
		.addFunction(Test));
}

/*Update once when submenu is opened*/
void DeveloperMenu::UpdateOnce() {}

/*Update while submenu is active*/
void DeveloperMenu::Update() {}

/*Background update*/
void DeveloperMenu::FeatureUpdate() {
#ifdef DEVELOPER_OPTIONS
	
#endif
}

/*Singleton*/
DeveloperMenu* _instance;
DeveloperMenu* DeveloperMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new DeveloperMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
DeveloperMenu::~DeveloperMenu() { delete _instance; }