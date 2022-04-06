#include "stdafx.h"
#include "menu/handler/submenus/weaponMenu.h"
#include "menu/handler/submenus/settingsMenu.h"
#include "menu/handler/submenus/settings/soundsMenu.h"
#include "menu/handler/submenus/main/mainChatCommands.h"
#include "menu/handler/submenus/main/developerMenu.h"
#include "menu/handler/submenus/main/movementMenu.h"
#include "menu/handler/submenus/main/loadoutEditorMenu.h"

typedef DWORD(*tDrawHudInventory)(DWORD, DWORD, DWORD);
tDrawHudInventory OriginalDrawHudInventory;
DWORD DrawHudInventoryHook(DWORD r3_, DWORD r4, DWORD r5) {
	/*
		lis       r9, a_hudinvzd@h # "_hudinvzd"
		lwz       r11, 0x58(r30)
		addi      r4, r9, a_hudinvzd@l # "_hudinvzd"
		addi      r3, r11, 0x2A8
		lwz       r5, 0x38(r10)
		bl        CreateText
	*/

	// as you can see from the game snippet above, it was pretty easy to figure out the manager ptr for rendering.
	// it's somewhat similar to bf3's text rendering, except the functions i used on bf3 seem to be called later on 
	// in the functions i use in bf4.
	pGFxDrawText.dwRendererManager = *(DWORD*)(r3_ + 0x58) + 0x2A8;
	pGFxDrawText.dwRendererBase = r3_;

	return OriginalDrawHudInventory(r3_, r4, r5);
}

typedef void(*tPrimaryFireCallback)(DWORD);
tPrimaryFireCallback OriginalPrimaryFireCallback;
void PrimaryFireCallbackHook(DWORD r3) {
	// this hook exists because it's an easy way to check that i have infact shot my gun. The hook i use for sending damage
	// is executed when anyone shoots, and so is this, but this one was easier to figure out who it was that shot.
	// I found this function because it sends a message: "ClientWeaponPlayerPrimaryWeaponFireMessage".

	if (r3) {
		DWORD r11 = *(DWORD*)(r3 - 0x54);
		if (r11) {
			DWORD res1 = *(DWORD*)(r11 + 0x150);
			if (res1) {
				if (res1 == (DWORD)pGame.GetLocalPlayer()) {
					pGame.bJustShot = true;

					if (AimbotMenuVars::vars.bUnfair || AimbotMenuVars::vars.iAimbotType == 2 || pGame.bIsUsingRailgun) {
						if (ValidPtr(pGame.pAimbotSoldier)) {
							if (ValidPtr(pGame.pAimbotSoldier->pClientPlayer)) {
								pPlayer.Kill(pGame.pAimbotSoldier->pClientPlayer);
							}
						}
					}

					if (WeaponMenuVars::vars.bTeamHealGun) {
						pGame.HealTeam();
					}

					if (WeaponMenuVars::vars.bSelfHealGun) {
						pPlayer.Heal(pGame.GetLocalPlayer());
					}
				}
			}
		}
	}

	OriginalPrimaryFireCallback(r3);
}

typedef void(*tGetDispersion)(WeaponSway*, Matrix*, bool);
tGetDispersion OriginalGetDispersion;
void GetDispersionHook(WeaponSway* pThis, Matrix* transform, bool scale) {
	// this hook isn't used anymore, but it was how i was doing visual no spread.
	OriginalGetDispersion(pThis, transform, scale);
}

typedef void(*tGetRecoil)(WeaponSway*, D3DXMATRIX&, bool);
tGetRecoil OriginalGetRecoil;
void GetRecoilHook(WeaponSway* pThis, D3DXMATRIX& transform, bool scale) {
	// this hook isn't used anymore, but it was how i was doing visual no recoil.

	if (WeaponMenuVars::vars.bNoRecoil) {
		pThis->CurrentRecoilDeviation = Deviation();
	}
	
	OriginalGetRecoil(pThis, transform, scale);
}

typedef void(*tAddDamageData)(void*, DamageData*);
tAddDamageData OriginalAddDamageData;
void AddDamageDataHook(void* pConnection, DamageData* pDamageData) {
	// this function is the last thing called in the "SendClientDamageMessage" function, it queues the damage data.
	// Damage data is populated on the stack in the function i just mentioned and passed to this, i used to hook
	// the function i mentioned, but i found it was easier to do it here instead.

	auto isBit = [](DWORD val) -> bool {
		return ((val & 0xFFF00000) >> 20) == 0x91C;
	};

	for (int i = 0; i < sizeof(DamageData); i++) {
		DWORD dw = *(DWORD*)((DWORD)pDamageData + i);
		if (isBit(dw)) {
			DbgPrint("Suspicious value detected in damage stream @ 0x%X -> val was 0x%X, nulling...", i, dw);
			*(DWORD*)((DWORD)pDamageData + i) = 0;
			i += 4;
		}
	}
	
	if (WeaponMenuVars::vars.bAlwaysHeadshot) {
		Natives::Write4Byte((DWORD)pDamageData + 0x9c, 0x1);
	}

	// spoofing weapon is essentially reading the aimbot targets weapon pointer and setting it to that.
	if (AimbotMenuVars::vars.bSpoofWeaponToTarget) {
		if (ValidPtr(pGame.pAimbotSoldier)) {
			if (ValidPtr(pGame.pAimbotSoldier->pClientPlayer)) {
				if (ValidPtr(pGame.pAimbotSoldier->pHealthComponent)) {
					if (pGame.pAimbotSoldier->pClientPlayer->IsAlive()) {
						if (ValidPtr(pGame.pAimbotSoldier)) {
							auto comp = pGame.pAimbotSoldier->pSoldierWeaponsComponent;
							if (ValidPtr(comp)) {
								auto wep = comp->GetActiveSoldierWeapon();
								if (ValidPtr(wep)) {
									pDamageData->pWeaponUnlockAsset = wep->pWeaponUnlockAsset;
								}
							}
						}
					}
				}
			}
		}
	}

	// and lastly, the highest priority of the spoofs, the loadout weapon index spoof. This spoofs it to something currently in your loadout,
	// such as a gadget or grenade.
	if (AimbotMenuVars::vars.iSpoofWeapon != 0) {
		auto soldier = pGame.GetMySoldierEntity();
		if (ValidPtr(soldier)) {
			auto comp = soldier->pSoldierWeaponsComponent;
			if (ValidPtr(comp)) {
				auto wep = comp->GetWeaponAtIndex(AimbotMenuVars::SpoofWeapons[AimbotMenuVars::vars.iSpoofWeapon].Result);
				if (ValidPtr(wep)) {
					pDamageData->pWeaponUnlockAsset = wep->pWeaponUnlockAsset;
				}
			}
		}
	}

	DbgPrint("Queueing %.2f of damage", pDamageData->fDamage);

	OriginalAddDamageData(pConnection, pDamageData);
}

