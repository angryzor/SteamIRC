#pragma once
namespace SteamIRC
{
	class CIRCClient;
	class CIRCContext;
}

#include "IRCMessage.h"
#include <set>

namespace SteamIRC
{
	class CIRCEnvironment
	{
	public:
		CIRCEnvironment();
		virtual void ProcessReceived(const IRCMessage& msg);
		virtual void SetConnection(CIRCClient& conn);
		virtual void Add(CIRCContext* con);
		virtual void Send(const IRCMessage& msg) const;
		virtual ~CIRCEnvironment(void);
	private:
		void Cleanup();
		CIRCClient* conn_;
		std::set<CIRCContext*> ctxts_;
	};
}
