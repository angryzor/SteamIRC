#pragma once
#include "WinSock2.h"
#include "IRCClient.h"

namespace SteamIRC
{
	class CIRCWinSock2 :
		public WinSock2::CWinSock2
	{
	public:
		CIRCWinSock2(void);
		CIRCClient* MakeIRCClient(CIRCEnvironment& env);
		virtual ~CIRCWinSock2(void);
	};
}
