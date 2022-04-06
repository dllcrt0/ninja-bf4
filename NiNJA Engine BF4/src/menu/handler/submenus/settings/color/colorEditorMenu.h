#pragma once
#include "menu/handler/submenu.h"

class ColorEditorMenu : public Submenu {
public:
	static ColorEditorMenu* GetInstance();
	void Init() override;
	void UpdateOnce() override;
	void Update() override;
	void FeatureUpdate() override;

	ColorEditorMenu() : Submenu() {}
	~ColorEditorMenu();
};

namespace ColorEditorMenuVars
{
	struct Vars {
		GColor ColorValue;
		GColor* pToUpdate;
	};

	extern Vars vars;
}