#include "tier2/tier2.h"
#include "TCPClient.h"

#define TCP_RECV_BUF_LEN 512

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

namespace SteamIRC
{
	namespace WinSock2
	{
		CTCPClient::CTCPClient() : sock(INVALID_SOCKET)
		{
			ZeroMemory(&infoHints, sizeof(infoHints));
			infoHints.ai_family = AF_UNSPEC;
			infoHints.ai_socktype = SOCK_STREAM;
			infoHints.ai_protocol = IPPROTO_TCP;
		}

		void CTCPClient::Connect(String hosturi, String port) throw(std::runtime_error)
		{
			if(sock != INVALID_SOCKET) throw std::logic_error(_T("Connection already established!"));
			addrinfo* results;
			addrinfo* cur;
			int res;

			try {
				// Resolve host name
				res = getaddrinfo(hosturi, port, &infoHints, &results);
				switch(res)
				{
				case EAI_AGAIN:
					throw std::runtime_error(_T("Host can temporarily not be resolved. Try again later."));
				case EAI_FAIL:
					throw std::runtime_error(_T("Failed to resolve host address."));
				case EAI_NONAME:
					throw std::runtime_error(_T("Host not found."));
				}
			}
			catch(std::runtime_error err) {
				freeaddrinfo(results);
				Warning(err.what());
				throw std::runtime_error("Connection failed.");
			}

			try {

				cur = results;

				while(sock == INVALID_SOCKET && cur)
				{
					// Create socket
					sock = socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
					if(sock == INVALID_SOCKET) throw std::runtime_error(_T("Socket creation failed."));

					// Connect
					res = connect( sock, cur->ai_addr, (int)cur->ai_addrlen);
					if (res == SOCKET_ERROR)
					{
						closesocket(sock);
						sock = INVALID_SOCKET;
						cur = cur->ai_next;
					}
				}

				freeaddrinfo(results);
				if(sock == INVALID_SOCKET) throw std::runtime_error(_T("Could not connect to host."));
			}
			catch(std::runtime_error err) {
				Warning(err.what());
				throw std::runtime_error("Connection failed.");
			}
		}

		void CTCPClient::Send(String& data)
		{
			try
			{
				int res;

				if(sock == INVALID_SOCKET) throw std::logic_error(_T("Invalid socket."));

				res = send(sock, data, data.length(), NULL);
				if(res == SOCKET_ERROR)
				{
					throw std::logic_error(_T("Can't send data"));
				}
			}
			catch(std::logic_error err) {
				Warning(String(_T("Sending error: ")) + err.what());
			}
		}

		String CTCPClient::Recv()
		{
			try
			{
				int res;
				char buf[TCP_RECV_BUF_LEN];
				ZeroMemory(buf, sizeof(buf));

				if(sock == INVALID_SOCKET) throw std::logic_error(_T("Invalid socket."));

				res = recv(sock, buf, TCP_RECV_BUF_LEN, NULL);
				if(res == SOCKET_ERROR)
				{
					throw std::logic_error(_T("Can't recieve data"));
				}
				return buf;
			}
			catch(std::logic_error err) {
				Warning(String(_T("Receiving error: ")) + err.what());
				return "";
			}
		}

		bool CTCPClient::CheckRecv()
		{
			try
			{
				int res;
				unsigned long size;

				if(sock == INVALID_SOCKET) throw std::logic_error(_T("Invalid socket."));

				res = ioctlsocket(sock, FIONREAD, &size);
				if(res == SOCKET_ERROR)
					throw std::logic_error(_T("Can't recieve data"));

				if(size <= 0)
					return false;

				return true;
			}
			catch(std::logic_error err) {
				Warning(String(_T("Receiving error: ")) + err.what());
				return false;
			}
		}

		void CTCPClient::Disconnect() throw(std::logic_error)
		{
			if(sock == INVALID_SOCKET) throw std::logic_error(_T("Trying to disconnect disconnected socket."));
			closesocket(sock);
			sock = INVALID_SOCKET;
		}

		CTCPClient::~CTCPClient(void)
		{
			if(sock != INVALID_SOCKET) Disconnect();
		}
	}
}