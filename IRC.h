#pragma once

#include "IRCDataStructures.h"
#include "IRCWinSock2.h"
#include "IRCClient.h"
#include "IRCEnvironment.h"
#include "IRCGui.h"
#include "eiface.h"
#include <string>

namespace SteamIRC {

	class IRC
	{
		friend DWORD WINAPI recvthread(void* lpThreadParameter);
	public:
		IRC(	CreateInterfaceFn interfaceFactory);
		void Connect(std::string hosturi, std::string port, IRCUserInfo& uInfo);
		void ShowGUI();
		void HideGUI();
		void Disconnect();
		virtual ~IRC(void);
	private:
		CIRCWinSock2 sockEngine_;
		CIRCClient* ircClient_;
		CIRCEnvironment ircEnv_;
		IRCGui* gui_;
		HANDLE tRecv_;
		bool runRecv;
	};

}
