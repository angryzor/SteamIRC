#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdexcept>

#include "Object.h"
#include "WS2Exception.h"

namespace SteamIRC
{
	namespace WinSock2
	{
		class CTCPClient : public Object
		{
		public:
			CTCPClient(void);
			virtual void Connect(String hosturi, String port) throw(std::runtime_error);
			virtual void Send(const String& data);
			virtual String Recv(void);
			virtual bool CheckRecv();
			virtual void Disconnect(void) throw(std::logic_error);
			virtual ~CTCPClient(void);
		private:
			SOCKET sock;
			addrinfo infoHints;
		};
	}
}
