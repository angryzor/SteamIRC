#include "IRCChannel.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

namespace SteamIRC
{
	CIRCChannel::CIRCChannel() : isJoined(false)
	{
	}

	void CIRCChannel::Join(String name)
	{
		Name = name;
		ircIO->JoinChannel(name);
		isJoined = true;
	}

	void CIRCChannel::Part()
	{
		ircIO->PartChannel(Name);
		isJoined = false;
	}
		

	void CIRCChannel::Chat(String message)
	{
		ircIO->Chat(Name, message);
	}

	CIRCChannel::~CIRCChannel(void)
	{
	}
}
