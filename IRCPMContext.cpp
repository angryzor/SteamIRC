#include "IRCPMContext.h"

namespace SteamIRC {

	CIRCPMContext::CIRCPMContext(CIRCEnvironment& env, std::string name) : CIRCCommunicator(name, env)
	{
	}

	bool CIRCPMContext::ProcessUserCommand(const std::string& cmnd, std::istringstream& params) {
		if(cmnd == "part" || cmnd == "leave") {
			DestroyContext();
			return true;
		}
		return CIRCCommunicator::ProcessUserCommand(cmnd, params);
	}

	// Overrides Communicator acceptincoming...
	CIRCContext::AcceptReturnValue CIRCPMContext::AcceptIncoming(const IRCMessage& msg) {
		switch(msg.Cmnd) {
		case PRIVMSG: 
			{
				if(msg.Origin.nick != id_ ||
					msg.Parameters[0] != env_.GetUInfo()->Nick) return CIRCContextWithCommands::AcceptIncoming(msg);

				Message(id_, msg.Parameters[1]);
			}
			break;
		case NICK:
			if(msg.Origin.nick == id_) {
				SetID(msg.Parameters[0]);
				SetTitle(msg.Parameters[0]);
			}
			return CIRCContextWithCommands::AcceptIncoming(msg);
		default:
			return CIRCContextWithCommands::AcceptIncoming(msg);
		}
		return ARV_PROCESSED;
	}

	void CIRCPMContext::DestroyContext() {
		if(env_.GetActiveContext() == this) {
			CIRCEnvironment::context_set::iterator i = env_.GetContexts()->find(this);
			i--;
			env_.SetActiveContext(*i);
		}
		env_.Remove(this);
		delete this;
	}

	CIRCPMContext::~CIRCPMContext(void)
	{
	}

}
