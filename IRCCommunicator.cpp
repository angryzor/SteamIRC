#include "IRCCommunicator.h"

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

	bool CIRCCommunicator::AcceptIncoming(const IRCMessage& msg) {
		switch(msg.Cmnd) {
		case PRIVMSG:
			if(msg.Parameters[0] != id_) return CIRCContextWithCommands::AcceptIncoming(msg);

			buffer_ += "<";
			buffer_ += msg.Origin.nick;
			buffer_ += "> ";
			buffer_ += msg.Parameters[1];
			buffer_ += "\n";
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
