#pragma once

#include "IRCConnectedObject.h"
#include "IRCMessageIO.h"

namespace SteamIRC
{
	class CIRCEnvironment :
		public CIRCConnectedObject
	{
	public:
		CIRCEnvironment();
		virtual ~CIRCEnvironment(void);
	};
}
