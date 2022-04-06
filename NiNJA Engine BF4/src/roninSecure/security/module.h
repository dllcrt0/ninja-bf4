#pragma once

struct LinkedListModules {
	DWORD dwBaseAddress;
	DWORD dwEntryPointAddress;
	DWORD dwSize;
	DWORD dwXexHeaderBase;
	DWORD dwCheckSum;
	HMODULE hModule;
	char szName[40];
};

struct Links {
	void* pModule;
	_LIST_ENTRY* InLoadOrderLinks_Flink_Blink;
	_LIST_ENTRY* InLoadOrderLinks_Blink_Flink;
	_LIST_ENTRY* InInitializationOrderLinks_Flink_Blink;
	_LIST_ENTRY* InInitializationOrderLinks_Blink_Flink;
	_LIST_ENTRY* InClosureOrderLinks_Flink_Blink;
	_LIST_ENTRY* InClosureOrderLinks_Blink_Flink;
};

class Module {
public:
	void RemoveModuleFromLinkedList(void* pModule);
	void RelinkModuleToLinkedList(void* pModule);
	vector<LinkedListModules> GetModulesInLinkedList();

	vector<Links> SavedLinks;
};