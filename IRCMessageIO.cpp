#include "tier2/tier2.h"
#include "IRCMessageIO.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

namespace SteamIRC
{
	CIRCMessageIO::CIRCMessageIO(CIRCClient* l_conn) : conn(l_conn)
	{
	}

	void CIRCMessageIO::ProcessReceived(IRCMessage& msg)
	{
		IRCMessage reply;
		switch(msg.Cmnd)
		{
		case PING:
			Warning("Ping?");
			reply.SetCommand(PONG);
			reply.SetParam(0, msg.Parameters[0]);
			conn->Send(reply);
			Warning(" Pong!\r\n");
			break;
		default:
			Warning(msg.GetString());
			break;
		}
	}

	void CIRCMessageIO::JoinChannel(String channame)
	{
		IRCMessage msg(JOIN);
		msg.SetParam(0, channame);
		conn->Send(msg);
	}

	void CIRCMessageIO::PartChannel(String channame, String message)
	{
		IRCMessage msg(PART);
		msg.SetParam(0, channame);
		if(message.length() > 0) msg.SetParam(1, message);
		conn->Send(msg);
	}

	void CIRCMessageIO::Chat(String dest, String message)
	{
		IRCMessage msg(PRIVMSG);
		msg.SetParam(0, dest);
		msg.SetParam(1, message);
		conn->Send(msg);
	}

	CIRCMessageIO::~CIRCMessageIO(void)
	{
	}
}
