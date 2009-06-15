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
		try {
			Msg(" Starting GUI system...\r\n");
			gui_ = new IRCGui(interfaceFactory, ircEnv_); 
			ircEnv_.SetGui(gui_);

			Msg(" Booting networking system...\r\n");
			ircClient_ = sockEngine_.MakeIRCClient(ircEnv_);
		}
		catch(std::runtime_error err)
		{
			Warning(err.what());
			throw std::runtime_error("Boot failed!");
		}
		catch(std::logic_error err)
		{
			Warning(err.what());
			throw std::runtime_error("Boot failed!");
		}
	}

	void IRC::Connect(std::string hosturi, std::string port, IRCUserInfo& uInfo, std::string pass) {
		try {
			// Connect our client
			ircClient_->Connect(hosturi, port, uInfo, pass);

			// Create the receive thread
			runRecv = true;
			tRecv_ = CreateThread(NULL, 0, &recvthread, static_cast<void*>(this), 0, NULL);
		}
		catch(std::runtime_error err)
		{
			Warning(err.what());
		}
		catch(std::logic_error err)
		{
			Warning(err.what());
		}
	}

	void IRC::Disconnect() {
		try {
			if(runRecv) {
				// Alert the recvthread that it should stop processing messages and wait for it to finish
				runRecv = false;
				WaitForSingleObject(tRecv_,INFINITE);

				ircClient_->Disconnect();
				ircEnv_.Cleanup();
				gui_->Update();
			}
		}
		catch(std::runtime_error err) {
			Warning(err.what());
			return;
		}
		catch(std::logic_error err) {
			Warning(err.what());
			return;
		}
	}

	void IRC::ShowGUI() {
		try {
			gui_->CreatePanel();
			gui_->Update();
		}
		catch(std::runtime_error err) {
			Warning(err.what());
			return;
		}
		catch(std::logic_error err) {
			Warning(err.what());
			return;
		}
	}

	void IRC::HideGUI() {
		try {
			gui_->DestroyPanel();
		}
		catch(std::runtime_error err) {
			Warning(err.what());
			return;
		}
		catch(std::logic_error err) {
			Warning(err.what());
			return;
		}
	}

	IRC::~IRC(void)
	{
		Disconnect();

		if(gui_){
			gui_->DestroyPanel();
			delete gui_;
		}
		if(ircClient_){
			delete ircClient_;
		}
	}

}
