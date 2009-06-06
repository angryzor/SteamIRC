#pragma once
#include "irccommunicator.h"

namespace SteamIRC {
	class CIRCPMContext :
		public CIRCCommunicator
	{
	public:
		CIRCPMContext(CIRCEnvironment& env, std::string name);
		virtual bool ProcessUserCommand(const std::string& cmnd, std::istringstream& params);
		virtual AcceptReturnValue AcceptIncoming(const IRCMessage& msg);
		virtual void DestroyContext();
		virtual ~CIRCPMContext(void);
	};
}
