#pragma once
#include "menu/handler/submenu.h"

class ClientMenu : public Submenu {
public:
	static ClientMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ClientMenu() : Submenu() {}
	~ClientMenu();
};

namespace ClientMenuVars
{
	struct UserData {
		int iID;
		const char* pName;

		UserData(int id, const char* name)
			: iID(id), pName(name) {
		}
	};

	struct Vars {
		bool bNeedsToSort;
		vector<UserData> Players;
		int iSelectedClient;
	};

	extern Vars vars;
}