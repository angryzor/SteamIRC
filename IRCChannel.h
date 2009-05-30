#pragma once

#include "IRCCommunicator.h"

namespace SteamIRC
{
	class CIRCChannel :
		public CIRCCommunicator
	{
	public:
		CIRCChannel(CIRCEnvironment& env, String name);
		virtual ~CIRCChannel(void);
		virtual bool UserInput(String txt);
		virtual bool AcceptIncoming(IRCMessage& msg);
	private:
	};
}
