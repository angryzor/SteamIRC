#pragma once
#include "IRCContext.h"
#include <sstream>
#include "IRCEnvironment.h"

namespace SteamIRC {

	class CIRCContextWithCommands :
		public CIRCContext
	{
	public:
		virtual bool UserInput(const std::string& txt);
		virtual bool ProcessUserCommand(const std::string& cmnd, std::istringstream& params);
		virtual AcceptReturnValue AcceptIncoming(const IRCMessage& msg);
		virtual ~CIRCContextWithCommands(void);
	protected:
		CIRCContextWithCommands(std::string title, CIRCEnvironment& env);
		CIRCEnvironment& env_;
	private:
		CIRCContextWithCommands& operator=(CIRCContextWithCommands&);
	};

}
