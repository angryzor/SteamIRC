#pragma once

#include "IRCCommunicator.h"
#include "IRCChannelUser.h"
#include <set>

namespace SteamIRC
{
	class CIRCChannel :
		public CIRCCommunicator
	{
	public:
		typedef std::set<IRCChannelUser> chanfolk_set;
		CIRCChannel(CIRCEnvironment& env, std::string name);
		virtual ~CIRCChannel(void);
//		virtual bool UserInput(std::string txt);
		virtual AcceptReturnValue AcceptIncoming(const IRCMessage& msg);
		void Part(std::istream& params);
		chanfolk_set* GetChanFolk();
		virtual bool ProcessUserCommand(const std::string& cmnd, std::istringstream& params);
		void EraseNick(std::string nick);
		chanfolk_set::iterator FindByNick(std::string nick);
		void DestroyChannel();
	private:
		chanfolk_set chanfolk_;
		bool sendingNames_;
	};
}