Detour<void> OriginalPhysicsRunner;
void PhysicsRunnerHook(DWORD r3, DWORD r4, DWORD r5, DWORD r6, DWORD r7) {
	// this is a function that virtually calls one of the ray query functions. I found this originall on BF3, was an easy find on BF4.
	// Instead of reversing the structures to find the Havok physics pointer, i just reversed the function and found out how the thisptr
	// is resolved and used that.

	OriginalPhysicsRunner.CallOriginal(r3, r4, r5, r6, r7);

	if (AimbotMenuVars::vars.bAutoWall || AimbotMenuVars::vars.bVisibilityCheck) {
		if (pGame.bCanRun) {
			if (ValidPtr((DWORD*)r4)) {
				auto r29 = *(DWORD*)(r4 + 0x114);
				if (ValidPtr((DWORD*)r29)) {
					auto ptr = *(DWORD*)(r29 + 0x30);
					if (ValidPtr((DWORD*)ptr)) {
						auto ptr2 = *(DWORD*)(ptr + 0x3C);
						if (ValidPtr((DWORD*)ptr2)) {
							pGame.dwRaycastHandle = ptr2;

							static int timer = 0;
							if (GetTickCount() - timer > 100) {
								timer = GetTickCount();

								pAimbot.ProcessAutoWall();
								pAimbot.ProcessVisibility();
							}
						}
					}
				}
			}
		}
	}
}

typedef ClientPlayerScore*(*tGetPlayerScore)(ClientPlayerScoreManager*, ClientPlayer*);
tGetPlayerScore OriginalGetPlayerScore;
ClientPlayerScore* GetPlayerScoreHook(ClientPlayerScoreManager* pThis, ClientPlayer* pPlayer) {
	// this function gets a players score, i hooked this so getting the kill and death count wouldn't rely on me shooting.

	if (ValidPtr(pGame.GetLocalPlayer())) {
		auto manager = OriginalGetPlayerScore(pThis, pGame.GetLocalPlayer());
		if (ValidPtr(manager)) {
			static int cached_kills = 0;
			static int cached_deaths = 0;
			static int cached_spree = 0;
			static int cached_streak = 0;
			static int cached_spree_timeout = 0; // double kill, triple etc

			if (manager->iKills > cached_kills) {
				cached_spree_timeout = GetTickCount();
				cached_kills++; // = manager->iKills;
				cached_spree++;
				cached_streak++;

				if (SoundsMenuVars::vars.bKillingSpree) {
					pSounds.Queue(SOUND_KILLSPREE, cached_spree);
				}

				if (SoundsMenuVars::vars.bKillingStreak) {
					pSounds.Queue(SOUND_KILLSTREAK, cached_streak);
				}

				if (SoundsMenuVars::vars.iOnKill != 0) {
					pSounds.QueueBF(SoundsMenuVars::SoundsTest[SoundsMenuVars::vars.iOnKill].pName);
				}
			}

			if ((GetTickCount() - cached_spree_timeout) > 4500) {
				cached_spree = 0;
			}

			if (manager->iDeaths > cached_deaths) {
				cached_deaths = manager->iDeaths;
				cached_spree = 0;
				cached_streak = 0;
			}
		}
	}

	return OriginalGetPlayerScore(pThis, pPlayer);
}

typedef DWORD(*tDrawRect)(DWORD, DWORD, DWORD, DWORD, DWORD);
tDrawRect OriginalDrawRect;
DWORD DrawRectHook(DWORD r3, DWORD r4, DWORD r5, DWORD r6, DWORD r7) {
	// if i recall, exile gave me a hook he used on ps3 for rendering rectangles, and i found one a bit higher up that ran better.
	
	Controller::Update();
	
	if (!pGFxDrawText.dwRendererManager || !pGFxDrawText.dwRendererBase)
		return OriginalDrawRect(r3, r4, r5, r6, r7);

	static bool init = false;
	if (!init) {
		init = true;

		pRenderer.Init();
		pMenu.Init();
	}

	if (pGame.bCanRun) {
		pMenu.Render();
		pSubmenuHandler.ASyncUpdate();

		if (pMenu.bOpen) {
			pSubmenuHandler.Update();
		}

		pMenu.ASyncUpdate();
	}

	return OriginalDrawRect(r3, r4, r5, r6, r7);
}

typedef void(*tHandleTextGlow)(DWORD, UIElementFontStyle*, float);
tHandleTextGlow OriginalHandleTextGlow;
void HandleTextGlowHook(DWORD r3, UIElementFontStyle* r4, float f1) {
	// i made this hook because for some reason if i didn't apply a glow filter to text i'd crash, and when i did
	// i ended up randomly crashing.

	if (ValidPtr(r4)) {
		if (ValidPtr(r4->pFontDefinition)) {
			if (ValidPtr(r4->pFontDefinition->pGlow)) {
				UIElementTextFilterGlow* glowFilter = (UIElementTextFilterGlow*)(*(DWORD*)(r4->pFontDefinition->pGlow));
				if (ValidPtr(glowFilter)) {
					if (glowFilter->fGlowStrength != 13.37f) {
						OriginalHandleTextGlow(r3, r4, f1);
					}
				}
			}
		}
	}
}

