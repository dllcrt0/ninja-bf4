#pragma once

typedef struct _XBLSMailMan {
	DWORD dwBoxes;
	pXBLSMsgBox NINJA;
} XBLSMailMan, *pXBLSMailMan;

static HRESULT FindMailMan(PVOID *out) {
	// Allocate some memory for stuff
	return XamAllocEx(0x20190000, 0x10000000, 34, out);
}

static pXBLSMsgBox MailManGetBox(DWORD Ident) {
	pXBLSMailMan MM = NULL;
	if (FindMailMan((PVOID*)&MM) == ERROR_SUCCESS) {
		//If only ninja registered, return..
		if (MM->dwBoxes <= 1)
			return NULL;

		BOOL hasFound = FALSE;
		for (DWORD i = 0; i < MM->dwBoxes - 1; i++) {
			DWORD iter = (DWORD)((DWORD)MM + sizeof(_XBLSMailMan) + (DWORD)(i * sizeof(DWORD)));
			pXBLSMsgBox tmpBox = (pXBLSMsgBox)(*(DWORD*)iter);

			if (tmpBox != NULL && tmpBox->Identifier == Ident) {
				DbgPrint("Ident: %08X", tmpBox->Identifier);
				return tmpBox;
			}
		}
	}

	return NULL;
}