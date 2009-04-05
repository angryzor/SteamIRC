#pragma once
#include "IRCMessage.h"
#include "IRCEnvironment.h"
#include <sstream>

namespace SteamIRC {

	class CIRCNetwork :
		public CIRCContext
	{
	public:
		CIRCNetwork(CIRCEnvironment& env, String nick);
		virtual bool AcceptIncoming(IRCMessage& msg);
		virtual bool UserInput(String txt);
		virtual ~CIRCNetwork(void);
	private:
		CIRCEnvironment& env_;
		String nick_;
	};

}
