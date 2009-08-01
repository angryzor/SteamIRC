#pragma once
//namespace SteamIRC
//{
//	class CIRCClient;
//}

#include "TCPClient.h"
#include "IRCDataStructures.h"
#include "IRCMessage.h"
#include "IRCEnvironment.h"
#include <vector>


namespace SteamIRC
{
	using namespace WinSock2;
	class CIRCWinSock2;

	class CIRCClient :
		public CTCPClient
	{
		friend CIRCWinSock2;
	public:
		virtual void Connect(std::string hosturi, std::string port, IRCUserInfo& uInfo, std::string pass = "");
		virtual void DoRecv();
		virtual void Send(const IRCMessage& msg);
		virtual void Disconnect(void);
		virtual ~CIRCClient(void);
		CIRCClient& operator<<(const IRCMessage& msg) {
	private:
		CIRCClient& operator=(CIRCClient&);
		virtual std::vector<std::string> TransformToVector(std::string rStr);
		std::string leftOverCmnd;
		CRITICAL_SECTION csSend;
		CIRCEnvironment& env_;
	protected:
		CIRCClient(CIRCEnvironment& env);
	};
}


