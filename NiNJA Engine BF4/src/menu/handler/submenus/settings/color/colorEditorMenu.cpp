#include "stdafx.h"
#include "colorEditorMenu.h"
#include "../colorMenu.h"

using namespace ColorEditorMenuVars;

namespace ColorEditorMenuVars {
	Vars vars;

	void UpdateIt() {
		if (vars.pToUpdate) {
			*vars.pToUpdate = vars.ColorValue;
			vars.pToUpdate->_ = vars.pToUpdate->A;
		}
	}
}

void ColorEditorMenu::Init() {
	SetParentSubmenu<ColorMenu>();
	SetName("");

	addOption(NumberOption<float>(SCROLL, "Red")
		.addNumber(vars.ColorValue.R, "%.2f", 0.01f).addMin(0.f).addMax(1.0f).setScrollSpeed(20)
		.addFunction(UpdateIt)
		.addTooltip("Choose the amount of red in the final color."));

	addOption(NumberOption<float>(SCROLL, "Green")
		.addNumber(vars.ColorValue.G, "%.2f", 0.01f).addMin(0.f).addMax(1.0f).setScrollSpeed(20)
		.addFunction(UpdateIt)
		.addTooltip("Choose the amount of green in the final \ncolor."));

	addOption(NumberOption<float>(SCROLL, "Blue")
		.addNumber(vars.ColorValue.B, "%.2f", 0.01f).addMin(0.f).addMax(1.0f).setScrollSpeed(20)
		.addFunction(UpdateIt)
		.addTooltip("Choose the amount of blue in the final \ncolor."));

	addOption(NumberOption<float>(SCROLL, "Alpha")
		.addNumber(vars.ColorValue.A, "%.2f", 0.01f).addMin(0.f).addMax(1.0f).setScrollSpeed(20)
		.addFunction(UpdateIt)
		.addTooltip("Choose the transparency of the final color."));

	addOption(ColorOption("Preview")
		.addPreviewBox()
		.addColor(vars.ColorValue)
		.addTooltip("Shows what the color will look like before \nyou exit the menu."));
}

/*Update once when submenu is opened*/
void ColorEditorMenu::UpdateOnce() {}

/*Update while submenu is active*/
void ColorEditorMenu::Update() {}

/*Background update*/
void ColorEditorMenu::FeatureUpdate() {}

/*Singleton*/
ColorEditorMenu* _instance;
ColorEditorMenu* ColorEditorMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new ColorEditorMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
ColorEditorMenu::~ColorEditorMenu() { delete _instance; }