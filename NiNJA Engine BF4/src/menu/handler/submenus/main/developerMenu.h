#pragma once
#include "menu/handler/submenu.h"

class DeveloperMenu : public Submenu {
public:
	static DeveloperMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	DeveloperMenu() : Submenu() {}
	~DeveloperMenu();
};

namespace DeveloperMenuVars
{
	struct Vars {
	};

	extern Vars vars;
}