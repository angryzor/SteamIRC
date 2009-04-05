#pragma once


namespace SteamIRC
{
	class CIRCEnvironment;
}

#define _WINSOCKAPI_
#include "Object.h"

namespace SteamIRC
{
	class CIRCConnectedObject :
		public Object
	{
	public:
		CIRCConnectedObject();
		virtual ~CIRCConnectedObject(void);
	protected:
		static CIRCEnvironment* ircIO;
	};
}

#include "IRCEnvironment.h"

