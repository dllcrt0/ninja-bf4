#include "stdafx.h"
#include "colorMenu.h"
#include "../settingsMenu.h"
#include "color/colorEditorMenu.h"

using namespace ColorMenuVars;

namespace ColorMenuVars {
	Vars vars;
	
	void ColorSubCallback() {
		ColorEditorMenu::GetInstance()->SetParentSubmenu<ColorMenu>();
		ColorEditorMenu::GetInstance()->SetName(ColorMenu::GetInstance()->GetOptions()[pMenu.iCurrentOption]->GetName());

		switch (pMenu.iCurrentOption) {
		case 0: // Background
			ColorEditorMenuVars::vars.ColorValue = pRenderer.UIColors.menuBackground;
			ColorEditorMenuVars::vars.pToUpdate = &pRenderer.UIColors.menuBackground;
			break;
		case 1: // Border
			ColorEditorMenuVars::vars.ColorValue = pRenderer.UIColors.border;
			ColorEditorMenuVars::vars.pToUpdate = &pRenderer.UIColors.border;
			break;
		case 2: // Description Text
			ColorEditorMenuVars::vars.ColorValue = pRenderer.UIColors.descriptionText;
			ColorEditorMenuVars::vars.pToUpdate = &pRenderer.UIColors.descriptionText;
			break;
		case 3: // Scrollbar
			ColorEditorMenuVars::vars.ColorValue = pRenderer.UIColors.scrollbar;
			ColorEditorMenuVars::vars.pToUpdate = &pRenderer.UIColors.scrollbar;
			break;
		case 4: // Selected Text
			ColorEditorMenuVars::vars.ColorValue = pRenderer.UIColors.selectedText;
			ColorEditorMenuVars::vars.pToUpdate = &pRenderer.UIColors.selectedText;
			break;
		case 5: // Text
			ColorEditorMenuVars::vars.ColorValue = pRenderer.UIColors.text;
			ColorEditorMenuVars::vars.pToUpdate = &pRenderer.UIColors.text;
			break;
		case 6: // Title Text
			ColorEditorMenuVars::vars.ColorValue = pRenderer.UIColors.titleText;
			ColorEditorMenuVars::vars.pToUpdate = &pRenderer.UIColors.titleText;
			break;
		case 7: // Top/Bottom
			ColorEditorMenuVars::vars.ColorValue = pRenderer.UIColors.topBottom;
			ColorEditorMenuVars::vars.pToUpdate = &pRenderer.UIColors.topBottom;
			break;
		case 8: // Slider
			ColorEditorMenuVars::vars.ColorValue = pRenderer.UIColors.slider;
			ColorEditorMenuVars::vars.pToUpdate = &pRenderer.UIColors.slider;
			break;
		}
	}
}

void ColorMenu::Init() {
	SetParentSubmenu<SettingsMenu>();
	SetName("Menu Colors");
	
	addOption(ColorOption("Background")
		.addPreviewBox()
		.addColor(pRenderer.UIColors.menuBackground)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the background color of the menu."));

	addOption(ColorOption("Border")
		.addPreviewBox()
		.addColor(pRenderer.UIColors.border)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the border color of anything that \nhas a border in the menu."));

	addOption(ColorOption("Description Text")
		.addPreviewBox()
		.addColor(pRenderer.UIColors.descriptionText)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the color of the menu option \ndescription text."));

	addOption(ColorOption("Scrollbar")
		.addPreviewBox()
		.addColor(pRenderer.UIColors.scrollbar)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the color of the menus scrollbar."));

	addOption(ColorOption("Selected Text")
		.addPreviewBox()
		.addColor(pRenderer.UIColors.selectedText)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the color of the menu text when an \noption is selected."));

	addOption(ColorOption("Text")
		.addPreviewBox()
		.addColor(pRenderer.UIColors.text)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the color of the menu text when \nnothing is selected."));

	addOption(ColorOption("Title Text")
		.addPreviewBox()
		.addColor(pRenderer.UIColors.titleText)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the color of the menus title text."));

	addOption(ColorOption("Top/Bottom")
		.addPreviewBox()
		.addColor(pRenderer.UIColors.topBottom)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the background color of the title \ntext, and description text."));

	addOption(ColorOption("Slider")
		.addPreviewBox()
		.addColor(pRenderer.UIColors.slider)
		.addSubmenu<ColorEditorMenu>()
		.addFunction(ColorSubCallback)
		.addTooltip("Choose the color of the menu slider."));
}

/*Update once when submenu is opened*/
void ColorMenu::UpdateOnce() {}

/*Update while submenu is active*/
void ColorMenu::Update() {}

/*Background update*/
void ColorMenu::FeatureUpdate() {}

/*Singleton*/
ColorMenu* _instance;
ColorMenu* ColorMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ColorMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
ColorMenu::~ColorMenu() { delete _instance; }