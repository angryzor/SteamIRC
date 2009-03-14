#pragma once
namespace SteamIRC
{
	class CIRCMessageIO;
}

#include "IRCClient.h"
#include "IRCMessage.h"
#include "Object.h"

namespace SteamIRC
{
	class CIRCMessageIO :
		public Object
	{
	public:
		CIRCMessageIO(CIRCClient* l_conn);
		virtual void ProcessReceived(IRCMessage& msg);
		virtual void JoinChannel(String channame);
		virtual void PartChannel(String channame, String message = "");
		virtual void Chat(String dest, String message);
		virtual ~CIRCMessageIO(void);
	private:
		CIRCClient* conn;
	};
}
