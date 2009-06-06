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

	void CIRCCommunicator::SetID(std::string id) {
		id_ = id;
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
			else throw std::runtime_error("USAGE: /me <does something>");
			return true;
		}
		return CIRCContextWithCommands::ProcessUserCommand(cmnd, params);
	}

	CIRCContext::AcceptReturnValue CIRCCommunicator::AcceptIncoming(const IRCMessage& msg) {
		switch(msg.Cmnd) {
		case PRIVMSG: 
			{
				if(msg.Parameters[0] != id_) return CIRCContextWithCommands::AcceptIncoming(msg);

				Message(msg.Origin.nick, msg.Parameters[1]);
			}
			break;
		default:
			return CIRCContextWithCommands::AcceptIncoming(msg);
		}
		return ARV_PROCESSED;
	}

	void CIRCCommunicator::Message(std::string from, std::string msg) {
		std::istringstream iss(msg);
		std::string tmp;
		if((iss >> tmp) && (tmp == "\x01" "ACTION") && std::getline(iss,tmp)) {
			buffer_ += "\x03" "06* ";
			buffer_ += from;
			buffer_ += tmp;
			buffer_ += "\x03\n";
		}
		else {
			buffer_ += "<";
			buffer_ += from;
			buffer_ += "> ";
			buffer_ += msg;
			buffer_ += "\n";
		}
	}


	CIRCCommunicator::~CIRCCommunicator(void)
	{
	}

}
