#include "IRCChannel.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

namespace SteamIRC
{
	CIRCChannel::CIRCChannel(CIRCEnvironment& env, String name) : env_(env), name_(name)
	{
	}

	void CIRCChannel::Part()
	{
		ircIO->PartChannel(Name);
		isJoined = false;
	}
		
	CIRCChannel::~CIRCChannel(void)
	{
	}
}
