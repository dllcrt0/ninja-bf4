#pragma once

class Game {
public:
	bool bInitialized;

	bool bJustShot;
	bool bReload;

	bool Initialize();
	void Run();
	void Shutdown();
	bool CanRun();
	void DownloadSoundPacks();
	void DownloadChangelog();

	Game();

	char szFormatStringBuffer[0x200];
	ClientPlayerScoreManager* pClientPlayerScoreManager;
	ClientSoldierEntity* pAimbotSoldier;
	TargetInformation AimbotTargetInfo;
	DWORD dwRaycastHandle;
	bool bShowMessageBox;
	bool bPaused;
	bool bCanRun;
	Vec2 vOldAngles;
	int iKillFeedRand[2];
	bool bIsUsingRailgun;

	UIRectInfo* GetUIRectInfo();
	ServerPeerManager* GetServerPeerManager();
	ClientGameContext* GetClientGameContext();
	DWORD GetClientGameContextPointer();
	UIEngine* GetUIEngine();
	GameRenderer* GetGameRenderer();
	SyncedBFSettings* GetSyncedBFSettings();
	ButtonSetup* GetButtonSetup();
	ClientPlayer* GetLocalPlayer();
	ClientSoldierWeapon* GetSoldierWeapon();
	ClientSoldierEntity* GetMySoldierEntity();
	ResourceManager* GetResourceManager();

	bool WorldToScreen(Vec3 world, Vec3& out);
	Vec2 BaseAimCorrection();
	char GetDamageID();
	void HealTeam();
	void ExecuteMessage(void* pManager, Message* pMessage);
	void DispatchMessage(void* pManager, Message* pMessage);
	void MessageConstruct(Message* pMessage, int iCategory, int iType);
	void ExecuteNetworkableMessage(void* pClientConnection, NetworkableMessage* message);
	void SendClientDamageMessage(void* pHealthComp, char damageId, ClientDamageInfo* pDamageMessage, int empty, ClientPlayer* pTarget, int empty2, float damage);
	void MessageBoxAccept(const char* pTitle, const char* pDesc, UIPopupType PopupType);
	void SendOrder(ClientPlayer* pWhoSentTheOrder, OrderType orderType);
};

extern Game pGame;