typedef DWORD(*tXamInputGetState)(int r3, int r4, XINPUT_STATE* state);
tXamInputGetState OriginalXamInputGetState;
DWORD XamInputGetStateHook(int r3, int r4, XINPUT_STATE* state) {
	// typical hook for handling input.

	DWORD ret = OriginalXamInputGetState(r3, r4, state);

	pController.bCanFly = state->Gamepad.sThumbLY >= 23000;
	pController.FlyThumb = state->Gamepad.sThumbLY;

	if (pController.bCanFly) {
		if (MovementMenuVars::vars.bFlyHack) {
			state->Gamepad.sThumbLY = 0;
			state->Gamepad.sThumbLX = 0;
		}
	}

	if (pConfig.bShouldShowChangelog) {
		pConfig.bShouldShowChangelog = false;

		pGame.MessageBoxAccept("Welcome to Battlefield 4 with the NiNJA Engine!", pConfig.szChangelogData, PopupType_PrivacyPolicy);
	}

	if (pGame.bReload) {
		pGame.bReload = false;

		static int lastReload = 0;
		if (GetTickCount() - lastReload > 5000) {
			lastReload = GetTickCount();
			state->Gamepad.wButtons &= XINPUT_GAMEPAD_X;
		}
	}

	if (pGame.bPaused) {
		pGame.bPaused = false;
		return ret;
	}

	pController.bZoomProgress = state->Gamepad.bLeftTrigger;

	if (pGame.bCanRun) {
		if (AimbotMenuVars::vars.bAimRequried && !pController.IsDown(LEFT_TRIGGER)) goto jJump;

		if (AimbotMenuVars::vars.iAimbotType
			&& pPlayer.IsValidClientSoldier(pGame.pAimbotSoldier)
			&& pPlayer.IsValidClientPlayer(pGame.pAimbotSoldier->pClientPlayer)) {
			if (AimbotMenuVars::vars.bAutoAim) {
				state->Gamepad.bLeftTrigger = 255;
			}

			if (AimbotMenuVars::vars.bAutoShoot) {
				if (pGame.bIsUsingRailgun) {
					state->Gamepad.bRightTrigger = 255;
				} else {
					static int timer = 0;
					if (GetTickCount() - timer > AimbotMenuVars::vars.iAutoShootDelay) {
						timer = GetTickCount();
						state->Gamepad.bRightTrigger = 255;
					}
				}
			}
		}

	jJump:
		if (pMenu.IsOpen() && !IsOpenPressed()) {
			if (state->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_LEFT;
				pController.bLeftBypass = true;
			}

			if (state->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_RIGHT;
				pController.bRightBypass = true;
			}

			if (state->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) {
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_UP;
				pController.bUpBypass = true;
			}

			if (state->Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) {
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_DOWN;
				pController.bDownBypass = true;
			}

			if (state->Gamepad.wButtons & XINPUT_GAMEPAD_A) {
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_A;
				pController.bABypass = true;
			}

			if (state->Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_LEFT_SHOULDER;
				pController.bLBBypass = true;
			}

			if (state->Gamepad.wButtons & XINPUT_GAMEPAD_B) {
				state->Gamepad.wButtons &= ~XINPUT_GAMEPAD_B;
				pController.bBBypass = true;
			}
		}
	}

	return ret;
}

typedef void(*tSetText)(DWORD* pText, const char* _pText, int unk);
tSetText OriginalSetText;
void SetTextHook(DWORD* pText, const char* _pText, int unk) {
	// hook to check when the pause menu is active, so the menu can stop rendering.

	OriginalSetText(pText, _pText, unk);

	if (_pText) {
		if (strlen(_pText) >= 6) {
			if (_pText[0] == 0x52
				&& _pText[3] == 0x55) {
				pGame.bPaused = true;
			}
		}
	}
}

typedef DWORD(*tPlayEffect)(DWORD r3, PlayEffectAsset* r4, DWORD r5, DWORD r6, DWORD r7, DWORD r8, DWORD r9);
tPlayEffect OriginalPlayEffect;
DWORD PlayEffectHook(DWORD r3, PlayEffectAsset* r4, DWORD r5, DWORD r6, DWORD r7, DWORD r8, DWORD r9) {
	if (r4) {
		// DbgPrint("%s", r4->pName);
	}

	return OriginalPlayEffect(r3, r4, r5, r6, r7, r8, r9);
}

typedef DWORD(*tCreateKillMessage)(DWORD* r3, DWORD* r4);
tCreateKillMessage OriginalCreateKillMessage;
DWORD CreateKillMessageHook(DWORD* r3, DWORD* pMessagePtr) {
	// this function was easy to find due to the "ID_H_MSG_KILLS_KILLED" and "ID_H_MSG_KILLS_SUICIDED" gxt labels.
	// I use it to check the feed when i've killed someone, or when someone has killed me.

	char* messages[] = {
		"I just wrecked",
		"My dad works at Microsoft",
		"I just shat on",
		"Shoulda bought NiNJA",
		"How does it feel?",
		"Why you heff to be mad, is just game",
		"Sorry, I'm too OP",
		"Yo praise the lord for ronins shit"
	};

	char* messagesDeath[] = {
		"I just got yeeted by",
		"My NiNJAs don't die we just vossi bop",
		"Small penis alert:",
		"Eat shoes",
		"Lucky shot"
	};

	if (MainChatCommandsMenuVars::vars.bCustomKillFeedKill || MainChatCommandsMenuVars::vars.bCustomKillFeedDeath || SoundsMenuVars::vars.iOnDeath != 0) {
		if (ValidPtr(pMessagePtr)) {
			// thanks teir for helping with this part.
			auto killer = (ClientPlayer*)(Invoke::Call<DWORD>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwCreateKillMessageGetInfo), (DWORD)pMessagePtr + 0x40) - 0xE58);
			auto victim = (ClientPlayer*)(Invoke::Call<DWORD>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwCreateKillMessageGetInfo), (DWORD)pMessagePtr + 0x48) - 0xE58);

			if (MainChatCommandsMenuVars::vars.bCustomKillFeedKill) {
				if (ValidPtr(pGame.GetLocalPlayer()) && killer == pGame.GetLocalPlayer() && ValidPtr(victim)) {
					pMessages.SendTextMessage(CctSayAll, CctAdmin, pUtils.va("%s %s", messages[pGame.iKillFeedRand[0]], victim->szName));
				}
			}

			if (MainChatCommandsMenuVars::vars.bCustomKillFeedDeath) {
				if (ValidPtr(pGame.GetLocalPlayer()) && victim == pGame.GetLocalPlayer() && ValidPtr(killer)) {
					if (killer == pGame.GetLocalPlayer()) {
						pMessages.SendTextMessage(CctSayAll, CctAdmin, "NiNJAs don't die we just vossi bop");
					} else {
						pMessages.SendTextMessage(CctSayAll, CctAdmin, pUtils.va("%s %s", messagesDeath[pGame.iKillFeedRand[1]], killer->szName));
					}
				}
			}

			if (SoundsMenuVars::vars.iOnDeath != 0) {
				if (ValidPtr(pGame.GetLocalPlayer()) && victim == pGame.GetLocalPlayer() && ValidPtr(killer)) {
					pSounds.QueueBF(SoundsMenuVars::SoundsTest[SoundsMenuVars::vars.iOnDeath].pName);
				}
			}
		}
	}

	if (MainMenuVars::vars.bDisableKillFeed) {
		return 0;
	}

	return OriginalCreateKillMessage(r3, pMessagePtr);
}

