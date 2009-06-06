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
		virtual AcceptReturnValue AcceptIncoming(const IRCMessage& msg);
		virtual bool ProcessUserCommand(const std::string& cmnd, istringstream& params);
		virtual void Join(std::string chan);
		virtual void BuildPMContext(std::string other, std::string msg);
		virtual ~CIRCNetwork(void);
	private:
		CIRCNetwork& operator=(CIRCNetwork&);
	};

}
