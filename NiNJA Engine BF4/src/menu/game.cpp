#include "stdafx.h"
#include "menu/handler/submenus/aimbotMenu.h"
#include "menu/handler/submenus/weaponMenu.h"
#include "menu/handler/submenus/main/loadoutEditorMenu.h"
#include <cmath>

Game pGame;

static char* pServerNames[] = {
	"arnold.xbl.ninja",
	"bernard.xbl.ninja",
	"charlie.xbl.ninja",
	"ford.xbl.ninja",
	"sylvester.xbl.ninja",
	"felix.xbl.ninja",
	"dolores.xbl.ninja",
	"elsie.xbl.ninja"
};

static char* GetRandomSubDomain() {
	srand(time(0));
	int iValue = rand() % 8;
	if (iValue > 7) {
		iValue = 0;
	}
	return pServerNames[iValue];
}

Game::Game() {
	bInitialized = false;
}

namespace Polynomial {
#define     EQN_EPS     1e-9
#define	    IsZero(x)	((x) > -EQN_EPS && (x) < EQN_EPS)
#define PI 3.141592654

	int SolveQuadratic(float a, float b, float c, std::vector<double>& Sol) {
		double Determinant = b * b - 4 * a*c;
		if (Determinant > 0) {
			double X = (-b + std::sqrt(Determinant)) / (2 * a);
			double X2 = (-b - std::sqrt(Determinant)) / (2 * a);
			Sol.push_back(X);
			Sol.push_back(X2);
			return 2;
		}

		if (IsZero(Determinant)) {
			double X = (-b + std::sqrt(Determinant)) / (2 * a);
			Sol.push_back(X);
			return 2;
		}

		//Only 1 real solution
		double X = -b / (2 * a);
		Sol.push_back(X);
		return 1;
	}

	int SolveCubic(float a, float b, float c, float d, std::vector<double>& Sol) {
		double A = b / a;
		double B = c / a;
		double C = d / a;

		double ASq = A * A;
		double p = 1.0f / 3.0f * (-1.0f / 3.0f * ASq + B);
		double q = 1.0f / 2.0f * (2.0f / 27.0f * A * ASq - 1.0f / 3.0f * A * B + C);

		double PCube = p * p*p;
		double D = q * q + PCube;

		double Sub = 1.0f / 3.0f * A;

		if (IsZero(D)) {
			if (IsZero(q)) {
				Sol.push_back(0 - Sub);
				return 1;
			} else {
				double u = pow(-q, 1 / 3.);
				Sol.push_back(2.0f * u - Sub);
				Sol.push_back(-u - Sub);
				return 2;
			}
		}

		if (D < 0) {
			double phi = 1.0f / 3.0f * acos(-q / sqrt(-PCube));
			double t = 2.0f * sqrt(-p);

			Sol.push_back((t*cos(phi)) - Sub);
			Sol.push_back((-t * cos(phi + PI / 3.0f)) - Sub);
			Sol.push_back((-t * cos(phi - PI / 3.0f)) - Sub);
			return 3;
		}

		double DSqrt = sqrt(D);
		double u = pow(DSqrt - q, 1 / 3.);
		double v = -pow(DSqrt + q, 1 / 3.);
		Sol.push_back(u + v - Sub);
		return 1;
	}

	int SolveQuartic(float a, float b, float c, float d, float e, std::vector<double>& Sol) {
		//  A            B           C          D         E
		//c[4] * x^4 + c[3] * x^3 + c[2] *x^2 + c[1] *x + c[0]
		double A = b / a;
		double B = c / a;
		double C = d / a;
		double D = e / a;

		/* substitute x = y - A/4 to eliminate cubic term:
		x^4 + px^2 + qx + r = 0 */
		double ASqr = A * A;
		double p = -3.0f / 8.0f *ASqr + B;
		double q = 1.0f / 8.0f *ASqr*A - 1.0f / 2.0f *A*B + C;
		double r = -3.0f / 256.0f *ASqr*ASqr + 1.0f / 16.0f*ASqr*B - 1.0f / 4.0f*A*C + D;
		double sub = 1.0f / 4.0f *A;

		if (IsZero(r)) {
			/*no absolute term: y(y^3 + py + q) = 0 */
			std::vector<double> QSol;
			int SolCount = SolveCubic(1, 0, p, q, QSol);
			for (int i = 0; i < SolCount; i++) {
				Sol.push_back(QSol[i] - sub);
			}
			Sol.push_back(0 - sub);
			return SolCount + 1;
		}

		double td = 1.0f / 2 * r * p - 1.0f / 8.0f *q*q;
		double tc = -r;
		double tb = -1.0f / 2 * p;
		double ta = 1;

		std::vector<double> QSol;
		SolveCubic(ta, tb, tc, td, QSol);

		double z = QSol[0];
		double u = z * z - r;
		double v = 2.0f *z - p;

		if (IsZero(u))
			u = 0.0f;
		else if (u > 0.0f)
			u = sqrt(u);
		else
			return 0;

		if (IsZero(v))
			v = 0.0f;
		else if (v > 0.0f)
			v = sqrt(v);
		else
			return 0;

		tc = z - u;
		tb = q < 0 ? -v : v;
		ta = 1;

		std::vector<double> QSol2;
		int Sol2Num = SolveQuadratic(ta, tb, tc, QSol2);

		tc = z + u;
		tb = q < 0 ? v : -v;
		ta = 1;

		int Sol3Num = SolveQuadratic(ta, tb, tc, QSol2);
		int TotalSol = Sol2Num + Sol3Num;
		for (int i = 0; i < TotalSol; i++) {
			Sol.push_back(QSol2[i] - sub);
		}
		return TotalSol;
	}
}

