#include "stdafx.h"

void XboxDebugManager::InitializeCommands() {
	// seeing how public kits won't be using any cheats, it doesn't matter much about this being
	// set up for only retails.

	for (int i = 0; i < 63; i++) {
		CommandTable.push_back((XboxDebugManagerCommandTableRetail*)(0x9101ABF8 + (i * sizeof(XboxDebugManagerCommandTableRetail))));
	}
}

XboxDebugManagerCommandTableRetail* XboxDebugManager::GetCommandInformationFromName(const char* pName) {
	for (int i = 0; i < CommandTable.size(); i++) {
		if (!strcmp(CommandTable[i]->pCommandName, pName)) {
			return CommandTable[i];
		}
	}

	return nullptr;
}

void XboxDebugManager::UnhookAll() {
	for (int i = 0; i < Hooks.size(); i++) {
		DbgPrint("Unhooked: Address=0x%X", &Hooks[i].first->pCallback);
		Hooks[i].first->pCallback = Hooks[i].second;
	}
}