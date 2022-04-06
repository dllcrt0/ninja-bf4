#pragma once

class SystemHooks {
public:
	bool bInitialized;

	bool Apply();
	void Remove();

	SystemHooks();
};