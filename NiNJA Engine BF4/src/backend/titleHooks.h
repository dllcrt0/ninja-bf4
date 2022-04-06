#pragma once

namespace TitleHooks {
	extern bool bInitialized;

	bool Apply();
	void Remove(bool bOverride = false);
};