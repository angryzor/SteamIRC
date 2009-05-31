#include "IRCChannel.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

namespace SteamIRC
{
	CIRCChannel::CIRCChannel(CIRCEnvironment& env, String name) : CIRCCommunicator(name, env)
	{
	}

	void CIRCChannel::Part()
	{
		IRCMessage msg(PART);
		msg.SetParam(0, id_);
		env_.Send(msg);
		env_.Remove(this);
		delete this;
	}
	
	bool CIRCChannel::AcceptIncoming(IRCMessage& msg) {
		return CIRCCommunicator::AcceptIncoming(msg);
	}

	bool CIRCChannel::ProcessUserCommand(const String& cmnd, std::istringstream& params) {
		return false;
	}
		
	CIRCChannel::~CIRCChannel(void)
	{
	}
}
