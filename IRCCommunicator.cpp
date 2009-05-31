#include "IRCCommunicator.h"

namespace SteamIRC {

	CIRCCommunicator::CIRCCommunicator(String id, CIRCEnvironment& env) : id_(id), env_(env), CIRCContextWithCommands(id)
	{
	}

	bool CIRCCommunicator::UserInput(const String& txt) {
		if(!CIRCContextWithCommands::UserInput(txt)) {
			IRCMessage msg(PRIVMSG);
			msg.SetParam(0, id_);
			msg.SetParam(1, txt);
			env_.Send(msg);
		}
		return true;
	}

	bool CIRCCommunicator::AcceptIncoming(const IRCMessage& msg) {
		switch(msg.Cmnd) {
		case PRIVMSG:
			if(msg.Origin.nick == id_) {
				buffer_ += msg.Parameters[0] + "\r\n";
				return true;
			}
			return false;
		default:
			return false;
		}
	}

	CIRCCommunicator::~CIRCCommunicator(void)
	{
	}

}
