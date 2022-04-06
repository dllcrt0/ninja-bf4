#pragma once
#include "menu/handler/submenu.h"

class SelectedClientMenu : public Submenu {
public:
	static SelectedClientMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	SelectedClientMenu() : Submenu() {}
	~SelectedClientMenu();
};

namespace SelectedClientMenuVars
{
	struct Vars {
		int iCallout;
	};

	extern Vars vars;
}