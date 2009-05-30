#pragma once
#include "IRCMessage.h"
#include "IRCEnvironment.h"
#include "IRCContextWithCommands.h"
#include <sstream>

namespace SteamIRC {

	class CIRCNetwork :
		public CIRCContextWithCommands
	{
	public:
		CIRCNetwork(CIRCEnvironment& env, IRCUserInfo& usr);
		virtual bool AcceptIncoming(IRCMessage& msg);
		virtual bool ProcessUserCommand(const String& cmnd, istringstream& params);
		virtual ~CIRCNetwork(void);
	private:
		CIRCEnvironment& env_;
		IRCUserInfo& usr_;
	};

}
