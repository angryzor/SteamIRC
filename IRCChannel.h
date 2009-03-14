#pragma once

#include "IRCConnectedObject.h"

namespace SteamIRC
{
	class CIRCChannel :
		public CIRCConnectedObject
	{
	public:
		CIRCChannel();
		virtual ~CIRCChannel(void);
		virtual void Join(String name);
		virtual void Part();
		virtual void Chat(String message);
		bool isJoined;
		String Name;
	private:

	};
}
