#include "eiface.h"
#include "IRCClient.h"
#include "IRCNetwork.h"
#include <xstring>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

namespace SteamIRC
{
	using namespace WinSock2;
	CIRCClient::CIRCClient(CIRCEnvironment& env) : env_(env)
	{
		InitializeCriticalSection(&csSend);
		env_.SetConnection(this);
	}

	void CIRCClient::Connect(std::string hosturi, std::string port, IRCUserInfo& uInfo)
	{
		// Connect to the IRC server on hosturi:port
		CTCPClient::Connect(hosturi, port);

		// Send the login data
		IRCMessage msg(NICK);
		msg.AddParam(uInfo.Nick);
		Send(msg);

		msg.Reset();
		msg.SetCommand(USER);
		msg.AddParam(uInfo.UserName);
		msg.AddParam(uInfo.autoModeBM);
		msg.AddParam("*");
		msg.AddParam(uInfo.RealName);
		Send(msg);
		
		env_.SetUInfo(uInfo);
		env_.Add(new CIRCNetwork(hosturi, env_, uInfo));
	}

//=====================================================================
//  THREAD BOUNDARY
// /-----------------------------------------------------------------\ 
	void CIRCClient::DoRecv()
	{
		std::string rStr = CTCPClient::Recv(); // This will block until info is received
		vector<std::string> cmnds;
		vector<IRCMessage> msgs;
		
		cmnds = TransformToVector(rStr);


		for(vector<std::string>::const_iterator i = cmnds.begin(); i != cmnds.end(); i++) {
			vector<IRCMessage>::iterator new_pos = msgs.insert(msgs.end(), IRCMessage());
			new_pos->ProcessString(*i);
		}

		for(vector<IRCMessage>::const_iterator i = msgs.begin(); i != msgs.end(); i++) {
			Msg("RAW: %s\r\n", i->GetString().c_str());
			env_.ProcessReceived(*i);
		}
	}
// \-----------------------------------------------------------------/
//  /THREAD BOUNDARY
//=====================================================================

	vector<std::string> CIRCClient::TransformToVector(std::string rStr)
	{
		vector<std::string> res;
		// Save last command if not finished
		rStr = leftOverCmnd + rStr;

		{
			std::string::size_type posfound = 0;
			while((posfound = rStr.find("\r\n", posfound)) != rStr.npos)
				rStr.replace(posfound, 2, "\n");
			while((posfound = rStr.find("\r", posfound)) != rStr.npos)
				rStr.replace(posfound, 1, ""); // CRs for some reason bug the stringstream
		}

		{
			std::istringstream iss(rStr, istringstream::in);
			std::string line;
			while(!(std::getline(iss, line).eof())){
				Msg("NOW INSERTING: %s\r\n", line.c_str());
				res.insert(res.end(), line);
			}
			leftOverCmnd = line;
		}

		return res;
	}

	// I'm not sure if this really needs a critical section, but I'm putting one for certainty.
	void CIRCClient::Send(const IRCMessage& msg)
	{
		std::string tmp = msg.GetString(false);
		EnterCriticalSection(&csSend);
		CTCPClient::Send(tmp);
		LeaveCriticalSection(&csSend);
	}

	void CIRCClient::Disconnect(void)
	{
		CTCPClient::Send(std::string("QUIT\r\n"));
		CTCPClient::Disconnect();
	}
		
	CIRCClient::~CIRCClient(void)
	{
		DeleteCriticalSection(&csSend);
	}
}

