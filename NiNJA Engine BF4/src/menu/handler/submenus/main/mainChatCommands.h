#pragma once
#include "menu/handler/submenu.h"

class MainChatCommandsMenu : public Submenu {
public:
	static MainChatCommandsMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	MainChatCommandsMenu() : Submenu() {}
	~MainChatCommandsMenu();
};

namespace MainChatCommandsMenuVars
{
	struct Vars {
		bool bCustomKillFeedKill;
		bool bCustomKillFeedDeath;

		int iChannelType;
		int iMessageType;
	};

	extern Vars vars;
}