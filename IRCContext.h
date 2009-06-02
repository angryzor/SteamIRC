#pragma once
#include <string>
#include "IRCMessage.h"

namespace SteamIRC {

	class CIRCContext
	{
	public:
		virtual bool UserInput(const std::string& txt) = 0;
		virtual bool AcceptIncoming(const IRCMessage& msg) = 0;
		virtual ~CIRCContext(void);
		std::string GetBuffer() const;
		std::string GetTitle() const;
	protected:
		CIRCContext(std::string title);
		std::string buffer_;
		std::string title_;
	};

}