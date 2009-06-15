#include "tier2/tier2.h"
#include "IRCClient.h"
#include "IRCEnvironment.h"
#include "IRCNetwork.h"
#include "IRCGui.h"
#include "IRCMessage.h"
#include "logging.h"
#include <boost/logging/format/named_write.hpp>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


namespace SteamIRC
{
	CIRCEnvironment::CIRCEnvironment() : conn_(NULL), act_(NULL)
	{
	}

	void CIRCEnvironment::ProcessReceived(const IRCMessage& msg)
	{
		L_ << "CIRCEnvironment::ProcessReceived() - About to process message " << msg.GetString();
		CIRCContext::AcceptReturnValue val(act_->AcceptIncoming(msg));
		bool was_processed(false);
		if(val == CIRCContext::ARV_PROCESSED) {
			gui_->Update();
			return;
		}
		if(val == CIRCContext::ARV_PROCESSED_BUT_CONTINUE) was_processed = true;

		for(context_set::const_iterator i = ctxts_.begin(); i != ctxts_.end(); i++) {
			if(*i == act_) continue;

			val = (*i)->AcceptIncoming(msg);
			if(val == CIRCContext::ARV_PROCESSED) {
				gui_->Update();
				return;
			}
			if(val == CIRCContext::ARV_PROCESSED_BUT_CONTINUE) was_processed = true;
		}

		gui_->Update();
		if(!was_processed) Warning("Received unprocessed message!!! | %s\r\n", msg.GetString().c_str());
	}

	void CIRCEnvironment::Send(const IRCMessage& msg) const
	{
//		Msg("Sending: %s\r\n", msg.GetString(false).c_str());
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

	CIRCEnvironment::context_set* CIRCEnvironment::GetContexts() {
		return &ctxts_;
	}

	void CIRCEnvironment::SetActiveContext(SteamIRC::CIRCContext *con) {
		act_ = con;
	}

	SteamIRC::CIRCContext* CIRCEnvironment::GetActiveContext() {
		return act_;
	}

	void CIRCEnvironment::UserInput(const std::string& txt) {
		act_->UserInput(txt);
		gui_->Update();
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
		act_ = NULL;
	}

	void CIRCEnvironment::SetUInfo(IRCUserInfo& uInfo) {
		uInfo_ = uInfo;
	}

	IRCUserInfo* CIRCEnvironment::GetUInfo() {
		return &uInfo_;
	}

	bool CIRCEnvironment::ContextExists(std::string title) {
		for(context_set::const_iterator i = ctxts_.begin(); i != ctxts_.end(); i++)
			if((*i)->GetTitle() == title)
				return true;
		return false;
	}

	CIRCEnvironment::~CIRCEnvironment(void)
	{
		Cleanup();
	}
}
