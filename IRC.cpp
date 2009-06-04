#include "IRC.h"

#define RECV_THREAD_SLEEP_MS 500

#include "tier0/memdbgon.h"

namespace SteamIRC {

	DWORD WINAPI recvthread(void* lpThreadParameter)
	{
		IRC* irc = static_cast<IRC*>(lpThreadParameter);
		while(irc->runRecv)
		{
			if(irc->ircClient_->CheckRecv())
				irc->ircClient_->DoRecv();
			else
				Sleep(RECV_THREAD_SLEEP_MS);
		}
		return EXIT_SUCCESS;
	}

	IRC::IRC(	CreateInterfaceFn interfaceFactory) : ircClient_(NULL), gui_(NULL), tRecv_(NULL), runRecv(false)
	{
		Msg(" Starting GUI system...\r\n");
		gui_ = new IRCGui(interfaceFactory, ircEnv_); 
		ircEnv_.SetGui(gui_);

		Msg(" Booting networking system...\r\n");
		ircClient_ = sockEngine_.MakeIRCClient(ircEnv_);
	}

	void IRC::Connect(std::string hosturi, std::string port, IRCUserInfo& uInfo) {
		try {
			// Connect our client
			ircClient_->Connect(hosturi, port, uInfo);

			// Create the receive thread
			runRecv = true;
			tRecv_ = CreateThread(NULL, 0, &recvthread, static_cast<void*>(this), 0, NULL);
		}
		catch(std::runtime_error err)
		{
			Warning(err.what());
		}
	}

	void IRC::Disconnect() {
		if(runRecv) {
			// Alert the recvthread that it should stop processing messages and wait for it to finish
			runRecv = false;
			WaitForSingleObject(tRecv_,INFINITE);

			ircClient_->Disconnect();
			ircEnv_.Cleanup();
		}
	}

	void IRC::ShowGUI() {
		try {
			gui_->CreatePanel();
			gui_->Update();
		}
		catch(std::logic_error err) {
			Warning(err.what());
			return;
		}
	}

	void IRC::HideGUI() {
		gui_->DestroyPanel();
	}

	IRC::~IRC(void)
	{
		Disconnect();

		Msg("irc");

		if(gui_){
			gui_->DestroyPanel();
			delete gui_;
		}
		if(ircClient_){
			delete ircClient_;
		}
		Msg("irc2");
	}

}