Vec3 PredictAimingPos(Vec3 position, Vec3 target_position, Vec3 target_velocity, float bullet_velocity, float bullet_gravity) {
	Vec3 predicted_aiming_position = target_position;
	Vec3 p1 = target_position - position;
	double a = bullet_gravity * bullet_gravity * 0.25;
	double b = -bullet_gravity * target_velocity.y;
	double c = target_velocity.x * target_velocity.x + target_velocity.y * target_velocity.y + target_velocity.z * target_velocity.z - bullet_gravity * (target_position - position).y - bullet_velocity * bullet_velocity;
	double d = 2.0 * (p1.x * target_velocity.x + p1.y * target_velocity.y + p1.z * target_velocity.z);
	double e = p1.x * p1.x + p1.y * p1.y + p1.z * p1.z;

	vector<double> roots;
	auto num_roots = Polynomial::SolveQuartic(a, b, c, d, e, roots);

	if (num_roots > 0) {
		double hitTime = 0.0;
		for (int i = 0; i < num_roots; ++i) {
			if (roots[i] > 0.0 && (hitTime == 0.0 || roots[i] < hitTime))
				hitTime = roots[i];
		}

		if (hitTime > 0.0) {
			double hitVelX = p1.x / hitTime + target_velocity.x;
			double hitVelY = p1.y / hitTime + target_velocity.y - 0.5f * bullet_gravity * hitTime;
			double hitVelZ = p1.z / hitTime + target_velocity.z;

			predicted_aiming_position.x = (float)(position.x + hitVelX * hitTime);
			predicted_aiming_position.y = (float)(position.y + hitVelY * hitTime);
			predicted_aiming_position.z = (float)(position.z + hitVelZ * hitTime);
		}
	}

	return predicted_aiming_position;
}

Vec2 Game::BaseAimCorrection() {
	if (ValidPtr(pWeapon.GetBulletEntityData())) {
		if (ValidPtr(pWeapon.GetFiringFunctionData())) {
			pGame.AimbotTargetInfo.BonePosition = PredictAimingPos(pGame.GetGameRenderer()->pRenderView->ViewMatrixInverse.translation, pGame.AimbotTargetInfo.BonePosition, pGame.pAimbotSoldier->pPredictedController->Velocity, pWeapon.GetFiringFunctionData()->ShotConfig.InitialSpeed.z, pWeapon.GetBulletEntityData()->fGravity);
		}
	}

	if (ValidPtr(pGame.pAimbotSoldier)) {
		if (ValidPtr(pGame.pAimbotSoldier->GetParachuteCompOther())) {
			if (pGame.pAimbotSoldier->GetParachuteCompOther()->IsParachuting()) {
				pGame.AimbotTargetInfo.BonePosition.y -= (pUtils.GetDistance(pGame.GetGameRenderer()->pRenderView->ViewMatrixInverse.translation, pGame.AimbotTargetInfo.BonePosition) * 0.2f) / 100;
			} else if (pGame.pAimbotSoldier->GetParachuteCompOther()->IsFalling()) {
				pGame.AimbotTargetInfo.BonePosition.y -= (pUtils.GetDistance(pGame.GetGameRenderer()->pRenderView->ViewMatrixInverse.translation, pGame.AimbotTargetInfo.BonePosition) * 0.4f) / 100;
			}
		}
	}

	Vec3 direction = pGame.AimbotTargetInfo.BonePosition - pGame.GetGameRenderer()->pRenderView->ViewMatrixInverse.translation;
	direction.Normalize();
	return Vec2(-atan2(direction.x, direction.z), atan2(direction.y, direction.VectorLength2()));
}

