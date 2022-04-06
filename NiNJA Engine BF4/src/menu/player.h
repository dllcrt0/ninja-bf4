#pragma once

class Player {
public:
	void Heal(ClientPlayer* pPlayer);
	void Kill(ClientPlayer* pPlayer);
	void Godmode(ClientPlayer* pShooter);
	bool IsFriendly(ClientPlayer* pPlayer);
	bool GetBonePosition(ClientSoldierEntity* pEntity, BONES bone, Vec3& out);
	void BoneToBone(ClientSoldierEntity* pEntity, BONES from, BONES to, GColor color);
	void PlayerCallback(bool excludeMe, void(*func)(ClientPlayer*), bool bypassHealth = false, bool onlyClientPlayer = false);
	ClientPlayer* GetClientPlayerFromID(int iID);

	bool IsValidClientPlayer(ClientPlayer* pPlayer);
	bool IsValidClientSoldier(ClientSoldierEntity* pEntity);
};

extern Player pPlayer;