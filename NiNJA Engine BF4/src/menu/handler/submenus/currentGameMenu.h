#pragma once
#include "menu/handler/submenu.h"

class CurrentGameMenu : public Submenu {
public:
	static CurrentGameMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	CurrentGameMenu() : Submenu() {}
	~CurrentGameMenu();
};

namespace CurrentGameMenuVars
{
	struct Vars {
		bool bInvisibility;
	};

	extern Vars vars;
}