void Game::DownloadSoundPacks() {
	vector<char*> toDownload;

	auto dir = GetWebDirectoryListing(GetRandomSubDomain(), "/authed/sounds/oof/");
	for (int i = 0; i < dir.size(); i++) {
		if (!pUtils.FileExists(pUtils.va("NINJA:\\ninjacache\\media\\sounds\\%s", dir[i]))) {
			toDownload.push_back(dir[i]);
		}
	}

	if (toDownload.size() > 0) {
		HttpRequest req;
		for (int i = 0; i < toDownload.size(); i++) {
			req.DownloadFile(GetRandomSubDomain(), pUtils.va("/authed/sounds/oof/%s", toDownload[i]), pUtils.va("NINJA:\\ninjacache\\media\\sounds\\%s", dir[i]), 11000, false);
		}
	}
}

void Game::DownloadChangelog() {
	auto ini = IniParse("NINJA:\\BF4-Engine.ini");
	if (ini.IsGood()) {
		HttpRequest req;
		if (req.DownloadFile(GetRandomSubDomain(), "/authed/changelog-bf4", "NINJA:\\ninjacache\\media\\changelog-bf4.txt", 11000, false)) {
			auto file = fopen("NINJA:\\ninjacache\\media\\changelog-bf4.txt", "r+");
			if (file) {
				fseek(file, 0, SEEK_END);
				auto size = ftell(file) - 1;
				fseek(file, 0, SEEK_SET);

				char* text = new char[size];
				fread(text, 1, size, file);

				char val[5];
				strncpy(val, text, 3);

				pConfig.iServerChangelogVersion = atoi(val);

				if (ini.ReadInt("Internal Settings", "Last Version", 0) != pConfig.iServerChangelogVersion) {
					strncpy(pConfig.szChangelogData, text + 4, size - 7);
					pConfig.bShouldShowChangelog = true;
				}

				delete[] text;
				fclose(file);
				remove("NINJA:\\ninjacache\\media\\changelog-bf4.txt");
			}
		}
	}
}

void Game::Run() {
	bCanRun = CanRun();

	if (bCanRun) {
		static bool seed = false;
		if (!seed) { seed = true; srand(time(0)); }

		iKillFeedRand[0] = rand() % 8;
		if (iKillFeedRand[0] > 7) {
			iKillFeedRand[0] = 0;
		}

		iKillFeedRand[1] = rand() % 5;
		if (iKillFeedRand[1] > 4) {
			iKillFeedRand[1] = 0;
		}

		bIsUsingRailgun = false;
	}

	if (pConfig.bLoadCurrentSettings) {
		pConfig.bLoadCurrentSettings = false;
		Sleep(1500);
		pConfig.LoadConfig("NINJA:\\BF4-Engine.ini");
	}

	if (pConfig.bSaveCurrentSettings) {
		pConfig.bSaveCurrentSettings = false;
		pConfig.UpdateCurrentSettings();
	}

	if (pConfig.bCreateConfigExt) {
		pConfig.bCreateConfigExt = false;

		pUtils.vaBuff(pConfig.szCreateConfigNameBuffer, 80, "NINJA:\\BF-Engine\\%s.ini", pConfig.pCreateConfigName.c_str());
		pConfig.CreateConfig(pConfig.szCreateConfigNameBuffer);
	}

	if (pConfig.bLoadConfigExt) {
		pConfig.bLoadConfigExt = false;

		pUtils.vaBuff(pConfig.szLoadConfigNameBuffer, 80, "NINJA:\\BF-Engine\\%s.ini", pConfig.pLoadConfigName.c_str());
		pConfig.LoadConfig(pConfig.szLoadConfigNameBuffer);
	}
}

bool Game::CanRun() {
	auto context = pGame.GetClientGameContext();
	auto player = GetLocalPlayer();

	if (!ValidPtr(context)) { return false; }
	if (!ValidPtr(player)) { return false; }

	if (!ValidPtr(context->pClientPlayerManager)) { return false; }
	if (!ValidPtr(player->GetClientSoldier()) && !ValidPtr(player->GetVehicleEntity())) { return false; }
	if (!ValidPtr(player->GetClientSoldier()->pSoldierWeaponsComponent) && !ValidPtr(player->GetVehicleEntity())) { return false; }

	return true;
}

