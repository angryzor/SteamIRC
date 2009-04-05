#pragma once

namespace SteamIRC {

	class CIRCContext : public Object
	{
	public:
		virtual bool UserInput(String txt) = 0;
		virtual bool AcceptIncoming(IRCMessage& msg) = 0;
		virtual ~CIRCContext(void);
	};

}