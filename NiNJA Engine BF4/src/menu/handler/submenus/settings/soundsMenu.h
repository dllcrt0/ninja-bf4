#pragma once
#include "menu/handler/submenu.h"

class SoundsMenu : public Submenu {
public:
	static SoundsMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	SoundsMenu() : Submenu() {}
	~SoundsMenu();
};

namespace SoundsMenuVars
{
	struct Vars {
		bool bKillingSpree;
		bool bKillingStreak;

		int iOnKill;
		int iOnDeath;

		vector<string> vFilenames;
	};

	extern ScrollStruct<int> SoundsTest[100];
	extern Vars vars;
}