#include "stdafx.h"

Messages pMessages;

void Messages::SendTextMessage(ChatChannelType channelType, ChatChannelType messageType, const char* message) {
	DbgPrint("%s", message);
	if (ValidPtr(pGame.GetClientGameContext())) {
		if (ValidPtr(pGame.GetClientGameContext()->pOnlineManager)) {
			if (ValidPtr(pGame.GetClientGameContext()->pOnlineManager->pChatManagerFirst)) {
				auto manager = *(DWORD*)((pGame.GetClientGameContext()->pOnlineManager->pChatManagerFirst->dwPtr - 0x10) + 0x2090);

				__int64 mask = -1;
				switch (channelType) {
					case CctSayAll: {
						DbgPrint("Sending text message to everyone - %s", message);
						break;
					}

					case CctTeam: {
						DbgPrint("Sending text message to team - %s", message);
						mask = pGame.GetLocalPlayer()->nTeamID;
						break;
					}

					case CctSquad: {
						DbgPrint("Sending text message to squad - %s", message);
						mask = ((pGame.GetLocalPlayer()->nTeamID << 5) + pGame.GetLocalPlayer()->nTeamID) + *(DWORD*)((DWORD)pGame.GetLocalPlayer() + 0x720) + 0x11;
						break;
					}
				}

				int textSize = strlen(message) + 1;
				wchar_t* wc = new wchar_t[textSize];
				mbstowcs(wc, message, textSize);

				Invoke::Call<int>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwSendTextMessage), manager, messageType, mask, wc); // 4 = admin

				//delete[] wc;
			}
		}
	}
}