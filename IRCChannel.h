#pragma once

#include "IRCConnectedObject.h"

namespace SteamIRC
{
	class CIRCChannel :
		public CIRCContext, CIRCConnectedObject
	{
	public:
		CIRCChannel(CIRCEnvironment& env, String name);
		virtual ~CIRCChannel(void);
		virtual bool UserInput(String txt);
		virtual bool AcceptIncoming(IRCMessage& msg);
		String get_name();
	private:
		String name_;
		CIRCEnvironment& env_;
	};
}
