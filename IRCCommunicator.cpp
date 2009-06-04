#include "IRCCommunicator.h"
#include <sstream>
#include <xstring>

#include "tier0/memdbgon.h"
namespace SteamIRC {

	CIRCCommunicator::CIRCCommunicator(std::string id, CIRCEnvironment& env) : id_(id), CIRCContextWithCommands(id, env)
	{
	}

	bool CIRCCommunicator::UserInput(const std::string& txt) {
		if(!CIRCContextWithCommands::UserInput(txt)) {
			IRCMessage msg(PRIVMSG);
			msg.AddParam(id_);
			msg.AddParam(txt);
			env_.Send(msg);
			buffer_ += "<";
			buffer_ += env_.GetUInfo()->Nick;
			buffer_ += "> ";
			buffer_ += txt;
			buffer_ += "\n";
		}
		return true;
	}

	bool CIRCCommunicator::ProcessUserCommand(std::string cmnd, std::istringstream& params) {
		if(cmnd == "me") {
			std::string tmp;
			if(std::getline(params,tmp)) {
				std::string out("\x01" "ACTION");
				out += tmp;
				out += "\x01";
				IRCMessage msg(PRIVMSG);
				msg.AddParam(id_);
				msg.AddParam(out);
				env_.Send(msg);

				buffer_ += "\x03" "06* ";
				buffer_ += env_.GetUInfo()->Nick;
				buffer_ += tmp;
				buffer_ += "\x03\n";
			}
			return true;
		}
		return false;
	}

	bool CIRCCommunicator::AcceptIncoming(const IRCMessage& msg) {
		switch(msg.Cmnd) {
		case PRIVMSG: 
			{
				if(msg.Parameters[0] != id_) return CIRCContextWithCommands::AcceptIncoming(msg);

				std::istringstream iss(msg.Parameters[1]);
				std::string tmp;
				if((iss >> tmp) && (tmp == "\x01" "ACTION") && std::getline(iss,tmp)) {
					buffer_ += "\x03" "06* ";
					buffer_ += msg.Origin.nick;
					buffer_ += tmp;
					buffer_ += "\x03\n";
				}
				else {
					buffer_ += "<";
					buffer_ += msg.Origin.nick;
					buffer_ += "> ";

					buffer_ += msg.Parameters[1];
					buffer_ += "\n";
				}
			}
			break;
		default:
			return CIRCContextWithCommands::AcceptIncoming(msg);
		}
		return true;
	}

	CIRCCommunicator::~CIRCCommunicator(void)
	{
	}

}
