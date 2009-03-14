#pragma once

// I call this ugly. Not gonna bother explaining the reason
namespace SteamIRC
{
	class CIRCMessageIO;
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
		static CIRCMessageIO* ircIO;
	};
}

#include "IRCMessageIO.h"

