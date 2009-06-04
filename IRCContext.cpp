#include "IRCContext.h"

#include "tier0/memdbgon.h"
namespace SteamIRC {

	CIRCContext::CIRCContext(std::string title) : title_(title) {
	}

	std::string CIRCContext::GetBuffer() const {
		return buffer_;
	}

	std::string CIRCContext::GetTitle() const {
		return title_;
	}

	CIRCContext::~CIRCContext(void)
	{
	}
}
