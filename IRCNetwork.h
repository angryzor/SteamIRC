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
		CIRCNetwork(std::string uri, CIRCEnvironment& env);
		virtual bool AcceptIncoming(const IRCMessage& msg);
		virtual bool ProcessUserCommand(const std::string& cmnd, istringstream& params);
		virtual void Join(std::string chan);
		virtual ~CIRCNetwork(void);
	private:
		CIRCNetwork& operator=(CIRCNetwork&);
	};

}
