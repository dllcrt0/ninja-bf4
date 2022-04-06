#pragma once
#include "menu/handler/submenu.h"

class VisualCustomizationMenu : public Submenu {
public:
	static VisualCustomizationMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	VisualCustomizationMenu() : Submenu() {}
	~VisualCustomizationMenu();
};

namespace VisualCustomizationMenuVars
{
	struct Vars {

	};

	extern Vars vars;
}