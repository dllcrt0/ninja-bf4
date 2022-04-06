#pragma once
#include "menu/handler/submenu.h"

class MovementMenu : public Submenu {
public:
	static MovementMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MovementMenu() : Submenu() {}
	~MovementMenu();
};

namespace MovementMenuVars
{
	struct Vars {
		bool bTeleportToCrosshair;
		bool bTeleportAbove;
		bool bFlyHack;

		int iTeleportAboveHeight;

		float fFlyHackSpeed;
	};

	extern Vars vars;
}