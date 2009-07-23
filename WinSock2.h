#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdexcept>

#include "TCPClient.h"

namespace SteamIRC
{
	namespace WinSock2
	{
		class CWinSock2
		{
		public:
			CWinSock2(void) throw(std::runtime_error);
			CTCPClient* MakeTCPClient();
			virtual ~CWinSock2(void);
		private:
			WSADATA wsaData;
		};

	}
}