typedef DWORD(*tUpdatePosition)(ClientConnection*, EntryInputStateNetworkMove*);
tUpdatePosition OriginalClientConnectionAddMove;
DWORD ClientConnectionAddMoveHook(ClientConnection* pConnection, EntryInputStateNetworkMove* pState) {
	// noticed a class mentioned here: https://www.unknowncheats.me/forum/battlefield-4-a/158597-super-jump.html
	// looked into it in the BF3 PC PDB, found it used with a function called "AddMove", seemed promising.

	if (MovementMenuVars::vars.bFlyHack) {
		// this bit flag updated when i tried to vault, noticed it was essentially adding 2, so instead of figuring
		// out some gay bit shit i just add 2, works fine so w/e. If i don't set this, i just sit there.

		if (pController.bCanFly) {
			pState->State.dwCustomBitFlags += 2;

			float speed = 5.f + ((MovementMenuVars::vars.fFlyHackSpeed - 5.f) / (0x8000 - 23000)) * (pController.FlyThumb - 23000);

			if (IsDown(LEFT_THUMB))
				speed *= 1.5f;

			Vec3 shit = pGame.GetGameRenderer()->pRenderView->Transform.forward * speed;
			shit.Negate();

			pState->State.VaultVelocity += shit;
		} else {
			if (ValidPtr(pGame.GetMySoldierEntity())) {
				if (ValidPtr(pGame.GetMySoldierEntity()->pClientParachuteComponent)) {
					if (pGame.GetMySoldierEntity()->pClientParachuteComponent->IsInAir()) {
						pState->State.dwCustomBitFlags += 2;
						pState->State.VaultVelocity = Vec3();
					}
				} else {
					if (ValidPtr(pGame.GetMySoldierEntity()->GetParachuteCompOther())) {
						if (pGame.GetMySoldierEntity()->GetParachuteCompOther()->IsInAir()) {
							pState->State.dwCustomBitFlags += 2;
							pState->State.VaultVelocity = Vec3();
						}
					}
				}
			}
		}
	}

	if (pState && AimbotMenuVars::vars.iAimbotType == 2 || pGame.bIsUsingRailgun) {
		if (pPlayer.IsValidClientSoldier(pGame.pAimbotSoldier)) {
			if (pPlayer.IsValidClientSoldier(pGame.GetMySoldierEntity())) {
				pState->State.AuthoritiveViewAngles = pAimbot.vSavedAngles;

				pAimbot.MovementFix(pState);

				auto wep = pGame.GetSoldierWeapon();
				if (ValidPtr(wep)) {
					auto swayHead = wep->pWeaponSwayHead;
					if (ValidPtr(swayHead)) {
						auto sway = swayHead->pWeaponSway;
						if (ValidPtr(sway)) {
							if ((DWORD)&sway->CurrentDispersionDeviation
								&& (DWORD)&sway->CurrentRecoilDeviation) {
								// figuring out this was somewhat easy, i first logged the recoil values and noticed that they were similar
								// to that of the angle ranges, so i just added it on and poof it worked. Not so sure about the spread,
								// i didn't actually test that but i just did the same calcs (adding it on), and i guess it works? :shrug:

								pState->State.AuthoritiveViewAngles += Vec2(sway->CurrentDispersionDeviation.fYaw, sway->CurrentDispersionDeviation.fPitch);
								pState->State.AuthoritiveViewAngles += Vec2(asinf(sway->CurrentRecoilDeviation.fYaw), asinf(sway->CurrentRecoilDeviation.fPitch));
							}
						}
					}
				}
			}
		}
	}

	return OriginalClientConnectionAddMove(pConnection, pState);
}

typedef void(*tClientConnectionSendMessage)(void*, NetworkableMessage*);
tClientConnectionSendMessage OriginalClientConnectionSendMessage;
void ClientConnectionSendMessageHook(void* pThis, NetworkableMessage* pNetworkableMessage) {
	OriginalClientConnectionSendMessage(pThis, pNetworkableMessage);

	DbgPrint("Sending networkable message %s", Invoke::Call<TypeInfo*>(*(DWORD*)pNetworkableMessage->pVtable, pNetworkableMessage)->pInfoData->pName);
}

typedef bool(*tFormatString)(char*, const char*, ...);
tFormatString OriginalFormatString;
bool FormatStringHook(char* pBuffer, const char* pGxt, ...) {
	va_list ap;
	va_start(ap, pGxt);
	vsprintf(pGame.szFormatStringBuffer, pGxt, ap);
	va_end(ap);

	if (strstr(pGame.szFormatStringBuffer, "Play multiplayer matches online with other players.")) {
		return OriginalFormatString(pBuffer, "Play multiplayer matches online with NiNJA engine.");
	}

	return OriginalFormatString(pBuffer, pGame.szFormatStringBuffer);
}

typedef DWORD(*tOnShoot)(DWORD r3, DWORD r4, DWORD r5);
tOnShoot OriginalOnShoot;
DWORD OnShootHook(DWORD r3, DWORD r4, DWORD r5) {
	// to get this, i put a breakpoint on the server settings byte for if hitmarkers were disabled in hopes to find where it renders
	// the hitmarker, instead i found a function that was called when people shoot. The reason i chose to use this instead of the hook
	// above was because the aimbot was being trash with that hook, not sure why. I use the other hook still, so i can tell if i have shot,
	// because i couldn't be bothered to reverse this function much.

	if (pGame.bJustShot) {
		pGame.bJustShot = false;

		if (AimbotMenuVars::vars.bUnfair || AimbotMenuVars::vars.iAimbotType == 2) {
			if (ValidPtr(pGame.pAimbotSoldier)) {
				if (ValidPtr(pGame.pAimbotSoldier->pClientPlayer)) {
					pPlayer.Kill(pGame.pAimbotSoldier->pClientPlayer);
				}
			}
		}

		if (WeaponMenuVars::vars.bTeamHealGun) {
			pGame.HealTeam();
		}

		if (WeaponMenuVars::vars.bSelfHealGun) {
			pPlayer.Heal(pGame.GetLocalPlayer());
		}
	}

	return OriginalOnShoot(r3, r4, r5);
}

