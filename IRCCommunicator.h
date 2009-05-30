#pragma once
#include "IRCContextWithCommands.h"
#include "IRCMessage.h"
#include "IRCEnvironment.h"

namespace SteamIRC {

	class CIRCCommunicator :
		public CIRCContextWithCommands
	{
	public:
		virtual bool UserInput(const String& txt);
		virtual bool AcceptIncoming(const IRCMessage& msg);
		virtual void ProcessUserCommand(String cmnd, std::istringstream& params);
		virtual ~CIRCCommunicator(void);
	protected:
		CIRCCommunicator(String id, CIRCEnvironment& env);
		String id_;
		CIRCEnvironment& env_;
	};

}
