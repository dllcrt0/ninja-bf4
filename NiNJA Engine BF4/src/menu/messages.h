#pragma once

class Messages {
public:
	void SendTextMessage(ChatChannelType channelType, ChatChannelType messageType, const char* message);
};

extern Messages pMessages;