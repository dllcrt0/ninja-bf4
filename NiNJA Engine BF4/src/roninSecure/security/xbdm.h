#pragma once

struct XboxDebugManagerCommandTableRetail {
	const char* pCommandName;
	DWORD dwUnk1;
	void* pCallback;

	friend bool operator==(XboxDebugManagerCommandTableRetail& left, const char* right) {
		return !strcmp(left.pCommandName, right);
	}
};

class XboxDebugManager {
public:
	void InitializeCommands();
	XboxDebugManagerCommandTableRetail* GetCommandInformationFromName(const char* pName);
	void UnhookAll();
	
	template <typename T>
	void HookCommand(const char* pName, void* pFunction, T* pTrampoline) {
		auto entry = GetCommandInformationFromName(pName);

		if (entry) {
			Hooks.push_back(make_pair(entry, entry->pCallback));
			*pTrampoline = (T)entry->pCallback;
			DbgPrint("Hooked XBDM command %s - 0x%X to 0x%X", pName, entry->pCallback, pFunction);
			entry->pCallback = pFunction;
		}
	}
private:
	vector<XboxDebugManagerCommandTableRetail*> CommandTable;
	vector<pair<XboxDebugManagerCommandTableRetail*, void*>> Hooks;
};