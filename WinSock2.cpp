#include "WinSock2.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

namespace SteamIRC
{
	namespace WinSock2
	{
		CWinSock2::CWinSock2(void) throw(std::runtime_error)
		{
			int res;
			
			res = WSAStartup( MAKEWORD(2,2), &wsaData );

			if(res)
				throw std::runtime_error("WinSock 2.2 failed to initialize!");
		}

		CTCPClient* CWinSock2::MakeTCPClient()
		{
			return new CTCPClient();
		}

		CWinSock2::~CWinSock2(void)
		{
			WSACleanup();
		}
	}
}
