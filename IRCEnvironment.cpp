#include "tier2/tier2.h"
#include "IRCClient.h"
#include "IRCEnvironment.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#include "IRCNetwork.h"

namespace SteamIRC
{
	CIRCEnvironment::CIRCEnvironment(String nick) : conn_(NULL)
	{
		ctxts_.insert(new CIRCNetwork(*this, nick));
	}

	void CIRCEnvironment::ProcessReceived(IRCMessage& msg)
	{
		for(ctxts_.const_iterator i = ctxts_.begin(); i != ctxts_.end(); i++)
			if((*i)->AcceptIncoming(msg))
				return;
		Warning("Received unprocessed message!!! | %s\r\n", msg.GetString());
	}

	void CIRCEnvironment::Send(IRCMessage& msg)
	{
		conn_->Send(msg);
	}

	void CIRCEnvironment::SetConnection(CIRCClient& conn)
	{
		conn_ = conn;
	}

	void CIRCEnvironment::Add(CIRCContext* con)
	{
		ctxts_.insert(con);
	}

	void CIRCEnvironment::Cleanup()
	{
		for(ctxts_.const_iterator i = ctxts_.begin(); i != ctxts_.end(); i++)
			delete *i;
		ctxts_.clear();
	}

	CIRCEnvironment::~CIRCEnvironment(void)
	{
		Cleanup();
	}
}
