#include "IRCContext.h"

namespace SteamIRC {

	CIRCContext::CIRCContext(String title) : title_(title) {
	}

	String CIRCContext::GetBuffer() const {
		return buffer_;
	}

	String CIRCContext::GetTitle() const {
		return title_;
	}

	CIRCContext::~CIRCContext(void)
	{
	}
}
