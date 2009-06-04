#include "IRCContextWithCommands.h"
#include <algorithm>

#include "tier0/memdbgon.h"
namespace SteamIRC {

	CIRCContextWithCommands::CIRCContextWithCommands(std::string title, CIRCEnvironment& env) : CIRCContext(title), env_(env)
	{
	}

	bool CIRCContextWithCommands::UserInput(const std::string& txt) {
		try {
			if(txt[0] == '/') {
				// Is a command.
				std::istringstream iss;
				iss.str(txt.substr(1));

				std::string cmnd;
				iss >> cmnd;
				std::transform(cmnd.begin(), cmnd.end(), cmnd.begin(), ::tolower);

				if(!ProcessUserCommand(cmnd, iss))
					throw std::runtime_error("Command does not exist.");
				return true;
			}
			return false;
		}
		catch(std::runtime_error e) {
			buffer_ += std::string("ERROR: ") + e.what() + "\n";
			return true;
		}
	}
	
	bool CIRCContextWithCommands::AcceptIncoming(const IRCMessage& msg) {
		switch(msg.Cmnd) {
		case NOTICE:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return false;

			buffer_ += "\x03" "05-";
			buffer_ += msg.Origin.nick;
			buffer_ += "- ";
			buffer_ += msg.Parameters[1];
			buffer_ += "\x03\n";
			break;
		case RPL_AWAY:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return false;
			buffer_ += msg.Parameters[1];
			buffer_ += " is away (";
			buffer_ += msg.Parameters[2];
			buffer_ += ")\n";
			break;
		default:
			return false;
		}
		return true;
	}


	CIRCContextWithCommands::~CIRCContextWithCommands(void)
	{
	}

}
