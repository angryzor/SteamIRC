#pragma once
#include "IRCContext.h"
#include <sstream>

namespace SteamIRC {

	class CIRCContextWithCommands :
		public CIRCContext
	{
	public:
		virtual bool UserInput(const String& txt);
		virtual bool ProcessUserCommand(const String& cmnd, std::istringstream& params) = 0;
		virtual ~CIRCContextWithCommands(void);
	protected:
		CIRCContextWithCommands(String title);
	};

}
