#pragma once
#include "menu/handler/submenu.h"

class ColorMenu : public Submenu {
public:
	static ColorMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ColorMenu() : Submenu() {}
	~ColorMenu();
};

namespace ColorMenuVars
{
	struct Vars {

	};

	extern Vars vars;
}