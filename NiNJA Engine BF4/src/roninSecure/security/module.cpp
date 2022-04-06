#include "stdafx.h"

void Module::RemoveModuleFromLinkedList(void* pModule) {
	LDR_DATA_TABLE_ENTRY* pTable = (LDR_DATA_TABLE_ENTRY*)GetModuleHandle("xboxkrnl.exe");
	LIST_ENTRY* pCurrentEntry = pTable->InLoadOrderLinks.Flink;
	LDR_DATA_TABLE_ENTRY* pCurrentTable = nullptr;

	while (pCurrentEntry != &pTable->InLoadOrderLinks && pCurrentEntry != nullptr) {
		pCurrentTable = CONTAINING_RECORD(pCurrentEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

		if (pCurrentTable) {
			if (pCurrentTable->ImageBase == pModule) {
				Links savedLink;
				savedLink.pModule = pModule;
				savedLink.InLoadOrderLinks_Flink_Blink = pCurrentTable->InLoadOrderLinks.Flink->Blink;
				savedLink.InLoadOrderLinks_Blink_Flink = pCurrentTable->InLoadOrderLinks.Blink->Flink;
				savedLink.InInitializationOrderLinks_Flink_Blink = pCurrentTable->InInitializationOrderLinks.Flink->Blink;
				savedLink.InInitializationOrderLinks_Blink_Flink = pCurrentTable->InInitializationOrderLinks.Blink->Flink;
				savedLink.InClosureOrderLinks_Flink_Blink = pCurrentTable->InClosureOrderLinks.Flink->Blink;
				savedLink.InClosureOrderLinks_Blink_Flink = pCurrentTable->InClosureOrderLinks.Blink->Flink;
				SavedLinks.push_back(savedLink);

				pCurrentTable->InLoadOrderLinks.Flink->Blink = pCurrentTable->InLoadOrderLinks.Blink;
				pCurrentTable->InLoadOrderLinks.Blink->Flink = pCurrentTable->InLoadOrderLinks.Flink;
				pCurrentTable->InInitializationOrderLinks.Flink->Blink = pCurrentTable->InInitializationOrderLinks.Blink;
				pCurrentTable->InInitializationOrderLinks.Blink->Flink = pCurrentTable->InInitializationOrderLinks.Flink;
				pCurrentTable->InClosureOrderLinks.Flink->Blink = pCurrentTable->InClosureOrderLinks.Blink;
				pCurrentTable->InClosureOrderLinks.Blink->Flink = pCurrentTable->InClosureOrderLinks.Flink;
				break;
			}
		}


		pCurrentEntry = pCurrentEntry->Flink;
	}
}

void Module::RelinkModuleToLinkedList(void* pModule) {
	LDR_DATA_TABLE_ENTRY* pTable = (LDR_DATA_TABLE_ENTRY*)GetModuleHandle("xboxkrnl.exe");
	LIST_ENTRY* pCurrentEntry = pTable->InLoadOrderLinks.Flink;
	LDR_DATA_TABLE_ENTRY* pCurrentTable = nullptr;

	while (pCurrentEntry != &pTable->InLoadOrderLinks && pCurrentEntry != nullptr) {
		pCurrentTable = CONTAINING_RECORD(pCurrentEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

		if (pCurrentTable) {
			if (pCurrentTable->ImageBase == pModule) {
				for (int i = 0; i < SavedLinks.size(); i++) {
					if (SavedLinks[i].pModule == pModule) {
						pCurrentTable->InLoadOrderLinks.Flink->Blink = SavedLinks[i].InLoadOrderLinks_Flink_Blink;
						pCurrentTable->InLoadOrderLinks.Blink->Flink = SavedLinks[i].InLoadOrderLinks_Blink_Flink;
						pCurrentTable->InInitializationOrderLinks.Flink->Blink = SavedLinks[i].InInitializationOrderLinks_Flink_Blink;
						pCurrentTable->InInitializationOrderLinks.Blink->Flink = SavedLinks[i].InInitializationOrderLinks_Blink_Flink;
						pCurrentTable->InClosureOrderLinks.Flink->Blink = SavedLinks[i].InClosureOrderLinks_Flink_Blink;
						pCurrentTable->InClosureOrderLinks.Blink->Flink = SavedLinks[i].InClosureOrderLinks_Blink_Flink;
						break;
					}
				}

				break;
			}
		}

		pCurrentEntry = pCurrentEntry->Flink;
	}
}

vector<LinkedListModules> Module::GetModulesInLinkedList() {
	vector<LinkedListModules> linked;

	LDR_DATA_TABLE_ENTRY* pTable = (LDR_DATA_TABLE_ENTRY*)GetModuleHandle("xboxkrnl.exe");
	LIST_ENTRY* pCurrentEntry = pTable->InLoadOrderLinks.Flink;
	LDR_DATA_TABLE_ENTRY* pCurrentTable = nullptr;

	while (pCurrentEntry != &pTable->InLoadOrderLinks && pCurrentEntry != nullptr) {
		pCurrentTable = CONTAINING_RECORD(pCurrentEntry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

		if (pCurrentTable) {
			auto name = pCurrentTable->BaseDllName.Buffer;
			if (!name) {
				goto jNext;
			}

			LinkedListModules module;
			module.dwBaseAddress = (DWORD)pCurrentTable->ImageBase;
			module.dwCheckSum = pCurrentTable->CheckSum;
			module.dwEntryPointAddress = (DWORD)pCurrentTable->EntryPoint;
			module.dwSize = pCurrentTable->SizeOfFullImage;
			module.dwXexHeaderBase = (DWORD)pCurrentTable->XexHeaderBase;
			
			wcstombs(module.szName, name, sizeof(module.szName));

			if (strlen(module.szName)) {
				// quick sanity...
				module.hModule = GetModuleHandleA(module.szName);
			}

			linked.push_back(module);
		}

	jNext:
		pCurrentEntry = pCurrentEntry->Flink;
	}

	return linked;
}