void DispatchMessageHook(void* pConnection, Message* pMessage) {
	// this was such a ball-ache to hook due to some gay tls shit they do, luckily i was able to use
	// the class i made for branch hooking on BF3 that teir helped me with, got some juicy logs from this.

	auto name = Invoke::Call<TypeInfo*>(*(DWORD*)pMessage->pVtable, pMessage)->pInfoData->pName;

	char* blacklistPrint[] = {
		"ServerComponentEntryComponentProcessedInputMessage",
		"ClientInputUnchangedInputMessage",
		"ServerPlayerTickMessage",
		"UINPXTooltipMessage",
		"NetworkLogicFirePlayerEventMessageBase",
		"ClientMetricsPushUIScreenMessage",
		"ClientMetricsPopUIScreenMessage",
		"ClientStartedMessage",
		"ServerStoppedMessage",
		"StreamInstallRequestSuspendMessage",
		"StreamInstallRequestResumeMessage",
		"BundleManagerBundleUnloadMessage"
	};

	Invoke::Call<int>(0x888EFDE8, pConnection, pMessage);
}

typedef void(*tDamageStreamCrash)(DWORD* pThis, DWORD _r4, DWORD _r5);
tDamageStreamCrash OriginalDamageStreamCrash;
void DamageStreamCrashHook(DWORD* pThis, DWORD _r4, DWORD _r5) {
	if (_r5 != 0) {
		DWORD r30 = *(DWORD*)(_r5);
		if (_r4 != 0) {
			if (_r4 != 1) {
				if (r30 != _r5) {
					bool ret = false;

					do {
						if (ValidPtr((DWORD*)r30)) {
							DWORD r4 = *(DWORD*)(r30 + 8);
							if (ValidPtr((DWORD*)r4)) {
								r30 = *(DWORD*)r30;
							} else {
								ret = true;
								break;
							}
						} else {
							ret = true;
							break;
						}
					} while (r30 != _r5);

					if (ret) {
						DbgPrint("Prevented damage stream crash, ya welcome");
						return;
					}
				}
			}
		}
	}

	OriginalDamageStreamCrash(pThis, _r4, _r5);
}

