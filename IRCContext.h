#pragma once
#include "StdString.h"
#include "IRCMessage.h"

namespace SteamIRC {

	class CIRCContext
	{
	public:
		virtual bool UserInput(const String& txt) = 0;
		virtual bool AcceptIncoming(const IRCMessage& msg) = 0;
		virtual ~CIRCContext(void);
		String GetBuffer() const;
		String GetTitle() const;
	protected:
		CIRCContext(String title);
		String buffer_;
		String title_;
	};

}