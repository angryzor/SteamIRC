#include "IRCClient.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

namespace SteamIRC
{
	using namespace WinSock2;
	CIRCClient::CIRCClient(void) : wasLeftOver(false)
	{
		ircIO = new CIRCMessageIO(this);
		InitializeCriticalSection(&csSend);
	}

	void CIRCClient::Connect(String hosturi, String port, IRCUserInfo& uInfo)
	{
		// Connect to the IRC server on hosturi:port
		CTCPClient::Connect(hosturi, port);

		// Send the login data
		IRCMessage msg(NICK);
		msg.SetParam(0, uInfo.Nick);
		Send(msg);

		msg.Reset();
		msg.SetCommand(USER);
		msg.SetParam(0, uInfo.UserName);
		msg.SetParam(1, uInfo.autoModeBM);
		msg.SetParam(2, "*");
		msg.SetParam(3, uInfo.RealName);
		Send(msg);
	}

//=====================================================================
//  THREAD BOUNDARY
// /-----------------------------------------------------------------\ 
	void CIRCClient::DoRecv()
	{
		String rStr = CTCPClient::Recv(); // This will block until info is received
		String* cmndArray;
		IRCMessage* msgArray;
		int numCmnds;
		
		numCmnds = TransformToArray(rStr, &cmndArray);

		msgArray = new IRCMessage [numCmnds];

		for(int i = 0; i < numCmnds; i++)
			msgArray[i].ProcessString(cmndArray[i]);
		for(int i = 0; i < numCmnds; i++)
			ircIO->ProcessReceived(msgArray[i]);

		delete [] msgArray;
		delete [] cmndArray;
	}
// \-----------------------------------------------------------------/
//  /THREAD BOUNDARY
//=====================================================================

	int CIRCClient::TransformToArray(String rStr, String** outCmndArr)
	{
		// Save last command if not finished

		if(wasLeftOver)
		{
			rStr = leftOverCmnd + rStr;
			wasLeftOver = false;
		}

		int strl = rStr.length();
		int indxLastCRLF = rStr.ReverseFind("\r\n");

		if(indxLastCRLF < (strl - 2)) // Not finished
		{
			leftOverCmnd = rStr.substr(indxLastCRLF+2);
			wasLeftOver = true;
		}
		
		rStr = rStr.substr(0, indxLastCRLF);

		return StringSplit(rStr, outCmndArr, String("\r\n"));
	}

	// I'm not sure if this really needs a critical section, but I'm putting one for certainty.
	void CIRCClient::Send(IRCMessage& msg)
	{
		String tmp = msg.GetString();
		EnterCriticalSection(&csSend);
		CTCPClient::Send(tmp);
		LeaveCriticalSection(&csSend);
	}

	void CIRCClient::Disconnect(void)
	{
		CTCPClient::Send(String("QUIT\r\n"));
		CTCPClient::Disconnect();
	}
		
	CIRCClient::~CIRCClient(void)
	{
		DeleteCriticalSection(&csSend);
		delete ircIO;
		ircIO = NULL;
	}

		// DON'T FORGET TO DELETE THE STRING ARRAY AFTERWARDS
	int StringSplit(String& str, String** arr, String& splitchar)
	{
		int idx = 0;
		int count = 0;
		int splclen = splitchar.length();
		int prevIdx = 0;

		// 2-pass
		while((idx = str.Find(splitchar, idx + splclen)) != -1)
		{
			count++;
		}

		*arr = new String [count+1];

		idx = -splclen;
		for(int i = 0; i < count; i++)
		{
			prevIdx = idx;
			idx = str.Find(splitchar, idx + splclen);

			(*arr)[i] = str.substr(prevIdx + splclen, idx - prevIdx - splclen);
		}
		(*arr)[count] = str.substr(idx + splclen, str.length() - idx - splclen);

		return count+1;
	}
}

