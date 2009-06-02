#pragma once
namespace SteamIRC
{
	class CIRCClient;
	class CIRCContext;
}

#include <set>
#include <string>
#include "IRCDataStructures.h"

namespace SteamIRC
{
	class IRCGui;
	class IRCMessage;
	class CIRCEnvironment
	{
	public:
		typedef std::set<CIRCContext*> context_set;
		CIRCEnvironment();
		void ProcessReceived(const IRCMessage& msg);
		void SetConnection(CIRCClient* conn);
		void Add(CIRCContext* con);
		void Remove(CIRCContext* con);
		CIRCEnvironment::context_set* GetContexts();
		void Send(const IRCMessage& msg) const;
		void SetActiveContext(CIRCContext* con);
		CIRCContext* GetActiveContext();
		void UserInput(const std::string& txt);
		void SetGui(IRCGui* gui);
		IRCGui* GetGui();
		void SetUInfo(IRCUserInfo& uInfo);
		IRCUserInfo* GetUInfo();
		virtual ~CIRCEnvironment(void);
		void Cleanup();
	private:
		CIRCEnvironment& operator=(CIRCEnvironment&);
		CIRCClient* conn_;
		context_set ctxts_;
		CIRCContext* act_;
		IRCGui* gui_;
		IRCUserInfo uInfo_;
	};
}
