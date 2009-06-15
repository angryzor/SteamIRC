#include "IRCContextWithCommands.h"
#include <algorithm>
#include <xstring>

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

	bool CIRCContextWithCommands::ProcessUserCommand(const std::string& cmnd, std::istringstream& params) {
		if(cmnd == "join") {
			std::string chan;
			std::string key;
			if(!(params >> chan)) throw std::runtime_error("USAGE: /join #channame [key]");

			IRCMessage msg(JOIN);
			msg.AddParam(chan);
			if(params >> key) msg.AddParam(key);

			env_.Send(msg);
			return true;
		}
		else if(cmnd == "msg") {
			std::string tgt;
			std::string msg;
			if(!(params >> tgt) || !std::getline(params, msg)) throw std::runtime_error("USAGE: /msg <target nick/channel> <message>");

			IRCMessage imsg(PRIVMSG);
			imsg.AddParam(tgt);
			imsg.AddParam(msg.substr(1)); // Drop the leading space
			env_.Send(imsg);

			buffer_ += "-> *";
			buffer_ += tgt;
			buffer_ += "* ";
			buffer_ += msg;
			buffer_ += "\n";
			return true;
		}
		else if(cmnd == "notice") {
			std::string tgt;
			std::string msg;
			if(!(params >> tgt) || !std::getline(params, msg)) throw std::runtime_error("USAGE: /notice <target nick/channel> <message>");

			IRCMessage imsg(NOTICE);
			imsg.AddParam(tgt);
			imsg.AddParam(msg.substr(1)); // Drop the leading space
			env_.Send(imsg);

			buffer_ += "-> -";
			buffer_ += tgt;
			buffer_ += "- ";
			buffer_ += msg;
			buffer_ += "\n";
			return true;
		}
		else if(cmnd == "nick") {
			std::string newnick;
			if(!(params >> newnick)) throw std::runtime_error("USAGE: /nick <new nick>");

			IRCMessage msg(NICK);
			msg.AddParam(newnick);
			env_.Send(msg);

			env_.GetUInfo()->Nick = newnick;
/*
			buffer_ += "\x03" "03* You are now known as ";
			buffer_ += newnick;
			buffer_ += ".\x03\n";
*/
			return true;
		}
		return false;
	}

	CIRCContext::AcceptReturnValue CIRCContextWithCommands::AcceptIncoming(const IRCMessage& msg) {
		switch(msg.Cmnd) {
		case NOTICE:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return ARV_FAIL;
			buffer_ += "\x03" "05-";
			buffer_ += msg.Origin.nick;
			buffer_ += "- ";
			buffer_ += msg.Parameters[1];
			buffer_ += "\x03\n";
			break;
		case RPL_AWAY:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return ARV_FAIL;
			buffer_ += msg.Parameters[1];
			buffer_ += " is away (";
			buffer_ += msg.Parameters[2];
			buffer_ += ")\n";
			break;
		case INVITE:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return ARV_FAIL;
			buffer_ += "\x03" "03* ";
			buffer_ += msg.Origin.nick;
			buffer_ += " (";
			buffer_ += msg.Origin.user;
			buffer_ += "@";
			buffer_ += msg.Origin.addr;
			buffer_ += ") invites you to join ";
			buffer_ += msg.Parameters[1];
			buffer_ += ".\x03\n";
			break;
		case NICK:
			buffer_ += "\x03" "03* ";
			if(msg.Parameters[0] == env_.GetUInfo()->Nick)
				buffer_ += "You are now known as ";
			else {
				buffer_ += msg.Origin.nick;
				buffer_ += " is now known as ";
			}
			buffer_ += msg.Parameters[0];
			buffer_ += ".\x03\n";
			return ARV_PROCESSED_BUT_CONTINUE;
		default:
			return ARV_FAIL;
		}
		return ARV_PROCESSED;
	}


	CIRCContextWithCommands::~CIRCContextWithCommands(void)
	{
	}

}