bool Game::Initialize() {
	if (!XexExecutableModuleHandle) { DbgPrint("Failed 1"); return false; }
	if (!*XexExecutableModuleHandle) { DbgPrint("Failed 2"); return false; }

	PLDR_DATA_TABLE_ENTRY Title = *XexExecutableModuleHandle;

	DbgPrint("Game compiled on %i", Title->TimeDateStamp);
	if (Title->TimeDateStamp != __TIMEDATESTAMP__) {
		DbgPrint("Invalid game version!");
		return false;
	}

	// lets setup folders and what-not.
	pConfig.Initialize();

	if (!TitleHooks::Apply()) {
		DbgPrint("Failed to apply hooks! Removing any that were successful!");
		TitleHooks::Remove(true);
		return false;
	}

	if (!pSounds.Start()) {
		DbgPrint("Failed to initialize sound engine!");
		return false;
	}

	XNotifyQueueUI(XNOTIFYUI_TYPE_PREFERRED_REVIEW, 0, XNOTIFYUI_PRIORITY_HIGH, L"NiNJA Engine Loaded", NULL);

	DbgPrint("Game initialized!");
	NiNJA.bGameInitialized = true;
	bInitialized = true;
	return true;
}

void Game::Shutdown() {
	TitleHooks::Remove();

	NiNJA.bShutdownCheats = false;
	DbgPrint("Game uninitialized!");
}

ClientGameContext* Game::GetClientGameContext() {
	return *(ClientGameContext**)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwClientGameContext);
}

ServerPeerManager* Game::GetServerPeerManager() {
	if (*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwServerPeerManager)) {
		return *(ServerPeerManager**)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwServerPeerManager);
	}

	return nullptr;
}

UIRectInfo* Game::GetUIRectInfo() {
	return *(UIRectInfo**)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwRectPtr);
}

DWORD Game::GetClientGameContextPointer() {
	return *(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwClientGameContext);
}

UIEngine* Game::GetUIEngine() {
	return *(UIEngine**)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwUIEngine);
}

GameRenderer* Game::GetGameRenderer() {
	return *(GameRenderer**)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwGameRenderer);
}

SyncedBFSettings* Game::GetSyncedBFSettings() {
	return *(SyncedBFSettings**)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwSyncedBFSettings);
}

ButtonSetup* Game::GetButtonSetup() {
	return *(ButtonSetup**)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwButtonSetup);
}

ClientPlayer* Game::GetLocalPlayer() {
	auto context = GetClientGameContext();
	if (ValidPtr(context)) {
		if (ValidPtr(context->pClientPlayerManager)) {
			if (ValidPtr(context->pClientPlayerManager->pLocalPlayer)) {
				return context->pClientPlayerManager->pLocalPlayer;
			}
		}
	}

	return nullptr;
}

ClientSoldierEntity* Game::GetMySoldierEntity() {
	auto player = GetLocalPlayer();
	if (ValidPtr(player)) {
		if (player->pAttachedControllable)
			return nullptr;
		return (ClientSoldierEntity*)player->GetClientSoldier();
	}

	return nullptr;
}

ResourceManager* Game::GetResourceManager() {
	return *(ResourceManager**)0x89904630;
}

ClientSoldierWeapon* Game::GetSoldierWeapon() {
	auto soldier = GetMySoldierEntity();
	if (ValidPtr(soldier)) {
		if (ValidPtr(soldier->pSoldierWeaponsComponent)) {
			if (ValidPtr(soldier->pSoldierWeaponsComponent->GetActiveSoldierWeapon())) {
				return soldier->pSoldierWeaponsComponent->GetActiveSoldierWeapon();
			}
		}
	}

	return nullptr;
}

void Game::ExecuteMessage(void* pManager, Message* pMessage) {
	Invoke::Call<int>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwExecuteMessage), pManager, pMessage);
}

void Game::DispatchMessage(void* pManager, Message* pMessage) {
	Invoke::Call<int>(0x888EFDE8, pManager, pMessage);
}

void Game::MessageConstruct(Message* pMessage, int iCategory, int iType) {
	Invoke::Call<int>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageConstruct), pMessage, iCategory, iType);
}

void Game::ExecuteNetworkableMessage(void* pClientConnection, NetworkableMessage* message) {
	Invoke::Call<int>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwExecuteNetworkableMessage), pClientConnection, message);
}

char Game::GetDamageID() {
	DWORD r28 = *(DWORD*)(GetClientGameContextPointer() + 0x10);
	return (*(DWORD*)(r28)+(*(DWORD*)(r28 + 4) << 1)) & 0x3F;
}

