#include "stdafx.h"

BOOL XBLSMsgr::InitPatches() {
	DbgPrint("InitPatches: Function Start");

	if (!IsReady()) {
		DbgPrint("InitPatches: ERR - [MSGR] not ready");
		return FALSE;
	}

	if (Not(NiNJA.pPatches)->bHasPatches) {
		DbgPrint("InitPatches: ERR - Already init");
		return FALSE;
	}

	//Request patches to be filled from server
	if (!SendRecvMsg(PATCHES)) {
		DbgPrint("InitPatches: ERR - Message exchange failed");
		return FALSE;
	}

	if (!Not(NiNJA.pPatches)->bHasPatches) {
		DbgPrint("InitPatches: ERR - MSG Exchange Success but patches not loaded");
		return FALSE;
	}

	DbgPrint("InitPatches: Success!");
	return TRUE;
}

BOOL XBLSMsgr::SendRecvMsg(XBLS_CMDS cmd) {
	if (!IsReady()) {
		return FALSE;
	}

	pXBLSMsg nMsg = new XBLSMsg();
	nMsg->nextMsg = NULL;
	nMsg->CMD = cmd;
	nMsg->Identifier = dwMyIdent;
	nMsg->Data = NULL;

	//Send msg and wait for response
	if (!SendMsg(nMsg) || !WaitForResponse()) {
		DbgPrint("[MSGR]: SendRecvMsg Send ERR");
		return FALSE;
	}

	//Receive Data
	if (FAILED(CheckInbox())) {
		DbgPrint("[MSGR]: SendRecvMsg Recv ERR");
		return FALSE;
	}

	return TRUE;
}

BOOL XBLSMsgr::SendMsg(pXBLSMsg Msg) {
	if (!IsReady()) {
		return FALSE;
	}
	if (Msg == NULL)
		return FALSE;

	Msg->nextMsg = pMsgBox->Outbox;
	pMsgBox->Outbox = Msg;

	return TRUE;
}

NTSTATUS XBLSMsgr::CheckInbox() {
	if (!IsReady())
		return E_FAIL;

	if (pMsgBox->Inbox == NULL)
		return ERROR_SUCCESS;

	pXBLSMsg nMsg = NULL;
	DWORD size = 0;
	PBYTE pbPatches = NULL;

	if (pMsgBox->Inbox->Identifier == __MSGRIDENT__) {
		DbgPrint("[MSGR]: Received message!");
	} else {
		DbgPrint("[MSGR]: ERR: CheckInbox, wrong ident");
		PopInbox();
		return E_FAIL;
	}

	switch (pMsgBox->Inbox->CMD) {
	case PATCHES:
		DbgPrint("[MSGR]: Parsing patch data message");

		if (pMsgBox->Inbox->Data == NULL || pMsgBox->Inbox->Size == 0) {
			DbgPrint("[MSGR]: ERR: Message sanity check fail");
			return E_FAIL;
		}
		DbgPrint("Copying data to patches");

		//Install patch data
		memcpy((PVOID)&Not(NiNJA.pPatches)->BF4, (PBYTE)pMsgBox->Inbox->Data, sizeof(CheatPatches));

		Not(NiNJA.pPatches)->bHasPatches = true;

		DbgPrint("Installed patch data successfully");

		//Clean Up
		PopInbox();
		break;

	default:
		return E_FAIL;
		break;
	}

	return ERROR_SUCCESS;
}