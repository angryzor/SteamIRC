#include "IRCContextWithCommands.h"

namespace SteamIRC {

	CIRCContextWithCommands::CIRCContextWithCommands(String title) : CIRCContext(title)
	{
	}

	bool CIRCContextWithCommands::UserInput(const String& txt) {
		try {
			if(txt[0] == '/') {
				// Is a command.
				std::istringstream iss;
				iss.str(txt.substr(1));

				String cmnd;
				iss >> cmnd;
				cmnd.ToLower();

				return ProcessUserCommand(cmnd, iss);
			}
			return false;
		}
		catch(std::runtime_error e) {
			buffer_ += String("ERROR: ") + e.what();
			return true;
		}
	}

	CIRCContextWithCommands::~CIRCContextWithCommands(void)
	{
	}

}
