#include "stdafx.h"
#include "mainChatCommands.h"
#include "../mainMenu.h"

using namespace MainChatCommandsMenuVars;

namespace MainChatCommandsMenuVars {
	Vars vars;
	
	ScrollStruct<ChatChannelType> ChannelType[] = {
		"All", CctSayAll,
		"Team", CctTeam,
		"Squad", CctSquad,
		"Squad Leader", CctSquadLeader,
		"Admin", CctAdmin,
	};

	void SendMessageKeyboardCallback(const char* pMsg) {
		pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, pMsg);
	}

	void SendPreset() {
		switch (pMenu.iCurrentOption - 5) {
		case 1:
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "I'm using NiNJA Engine!");
			break;
		case 2:
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "Buy NiNJA at xbl.ninja!");
			break;
		case 3:
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "<3 NiNJA <3");
			break;
		case 4:
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "GG EZ");
			break;
		case 5:
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "GLHF");
			break;
		case 6:
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "Woah, who's cheating? smh");
			break;
		case 7:
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "What the fuck did you just fucking say about me, you little bitch?");
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "I'll have you know I graduated top of my class in the Navy Seals.");
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "and I've been involved in numerous secret raids on Al-Quaeda, and I have over 300 confirmed kills.");
			break;
		case 8:
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "He's a semi-aquatic, egg-laying mammal of action!");
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "He's a furry little flatfoot,");
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "Who'll never flinch from a fray-ee-ay-ee-ay!");
			break;
		case 9:
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "NO, THIS IS PATRICK!");
			break;
		case 10:
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "here come dat boi!!!!!!");
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "o shit waddup!");
			break;
		case 11:
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "NiNJA Owns Battlefield!");
			break;
		case 12:
			pMessages.SendTextMessage(ChannelType[vars.iChannelType].Result, ChannelType[vars.iMessageType].Result, "Hate hackers? Call 866-543-5435 bitch.");
			break;
		}
	}
}

void MainChatCommandsMenu::Init() {
	SetParentSubmenu<MainMenu>();
	SetName("Chat Messages");

	addOption(ScrollOption<ChatChannelType>(SCROLL, "Channel Type").canLoop()
		.addScroll(vars.iChannelType, 0, NUMOF(ChannelType) - 3, ChannelType)
		.addTooltip("Sets the group of players to send the \nmessage to."));

	addOption(ScrollOption<ChatChannelType>(SCROLL, "Message Type").canLoop()
		.addScroll(vars.iMessageType, 0, NUMOF(ChannelType) - 1, ChannelType)
		.addTooltip("Sets the message type."));

	addOption(ButtonOption("Input Message")
		.addKeyboard(SendMessageKeyboardCallback)
		.addTooltip("Input your own message."));

	addOption(ToggleOption("Kill Feed - Kills")
		.addToggle(vars.bCustomKillFeedKill)
		.addTooltip("Displays a message in chat when you get a \nkill."));

	addOption(ToggleOption("Kill Feed - Death")
		.addToggle(vars.bCustomKillFeedDeath)
		.addTooltip("Displays a message in chat when you get \nkilled."));

	addOption(BreakOption("Presets"));

	addOption(ButtonOption("I'm using NiNJA Engine!")
		.addFunction(SendPreset)
		.addTooltip("Send this preset message."));

	addOption(ButtonOption("Buy NiNJA at xbl.ninja!")
		.addFunction(SendPreset)
		.addTooltip("Send this preset message."));

	addOption(ButtonOption("<3 NiNJA <3")
		.addFunction(SendPreset)
		.addTooltip("Send this preset message."));

	addOption(ButtonOption("GG EZ")
		.addFunction(SendPreset)
		.addTooltip("Send this preset message."));

	addOption(ButtonOption("GLHF")
		.addFunction(SendPreset)
		.addTooltip("Send this preset message."));

	addOption(ButtonOption("Woah, who's cheating? smh")
		.addFunction(SendPreset)
		.addTooltip("Send this preset message."));

	addOption(ButtonOption("Navy Seal Meme")
		.addFunction(SendPreset)
		.addTooltip("Send this preset message."));

	addOption(ButtonOption("Perry the Platypus")
		.addFunction(SendPreset)
		.addTooltip("Send this preset message."));

	addOption(ButtonOption("NO, THIS IS PATRICK!")
		.addFunction(SendPreset)
		.addTooltip("Send this preset message."));

	addOption(ButtonOption("Here Come Dat Boi")
		.addFunction(SendPreset)
		.addTooltip("Send this preset message."));

	addOption(ButtonOption("NiNJA Owns Battlefield!")
		.addFunction(SendPreset)
		.addTooltip("Send this preset message."));

	addOption(ButtonOption("Hate hackers? Call 866-543-5435 bitch.")
		.addFunction(SendPreset)
		.addTooltip("Send this preset message."));
}

/*Update once when submenu is opened*/
void MainChatCommandsMenu::UpdateOnce() {}

/*Update while submenu is active*/
void MainChatCommandsMenu::Update() {}

/*Background update*/
void MainChatCommandsMenu::FeatureUpdate() {}

/*Singleton*/
MainChatCommandsMenu* _instance;
MainChatCommandsMenu* MainChatCommandsMenu::GetInstance() {
	if (_instance == nullptr) {
		_instance = new MainChatCommandsMenu();
		_instance->Init();
		pSubmenuHandler.AddSubmenu(_instance);
	}
	return _instance;
}
MainChatCommandsMenu::~MainChatCommandsMenu() { delete _instance; }