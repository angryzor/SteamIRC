#include "tier2/tier2.h"
#include "IRCClient.h"
#include "IRCEnvironment.h"
#include "IRCNetwork.h"
#include "IRCGui.h"
#include "IRCMessage.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


namespace SteamIRC
{
	CIRCEnvironment::CIRCEnvironment() : conn_(NULL)
	{
	}

	void CIRCEnvironment::ProcessReceived(const IRCMessage& msg)
	{
		for(context_set::const_iterator i = ctxts_.begin(); i != ctxts_.end(); i++)
			if((*i)->AcceptIncoming(msg)) {
				gui_->Update();
				return;
			}
		Warning("Received unprocessed message!!! | %s\r\n", msg.GetString().c_str());
	}

	void CIRCEnvironment::Send(const IRCMessage& msg) const
	{
		conn_->Send(msg);
	}

	void CIRCEnvironment::SetConnection(CIRCClient* conn)
	{
		conn_ = conn;
	}

	void CIRCEnvironment::Add(CIRCContext* con)
	{
		if(ctxts_.empty())
			SetActiveContext(con);
		ctxts_.insert(con);
	}

	void CIRCEnvironment::Remove(CIRCContext* con)
	{
		ctxts_.erase(con);
	}

	void CIRCEnvironment::GetContexts() {
		ctxts_;
	}

	void CIRCEnvironment::SetActiveContext(SteamIRC::CIRCContext *con) {
		act_ = con;
	}

	SteamIRC::CIRCContext* CIRCEnvironment::GetActiveContext() {
		return act_;
	}

	void CIRCEnvironment::UserInput(const std::string& txt) {
		act_->UserInput(txt);
	}

	void CIRCEnvironment::SetGui(IRCGui* gui){
		gui_ = gui;
	}

	IRCGui* CIRCEnvironment::GetGui(){
		return gui_;
	}

	void CIRCEnvironment::Cleanup()
	{
		for(context_set::const_iterator i = ctxts_.begin(); i != ctxts_.end(); i++)
			delete *i;
		ctxts_.clear();
	}

	CIRCEnvironment::~CIRCEnvironment(void)
	{
		Cleanup();
	}
}
