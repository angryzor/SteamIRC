#pragma once
#include <string>
#include "IRCMessage.h"

namespace SteamIRC {

	class CIRCContext
	{
	public:
		enum AcceptReturnValue {
			ARV_FAIL = 0,
			ARV_PROCESSED_BUT_CONTINUE = 1,
			ARV_PROCESSED = 2
		};
		virtual bool UserInput(const std::string& txt) = 0;
		virtual AcceptReturnValue AcceptIncoming(const IRCMessage& msg) = 0;
		virtual ~CIRCContext(void);
		std::string GetBuffer() const;
		std::string GetTitle() const;
		void SetTitle(std::string newT);
	protected:
		CIRCContext(std::string title);
		std::string buffer_;
	private:
		std::string title_;
	};

}