void Game::SendClientDamageMessage(void* pHealthComp, char damageId, ClientDamageInfo* pDamageMessage, int empty, ClientPlayer* pTarget, int empty2, float damage) {
	Invoke::Call<int>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwSendClientDamageMessage), pHealthComp, damageId, pDamageMessage, empty, pTarget, empty2, WeaponMenuVars::vars.bAlwaysHeadshot ? 1 : 0, damage);
}

void Game::SendOrder(ClientPlayer* pWhoSentTheOrder, OrderType orderType) {
	auto soldier = GetMySoldierEntity();
	if (ValidPtr(soldier) && ValidPtr(pWhoSentTheOrder)) {
		auto comp = soldier->pClientSpottingComponent;
		if (ValidPtr(comp)) {
			Invoke::Call<int>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwSendOrder), comp, pWhoSentTheOrder, orderType);
		}
	}
}

void Game::MessageBoxAccept(const char* pTitle, const char* pDesc, UIPopupType PopupType) {
	bShowMessageBox = true;

	// patch the default popup type to ours.
	Natives::Write1Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageBox[0]), PopupType);

	// nop out something that i can't remember.
	Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageBox[1]), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop));

	struct LeaderboardComp {
		char _0x0000[0xD4];
		bool bIsShowing;
		char _0x00D5[0x20];
	};

	LeaderboardComp comp;
	comp.bIsShowing = false;

	// allocate the storage buffer for holding the original bytes of the string patch address.
	BYTE* szOriginalPatch = new BYTE[strlen(pTitle) + 1];

	// save the original bytes at the string patch address
	memcpy(szOriginalPatch, (void*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageBox[3]), strlen(pTitle) + 1);

	// set a presumed unused memory section to the string of our title.
	strcpy((char*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageBox[3]), pTitle);

	// patch out the default label "ID_M_LEADERBOARD_FAILED" load to point to the section we wrote the title to.
	// because our title isn't a registered label, it'll just use the value instead of trying to convert it thru the gxt table.
	Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageBox[4]), 0x3D60899B);
	Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageBox[5]), 0x388B6F50);

	// patch the size of the title. This will be fine, as long as the title string is below 255 chars.
	Natives::Write1Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageBox[6]), (BYTE)strlen(pTitle));

	Invoke::Call<int>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageBox[2]), &comp, pDesc);

	// patch back the default popup type.
	Natives::Write1Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageBox[0]), 6);

	// patch back the thing that i can't remember.
	Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageBox[1]), 0x914100AC);

	// copy back the memory we wrote the title to.
	memcpy((void*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageBox[3]), szOriginalPatch, strlen(pTitle) + 1);

	// patch back the original title load
	Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageBox[4]), 0x3D608804);
	Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageBox[5]), 0x388B0DC8);

	// patch back the size of the title gxt.
	Natives::Write1Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwMessageBox[6]), 0x17);

	// we work at ninja, we aint tryna embarrass over mem leakz. Fuck is wrong with you.
	delete[] szOriginalPatch;
}

bool Game::WorldToScreen(Vec3 world, Vec3& out) {
	auto renderer = GetGameRenderer();
	if (ValidPtr(renderer)) {
		auto viewProjectionMatrix = renderer->pRenderView->ViewProjectionMatrix;
		float w = viewProjectionMatrix(0, 3) * world.x +
			viewProjectionMatrix(1, 3) * world.y +
			viewProjectionMatrix(2, 3) * world.z +
			viewProjectionMatrix(3, 3);

		if (w < 0.0001f) {
			return false;
		}

		float x = viewProjectionMatrix(0, 0) * world.x +
			viewProjectionMatrix(1, 0) * world.y +
			viewProjectionMatrix(2, 0) * world.z +
			viewProjectionMatrix(3, 0);

		float y = viewProjectionMatrix(0, 1) * world.x +
			viewProjectionMatrix(1, 1) * world.y +
			viewProjectionMatrix(2, 1) * world.z +
			viewProjectionMatrix(3, 1);

		out.x = (1280.0f * 0.5f) + (1280.0f * 0.5f) * x / w;
		out.y = (720.0f * 0.5f) - (720.0f * 0.5f) * y / w;
		out.z = w;

		return true;
	}

	return false;
}

void HealTeamCallback(ClientPlayer* pEntity) {
	if (ValidPtr(pEntity)) {
		if (pPlayer.IsFriendly(pEntity)) {
			pPlayer.Heal(pEntity);
		}
	}
}

void Game::HealTeam() {
	pPlayer.PlayerCallback(true, HealTeamCallback);
}