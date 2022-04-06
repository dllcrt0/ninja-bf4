#include "stdafx.h"
#include "visualCustomizationMenu.h"
#include "../visualMenu.h"
#include "../settings/color/colorEditorMenu.h"

using namespace VisualCustomizationMenuVars;

namespace VisualCustomizationMenuVars {
	Vars vars;
	
	void ColorSubCallback() {
		ColorEditorMenu::GetInstance()->SetParentSubmenu<VisualCustomizationMenu>();
		ColorEditorMenu::GetInstance()->SetName(VisualCustomizationMenu::GetInstance()->GetOptions()[pMenu.iCurrentOption]->GetName());

		switch (pMenu.iCurrentOption) {
		case 0: // Enemy
			ColorEditorMenuVars::vars.ColorValue = VisualMenuVars::vars.cEnemy;
			ColorEditorMenuVars::vars.pToUpdate = &VisualMenuVars::vars.cEnemy;
			break;
		case 1: // Friendly
			ColorEditorMenuVars::vars.ColorValue = VisualMenuVars::vars.cFriendly;
			ColorEditorMenuVars::vars.pToUpdate = &VisualMenuVars::vars.cFriendly;
			break;
		case 2: // Preferred
			ColorEditorMenuVars::vars.ColorValue = VisualMenuVars::vars.cPreferred;
			ColorEditorMenuVars::vars.pToUpdate = &VisualMenuVars::vars.cPreferred;
			break;
		case 3: // Wallbangable
			ColorEditorMenuVars::vars.ColorValue = VisualMenuVars::vars.cWallbangable;
			ColorEditorMenuVars::vars.pToUpdate = &VisualMenuVars::vars.cWallbangable;
			break;
		case 4: // Whitelisted
			ColorEditorMenuVars::vars.ColorValue = VisualMenuVars::vars.cWhitelisted;
			ColorEditorMenuVars::vars.pToUpdate = &VisualMenuVars::vars.cWhitelisted;
			break;
		case 5: // Visible
			ColorEditorMenuVars::vars.ColorValue = VisualMenuVars::vars.cVisible;
			ColorEditorMenuVars::vars.pToUpdate = &VisualMenuVars::vars.cVisible;
			break;
		}
	}
}

void VisualCustomizationMenu::Init() {
	SetParentSubmenu<VisualMenu>();
	SetName("Customization");

	addOption(ColorOption("Enemy")
		.addPreviewBox()
		.addColor(VisualMenuVars::vars.cEnemy)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the enemy ESP color when not \nvisible, wallbangable, preferred, or \nwhitelisted."));

	addOption(ColorOption("Friendly")
		.addPreviewBox()
		.addColor(VisualMenuVars::vars.cFriendly)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the ESP color for friendlies."));

	addOption(ColorOption("Preferred")
		.addPreviewBox()
		.addColor(VisualMenuVars::vars.cPreferred)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the ESP color for clients you have \nset as preferred targets."));

	addOption(ColorOption("Wallbangable")
		.addPreviewBox()
		.addColor(VisualMenuVars::vars.cWallbangable)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the ESP color when a target is \nwallbangable."));

	addOption(ColorOption("Whitelisted")
		.addPreviewBox()
		.addColor(VisualMenuVars::vars.cWhitelisted)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the ESP color for clients you have \nwhitelisted from being targeted."));

	addOption(ColorOption("Visible")
		.addPreviewBox()
		.addColor(VisualMenuVars::vars.cVisible)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the ESP color when a target is \nvisible."));
}

/*Update once when submenu is opened*/
void VisualCustomizationMenu::UpdateOnce() {}

/*Update while submenu is active*/
void VisualCustomizationMenu::Update() {}

/*Background update*/
void VisualCustomizationMenu::FeatureUpdate() {}

/*Singleton*/
VisualCustomizationMenu* _instance;
VisualCustomizationMenu* VisualCustomizationMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new VisualCustomizationMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
VisualCustomizationMenu::~VisualCustomizationMenu() { delete _instance; }