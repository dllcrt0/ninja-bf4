#pragma once

enum XBLS_CMDS : DWORD {
	AUTH = 0x1,
	PATCHES = 0x2,
	PING = 0x4
};

typedef struct _Msg {
	XBLS_CMDS CMD;
	DWORD Identifier;
	DWORD* Data;
	DWORD Size;
	_Msg* nextMsg;
} XBLSMsg, *pXBLSMsg;

typedef struct _MsgBox {
	DWORD Identifier;
	pXBLSMsg Inbox;
	pXBLSMsg Outbox;
} XBLSMsgBox, *pXBLSMsgBox;

enum XBLSMSGR_IDENTS : DWORD {
	NINJA_IDENT = 'NINJ'
};

#define MAX_MSG 255

class XBLSMsgr {
public:
	XBLSMsgBox* pMsgBox;
	bool bIsRunning;

	DWORD dwMyIdent;

	NTSTATUS CheckInbox();

	bool IsReady() {
		return (pMsgBox != NULL && bIsRunning);
	}

	pXBLSMsg* GetLastOutboxMsg() {
		if (!IsReady())
			return NULL;
		pXBLSMsg* tmp = &pMsgBox->Outbox;
		for (int i = 0; i < MAX_MSG && *tmp != NULL; i++) {
			tmp = &(*tmp)->nextMsg;
		}
		return tmp;
	}

	BOOL SendMsg(pXBLSMsg Msg);

	VOID PopInbox() {
		if (pMsgBox->Inbox == NULL)
			return;
		pXBLSMsg tmp = pMsgBox->Inbox->nextMsg;
		//free(pMsgBox->Inbox);
		if (tmp != NULL)
			pMsgBox->Inbox = tmp;
		else
			pMsgBox->Inbox = NULL;
	}

	BOOL SendRecvMsg(XBLS_CMDS cmd);
	BOOL InitPatches();

	BOOL WaitForResponse() {
		for (int i = 0; (!IsReady() || pMsgBox->Inbox == NULL) && i < 20000; i += 1000) {
			Sleep(1000);
		}
		return pMsgBox->Inbox != NULL;
	}

	VOID SetMsgBox(pXBLSMsgBox mBox) {
		if (mBox == NULL)
			return;
		pMsgBox = mBox;
		bIsRunning = TRUE;
	}

	XBLSMsgr() {
		bIsRunning = FALSE;
		pMsgBox = NULL;
		dwMyIdent = __MSGRIDENT__;
	}

	~XBLSMsgr() {}
};