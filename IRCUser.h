#pragma once

#include "IRCConnectedObject.h"

namespace SteamIRC
{
	class CIRCUser :
		public CIRCConnectedObject
	{
	public:
		CIRCUser(void);
		virtual ~CIRCUser(void);
		String Name;
	};
}
