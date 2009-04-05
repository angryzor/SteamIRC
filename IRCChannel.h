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
		String get_name();
	private:
		String name_;
		CIRCEnvironment& env_;
	};
}
