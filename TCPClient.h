#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdexcept>

namespace SteamIRC
{
	namespace WinSock2
	{
		class CWinSock2;

		class CTCPClient
		{
			friend CWinSock2;
		public:
			virtual void Connect(std::string hosturi, std::string port) throw(std::logic_error, std::runtime_error);
			virtual void Send(const std::string& data) throw(std::logic_error, std::runtime_error);
			virtual std::string Recv(void) throw(std::logic_error, std::runtime_error);
			virtual bool CheckRecv() throw(std::logic_error, std::runtime_error);
			virtual void Disconnect(void) throw(std::logic_error);
			virtual ~CTCPClient(void);
			CTCPClient& operator<<(const std::string& str);
		private:
			CTCPClient& operator=(CTCPClient&);
			SOCKET sock;
			addrinfo infoHints;
		protected:
			CTCPClient(void);
		};
	}
}
