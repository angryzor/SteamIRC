#pragma once
#include "IRCMessage.h"
#include "IRCEnvironment.h"
#include "IRCContextWithCommands.h"
#include <sstream>
#include "IRCDataStructures.h"

namespace SteamIRC {
	using namespace std;

	class CIRCNetwork :
		public CIRCContextWithCommands
	{
	public:
		CIRCNetwork(String uri, CIRCEnvironment& env, IRCUserInfo& usr);
		virtual bool AcceptIncoming(const IRCMessage& msg);
		virtual bool ProcessUserCommand(const String& cmnd, istringstream& params);
		virtual void Join(String chan);
		virtual ~CIRCNetwork(void);
	private:
		CIRCNetwork& operator=(CIRCNetwork&);
		CIRCEnvironment& env_;
		IRCUserInfo& usr_;
	};

}
