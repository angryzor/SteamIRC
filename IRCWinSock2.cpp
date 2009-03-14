#include "IRCWinSock2.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

namespace SteamIRC
{
	CIRCWinSock2::CIRCWinSock2(void)
	{
	}

	CIRCClient* CIRCWinSock2::MakeIRCClient()
	{
		return new CIRCClient();
	}

	CIRCWinSock2::~CIRCWinSock2(void)
	{
	}
}
