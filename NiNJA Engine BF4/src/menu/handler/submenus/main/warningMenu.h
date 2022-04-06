#pragma once
#include "menu/handler/submenu.h"

class WarningMenu : public Submenu {
public:
	static WarningMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	WarningMenu() : Submenu() {}
	~WarningMenu();
};

namespace WarningMenuVars
{
	struct Vars {
		int iAimAngle;
		int iProximity;
		int iVisible;
		int iDrawIndex;
	};

	extern Vars vars;
}