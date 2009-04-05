#pragma once
namespace SteamIRC
{
	class CIRCClient;
}

#include "IRCMessage.h"
#include <set>

namespace SteamIRC
{
	class CIRCEnvironment
	{
	public:
		CIRCEnvironment(String nick);
		virtual void ProcessReceived(IRCMessage& msg);
		virtual void SetConnection(CIRCClient& conn);
		virtual void Add(CIRCContext* con);
		virtual ~CIRCEnvironment(void);
	private:
		void Cleanup()
		CIRCClient* conn_;
		set<CIRCContext*> ctxts_;
	};
}
