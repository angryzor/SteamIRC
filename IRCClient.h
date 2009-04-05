#pragma once
//namespace SteamIRC
//{
//	class CIRCClient;
//}

#include "TCPClient.h"
#include "IRCDataStructures.h"
#include "IRCMessage.h"
#include "IRCEnvironment.h"


namespace SteamIRC
{
	using namespace WinSock2;
	class CIRCClient :
		public CTCPClient
	{
	public:
		CIRCClient(CIRCEnvironment& env);
		virtual void Connect(String hosturi, String port, IRCUserInfo& uInfo);
		virtual void DoRecv();
		virtual int TransformToArray(String rStr, String** outCmndArr);
		virtual void Send(IRCMessage& msg);
		virtual void Disconnect(void);
		virtual ~CIRCClient(void);
	private:
		String leftOverCmnd;
		bool wasLeftOver;
		CRITICAL_SECTION csSend;
		CIRCEnvironment& env_;
	};



// UNBEAUTIFUL CODE ___ SHOULD BE REWRITTEN WHEN IN THE MOOD
	int StringSplit(String& str, String** arr, String& splitchar);
}


