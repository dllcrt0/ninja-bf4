#pragma once
#include "menu/handler/submenu.h"

class SpotMenu : public Submenu {
public:
	static SpotMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	SpotMenu() : Submenu() {}
	~SpotMenu();
};

namespace SpotMenuVars
{
	struct Vars {
		DWORD dwPlayerComponents[100];
		SpotMessage* pMessage;
		
		bool bAlwaysSpot;

		int iSpotType;
		int iCount;
	};

	extern Vars vars;
}