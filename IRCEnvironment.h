#pragma once
namespace SteamIRC
{
	class CIRCClient;
	class CIRCContext;
}

#include <set>
#include <string>

namespace SteamIRC
{
	class IRCGui;
	class IRCMessage;
	class CIRCEnvironment
	{
	public:
		CIRCEnvironment();
		void ProcessReceived(const IRCMessage& msg);
		void SetConnection(CIRCClient* conn);
		void Add(CIRCContext* con);
		void Remove(CIRCContext* con);
		void GetContexts();
		void Send(const IRCMessage& msg) const;
		void SetActiveContext(CIRCContext* con);
		CIRCContext* GetActiveContext();
		void UserInput(const std::string& txt);
		void SetGui(IRCGui* gui);
		IRCGui* GetGui();
		virtual ~CIRCEnvironment(void);
	private:
		CIRCEnvironment& operator=(CIRCEnvironment&);
		typedef std::set<CIRCContext*> context_set;
		void Cleanup();
		CIRCClient* conn_;
		context_set ctxts_;
		CIRCContext* act_;
		IRCGui* gui_;
	};
}