typedef DWORD(*tAimbot)(DWORD, DWORD);
tAimbot OriginalAimbot;
DWORD AimbotHook(DWORD r3, DWORD r4) {
	DWORD resp = OriginalAimbot(r3, r4);

	if (pGame.bCanRun) {
		auto player = pGame.GetLocalPlayer();
		if (ValidPtr(player)) {
			auto soldier = player->GetClientSoldier();
			if (ValidPtr(soldier)) {
				auto comp = soldier->pSoldierWeaponsComponent;
				if (ValidPtr(comp)) {
					auto wep = comp->GetActiveSoldierWeapon();
					if (ValidPtr(wep)) {
						auto firing = wep->pWeaponFiring;
						if (ValidPtr(firing)) {
							auto currentAmmo = firing->nAmmoInClip;
							auto ammoInPile = firing->nAmmoInReserve;

							if (WeaponMenuVars::vars.bAutoReload && !WeaponMenuVars::vars.bUnlimitedAmmo) {
								if (ammoInPile > 0) {
									if (currentAmmo == 0) {
										pGame.bReload = true;
									}
								}
							}

							if (WeaponMenuVars::vars.bUnlimitedAmmo) {
								if (currentAmmo == 0) {
									static int timer = 0; // prevent spamming of the networkable message while ammo count is 0
									if (GetTickCount() - timer > 2000) {
										timer = GetTickCount();

										// this was pretty stressful to figure out. I Originally constructed and executed the "NetworkPlayerSelectedWeaponMessage" message,
										// but the annoying this with that is it had to go through all of the items in your loadout otherwise once called it'l just do it for your current weapon, and you'd lose everything
										// else in your loadout. So, i just reversed the function that did the full message (it iterates through the loadout), and called it below. Good hax.

										DWORD r3 = Invoke::Call<DWORD>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwUnlimitedAmmo[0]), *(DWORD*)(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwUnlimitedAmmo[1])) + 0x150, "SelectedKit", 0);
										DWORD r4 = Invoke::Call<DWORD>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwUnlimitedAmmo[2])); // might be active slot?
										bool bIsSpawning = false;

										Invoke::Call<DWORD>(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwUnlimitedAmmo[3]), r3, r4, bIsSpawning);
									}
								}
							}
						}
					}
				}
			}
		}

		if (AimbotMenuVars::vars.iAimbotType != 0) {
			if (AimbotMenuVars::vars.bAimRequried && !pController.IsDown(LEFT_TRIGGER)) return resp;

			pGame.pAimbotSoldier = pAimbot.GetTarget(pGame.AimbotTargetInfo);

			if (ValidPtr(pGame.pAimbotSoldier) && ValidPtr(pGame.pAimbotSoldier->pClientPlayer)) {
				auto weapon = pGame.GetSoldierWeapon();
				if (ValidPtr(weapon)) {
					if (ValidPtr(weapon->pAimingSimulation)) {
						if (ValidPtr(weapon->pAimingSimulation->pAimAssist)) {
							if (ValidPtr(pGame.GetMySoldierEntity())) {
								if (ValidPtr(pGame.GetMySoldierEntity()->pPredictedController)) {
									auto mySoldier = pGame.GetMySoldierEntity();
									if (!ValidPtr(mySoldier)) return resp;

									if (!ValidPtr(pWeapon.GetBulletEntityData())) {
										DbgPrint("Can't execute aimbot logic, bulletentitydata isn't valid.");
										return resp;
									}

									if (!ValidPtr(pWeapon.GetFiringFunctionData())) {
										DbgPrint("Can't execute aimbot logic, firingfunctiondata isn't valid.");
										return resp;
									}

									pAimbot.vSavedAngles = pGame.BaseAimCorrection();

									switch (AimbotMenuVars::vars.iAimbotType) {
									case 1: // normal (snap)
										if (ValidPtr(weapon) && ValidPtr(weapon->pAimingSimulation)) {
											if (ValidPtr(weapon->pAimingSimulation->pAimAssist)) {
												weapon->pAimingSimulation->pAimAssist->Angles = pAimbot.vSavedAngles;

												auto wep = pGame.GetSoldierWeapon();
												if (ValidPtr(wep)) {
													auto swayHead = wep->pWeaponSwayHead;
													if (ValidPtr(swayHead)) {
														auto sway = swayHead->pWeaponSway;
														if (ValidPtr(sway)) {
															weapon->pAimingSimulation->pAimAssist->Angles += Vec2(asinf(sway->CurrentRecoilDeviation.fYaw), asinf(sway->CurrentRecoilDeviation.fPitch));
														}
													}
												}
											}
										}
										break;
									case 3: // humanized
										if (ValidPtr(weapon) && ValidPtr(weapon->pAimingSimulation)) {
											if (ValidPtr(weapon->pAimingSimulation->pAimAssist)) {
												Vec2 angles = pAimbot.SmoothAimbotAngles(weapon->pAimingSimulation->pAimAssist->Angles, pAimbot.vSavedAngles);

												weapon->pAimingSimulation->pAimAssist->Angles = angles;

												auto wep = pGame.GetSoldierWeapon();
												if (ValidPtr(wep)) {
													auto swayHead = wep->pWeaponSwayHead;
													if (ValidPtr(swayHead)) {
														auto sway = swayHead->pWeaponSway;
														if (ValidPtr(sway)) {
															weapon->pAimingSimulation->pAimAssist->Angles += Vec2(asinf(sway->CurrentRecoilDeviation.fYaw), asinf(sway->CurrentRecoilDeviation.fPitch));
														}
													}
												}

												pGame.vOldAngles = weapon->pAimingSimulation->pAimAssist->Angles;
											}
										}
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return resp;
}

void UnknownCrashHook(DWORD r3, DWORD r4) {
	if (ValidPtr((DWORD*)r4)) {
		DWORD r11 = *(DWORD*)(r4 + 4);
		DWORD r10 = *(DWORD*)(r4);
		DWORD r9 = r11 & 0xF;
		r10 = r10 & 0xF;
		DWORD r8 = r9 << 4;
		r11 = r8 | r10;
		r11--;
		DWORD r7 = r11;

		if (r11 > 0x1F) {
			r7 = 0x1F;
		}

		r11 = r7 + 1;
		r8 = r11 << 2;
		r11 = *(DWORD*)(r8 + r3);

		if (r11 == 0) {
			r11 = r10 | r4;
			r10 = r9 | r4;
			*(DWORD*)(r4) = r11;
			*(DWORD*)(r4 + 4) = r10;
		} else {
			if (ValidPtr((DWORD*)r11)) {
				r10 = r10 | r11;
				*(DWORD*)r4 = r10;
				DWORD r6 = *(DWORD*)(r11 + 4);
				DWORD r5 = r6 & 0xFFFFFFF0;
				r10 = r5 | r9;
				*(DWORD*)(r4 + 4) = r10;
				r9 = *(DWORD*)(r11 + 4);
				r6 = r9 & 0xFFFFFFF0;
				r5 = *(DWORD*)r6;
				r10 = r5 & 0xF;
				r9 = r10 | r4;
				*(DWORD*)(r6) = r9;
				r6 = *(DWORD*)(r11 + 4);
				r5 = r6 & 0xF;
				r10 = r5 | r4;
				*(DWORD*)(r11 + 4) = r10;
			} else {
				DbgPrint("Prevented unk crash");
			}
		}

		*(DWORD*)(r8 + r3) = r4;
		r11 = 1;
		r10 = r11 << r7;
		r9 = *(DWORD*)r3;
		r8 = r10 | r9;
		*(DWORD*)r3 = r8;
	}
}

typedef void(*tUnknown2Crash)(DWORD, DWORD);
tUnknown2Crash OriginalUnknown2Crash;
void Unknown2CrashHook(DWORD r3, DWORD r4) {
	if (ValidPtr((DWORD*)r4)) {
		auto r6 = *(DWORD*)(r4 - 4) + (r4 - 8);
		if (ValidPtr((DWORD*)r6)) {
			OriginalUnknown2Crash(r3, r4);
		} else {
			DbgPrint("Prevented crash");
		}
	} else {
		DbgPrint("Prevented crash");
	}
}

typedef DWORD(*tUnknown3Crash)(DWORD r3, DWORD r4, DWORD r5, DWORD r6);
tUnknown3Crash OriginalUnknown3;
DWORD Unknown3CrashHook(DWORD r3, DWORD r4, DWORD r5, DWORD r6) {
	DWORD r11 = r4 + r5;
	DWORD r10 = r5 - 1;
	DWORD r9 = r11 - 1;
	DWORD r29 = r3;
	DWORD r31 = r9 & ~r10;
	DWORD r26 = r6;

	if (r31 <= 0x100) {
		DWORD r27 = 0x898D3FA0;
		r10 = *(DWORD*)(r29 + 0xA60);
		r11 = r10 << 2;
		DWORD r30 = (DWORD)TlsGetValue(*(DWORD*)(r11 + r27));
		
		if (r30 == 0) {
			r30 = Invoke::Call<DWORD>(0x889026A0, 0x118, 4);
			if (r30 == 0) return 0;
		}

		r11 = r31 - 1;
		r9 = r31 - 0x1D;
		r10 = r11 >> 4;
		DWORD r8 = r11 >> 2;
		r11 = r10 + 6;
		int r7 = (int)((int)r9 >> 0x1f); // srawi     r7, r9, 0x1F
		DWORD _r6 = r11 - r8;
		r10 = _r6 & r7;
		r31 = r10 + r11;
		r11 = r31 << 1;
		DWORD _r5 = r31 + r11;
		r11 = _r5 << 2;
		r11 = r11 + r30;
		r30 = r11 + 8;
		DWORD _r3 = *(DWORD*)(r11 + 8);
		
		DWORD resp = 0;
		if (_r3 == 0) {
			resp = Invoke::Call<DWORD>(0x888ED4E0, r3, r31, r30 + 8, r30 + 4);
			*(DWORD*)r30 = resp;
		} else resp = _r3;

		if (!ValidPtr((DWORD*)resp)) {
			DbgPrint("Prevented crash");
			return 0; // the fix...
		}

		return OriginalUnknown3(r3, r4, r5, r6);
	}

	return 0;
}

void Unknown4CrashHook(DWORD r3, DWORD r4, DWORD r5) {
	if (ValidPtr((DWORD*)r3)) {
		DWORD r30 = *(DWORD*)(r3 + 0x10);
		DWORD r29 = r4;
		DWORD r31 = *(DWORD*)(r3 + 0xC);
		DWORD r28 = r5;
		
		if (r31 == r30) return;

		do {
			DWORD _r3 = *(DWORD*)r31;
			DWORD _r5 = r28;
			DWORD _r4 = r29;

			if (ValidPtr((DWORD*)_r3)) {
				DWORD r11 = *(DWORD*)(_r3);
				DWORD r10 = *(DWORD*)(r11 + 0x8);
				Invoke::Call<int>(r10, _r3, _r4, _r5);
				r31 = r31 + 4;
			} else {
				DbgPrint("Prevented crash");
				break;
			}
		} while (r31 != r30);
	}
}

DWORD CrashHook(DWORD r3) {
	// idk, i was crashing in a function a lot so i just rewrote the function and added my own additions to prevent the crash.

	if (!ValidPtr((DWORD*)r3)) {
		//DbgPrint("Not valid first 0x%X", r3);
		return 0;
	}

	DWORD r11 = *(DWORD*)(r3 + 8);
	DWORD r9 = *(DWORD*)(r3 + 0xC);
	DWORD r10 = r11 + 8;
	*(DWORD*)(r3 + 8) = r10;

	if (r10 >= r9) {
		/*r10 = 0;
		*(DWORD*)0 = r10;*/
		//DbgPrint("woulda broken");
	}

	r10 = *(DWORD*)r3;
	r11 = r10 + r11;

	if (!ValidPtr((DWORD*)r11)) {
		//DbgPrint("Not valid second 0x%X", r11);
		return 0;
	}

	DWORD _r3 = *(DWORD*)r11;

	if (_r3 == 0) {
		return 0;
	}

	if (!ValidPtr((DWORD*)_r3)) {
		//DbgPrint("Not valid 0x%X", _r3);
		return 0;
	}

	r10 = *(DWORD*)_r3;
	r9 = r10 & 0xFFFFFFFC;
	DWORD r8 = r10 | 3;
	DWORD r7 = ~r10;
	*(DWORD*)(_r3) = r8;
	float f0 = *(float*)(r11 + 4);
	DWORD r4 = r7 & 1;
	*(float*)(r9) = f0;

	return Invoke::Call<DWORD>(0x888DD068, _r3, r4);
}

__declspec(naked) DWORD MultiplayerBadConnectionFixHook() {
	__asm {
		cmpwi cr6, r4, 0
		bne cr6, good
		li r3, 1
		blr
		good :
		lwz r11, 0x44(r4)
			cmpwi cr6, r11, 3
			bne cr6, hmm
			li r3, 1
			blr
			hmm :
		addi r11, r11, 1
			addic r10, r11, -1
			subfe r3, r9, r9
			blr
	}
}

typedef DWORD(*tBlazeUserManagerUserConstruct)(BlazeUserManagerUser* pThis, __int64 blazeId, const char *userName, unsigned __int64 externalId, const void *externalBlob, __int64 accountId, unsigned int accountLocale, DWORD *statusFlags);
tBlazeUserManagerUserConstruct OriginalBlazeUserManagerUserConstruct;
DWORD BlazeUserManagerUserConstructHook(BlazeUserManagerUser* pThis, __int64 blazeId, const char *userName, unsigned __int64 externalId, const void *externalBlob, __int64 accountId, unsigned int accountLocale, DWORD *statusFlags) {
	DWORD ret = OriginalBlazeUserManagerUserConstruct(pThis, blazeId, userName, externalId, externalBlob, accountId, accountLocale, statusFlags);

	if (pThis) {
		DbgPrint("Constructing %s's Blaze user...", userName);

		strcpy(pThis->szName, "Herb Roller");
		strcpy(pThis->szBlazePersonalId, "236434502");
		pThis->AccountID = 0x8a22e00d;
		pThis->ExternalId = 0x45d2e54;
	}

	return ret;
}

typedef bool(*tDeploy)(DWORD r3, DWORD r4, DWORD r5, DWORD r6, DWORD r7, DWORD r8);
tDeploy OriginalDeployVisuals;
bool DeployVisualsHook(DWORD r3, DWORD r4, DWORD r5, DWORD r6, DWORD r7, DWORD r8) {
	 *(bool*)(r3 + 0x18C) = true; // force deploy
	 *(float*)(r3 + 0x180) = 0.f;

	if (ValidPtr(pGame.GetLocalPlayer())) {
		auto fam = pGame.GetLocalPlayer()->pOwnPlayerView;
		if (fam) {
			*(float*)((DWORD)fam + 0x118) = 0.f; // m_warmUpTimer
			*(float*)((DWORD)fam + 0x11C) = 0.f; // m_lastRespawnTime
		}
	}

	return OriginalDeployVisuals(r3, r4, r5, r6, r7, r8);
}

DWORD GetOptionHook(DWORD r3, const char* pStr, DWORD def) {
	auto val = *(DWORD*)(r3 + 0x420);
	if (!val) { return def; }

	auto ret = Invoke::Call<DWORD>(0x88B48F50, val, pStr, def);

	if (ret) {
		if (!strcmp(pStr, "IAmReadyProperty")
			|| !strcmp(pStr, "SpawnInVehicleUIEnabled")
			|| !strcmp(pStr, "DeployEnabled")
			|| !strcmp(pStr, "ConsoleButtonBarDeployVisible")) {
			*(bool*)ret = true;
		}
	}

	return ret;
}

bool TitleHooks::bInitialized = false;
bool TitleHooks::Apply() {
	if (!bInitialized) {
		// hook functions

		/*if (!pHooking.HookFunction(0x888CABF0, &FormatStringHook, &OriginalFormatString)) {
			DbgPrint("Failed to hook FormatString!");
			return false;
		}

		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwOnShoot), &OnShootHook, &OriginalOnShoot, "OnShoot")) {
			DbgPrint("Failed to hook OnShoot!");
			return false;
		}

		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwGetDispersion), &GetDispersionHook, &OriginalGetDispersion)) {
			DbgPrint("Failed to hook GetDispersion!");
			return false;
		}

		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwGetRecoil), &GetRecoilHook, &OriginalGetRecoil)) {
			DbgPrint("Failed to hook GetRecoil!");
			return false;
		}
		
		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwClientConnectionSendMessage), &ClientConnectionSendMessageHook, &OriginalClientConnectionSendMessage)) {
			DbgPrint("Failed to hook ClientConnectionSendMessage!");
			return false;
		}

		pBranchHooking.Branch(0x888F0154, (DWORD)DispatchMessageHook);
		
		if (!pHooking.HookFunction(0x88F8F730, &Unknown2CrashHook, &OriginalUnknown2Crash)) {
			DbgPrint("Failed to hook Unknown2Crash!");
			return false;
		}

		if (!pHooking.HookFunction(0x8881F000, &BlazeUserManagerUserConstructHook, &OriginalBlazeUserManagerUserConstruct)) {
			DbgPrint("Failed to hook BlazeUserManagerUserConstruct!");
			return false;
		}

		if (!pHooking.HookFunction(0x888EDB98, &Unknown3CrashHook, &OriginalUnknown3)) {
			DbgPrint("Failed to hook Unknown3Crash!");
			return false;
		}

		if (!pHooking.HookFunction(0x88D298E8, &DamageStreamCrashHook, &OriginalDamageStreamCrash, "DamageStreamCrash")) {
			DbgPrint("Failed to hook DamageStreamCrash!");
			return false;
		}

		if (!pHooking.HookFunctionNoLink(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwCrashHookFix), &CrashHook)) {
			DbgPrint("Failed to hook Crash!");
			return false;
		}

		if (!pHooking.HookFunction(0x8875DF78, &DeployVisualsHook, &OriginalDeployVisuals, "DeployVisuals")) {
			DbgPrint("Failed to hook DeployVisuals!");
			return false;
		}
		
		pHooking.HookFunctionNoLink(0x8907C568, &UnknownCrashHook);
		pHooking.HookFunctionNoLink(0x882F1420, &UnhandledExceptionFilterHook);
		pHooking.HookFunctionNoLink(0x88D10620, &Unknown4CrashHook);
		pHooking.HookFunctionNoLink(0x88AF5D78, &GetOptionHook);*/

		pHooking.HookFunctionNoLink(0x889410E8, &MultiplayerBadConnectionFixHook);
		pHooking.HookFunctionNoLink(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwUnknownCrash4Hook), &Unknown4CrashHook);

		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwAimbotHook), &AimbotHook, &OriginalAimbot, "Aimbot")) {
			DbgPrint("Failed to hook Aimbot!");
			return false;
		}

		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwUpdatePosition), &ClientConnectionAddMoveHook, &OriginalClientConnectionAddMove, "ClientConnection::addMove")) {
			DbgPrint("Failed to hook ClientConnectionAddMove!");
			return false;
		}

		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwHandleTextGlow), &HandleTextGlowHook, &OriginalHandleTextGlow, "HandleTextGlow")) {
			DbgPrint("Failed to hook HandleTextGlow!");
			return false;
		}

		OriginalPhysicsRunner.SetupDetour(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwPhysicsRunner), &PhysicsRunnerHook);

		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwCreateKillMessage), &CreateKillMessageHook, &OriginalCreateKillMessage, "CreateKillMessage")) {
			DbgPrint("Failed to hook CreateKillMessage!");
			return false;
		}

		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwSetText), &SetTextHook, &OriginalSetText, "SetText")) {
			DbgPrint("Failed to hook SetText!");
			return false;
		}

		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwXamInputGetState), &XamInputGetStateHook, &OriginalXamInputGetState, "XamInputGetState")) {
			DbgPrint("Failed to hook XamInputGetState!");
			return false;
		}

		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwAddDamageData), &AddDamageDataHook, &OriginalAddDamageData, "AddDamageData")) {
			DbgPrint("Failed to hook AddDamageData!");
			return false;
		}

		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwPrimaryFireCallbackExecuteMessage), &PrimaryFireCallbackHook, &OriginalPrimaryFireCallback, "PrimaryFireCallback")) {
			DbgPrint("Failed to hook GetPlayerScore!");
			return false;
		}

		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwGetPlayerScore), &GetPlayerScoreHook, &OriginalGetPlayerScore, "GetPlayerScore")) {
			DbgPrint("Failed to hook GetPlayerScore!");
			return false;
		}

		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwDrawHudInventory), &DrawHudInventoryHook, &OriginalDrawHudInventory, "DrawHudInventory")) {
			DbgPrint("Failed to hook DrawHudInventory!");
			return false;
		}

		if (!pHooking.HookFunction(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwDrawRectHk), &DrawRectHook, &OriginalDrawRect, "DrawRect")) {
			DbgPrint("Failed to hook DrawRect!");
			return false;
		}

		Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwSendOrderPatch), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwBLR));
		Natives::Write4Byte(decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwPatch), decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop));

		// text anti spam bypass (local side), keep to a min of about 1.5s intervals otherwise after about 15
		// messages it's gonna not allow you to send any more for like 20 seconds
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTextAntiSpam1) = 0x38600001;
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwTextAntiSpam2) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwBLR);

		// render patches
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwRenderPatches[0]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwRenderPatches[1]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwRenderPatches[2]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);

		// job crash patches
		// *(DWORD*)0x88EFC730 = 0x60000000;
		// *(DWORD*)0x88EFC734 = 0x60000000;

		// stw patches
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[0]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[1]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[2]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[3]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[4]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[5]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[6]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[7]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[8]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[9]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[10]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[11]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[12]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[13]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[14]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)decryptDWORD(Not(NiNJA.pPatches)->BF4.dwStwCrashPatches[15]) = decryptDWORD(Not(NiNJA.pPatches)->BF4.dwNop);
		*(DWORD*)0x88902480 = 0x4e800020; // reaper

		*(DWORD*)0x886476C8 = 0x38600001;
		*(DWORD*)0x88632B84 = 0x38600001;
		*(DWORD*)0x88684AC8 = 0x38600001;
		*(DWORD*)0x88684B4C = 0x38600001;
		*(DWORD*)0x8868BE20 = 0x38600001;

		Sleep(1000);

		DbgPrint("Applied detours!");
		bInitialized = true;
		return true;
	} else {
		DbgPrint("TitleHooks already initialized!");
	}

	return false;
}

void TitleHooks::Remove(bool bOverride) {
	if (bInitialized || bOverride) {
		pHooking.UnhookAll();
		pBranchHooking.UnhookAll();
		OriginalPhysicsRunner.TakeDownDetour();

		Sleep(1000);

		DbgPrint("Removed detours!");
		bInitialized = false;
	} else {
		DbgPrint("TitleHooks have not been initialized!");
	}
}