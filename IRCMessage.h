#pragma once

#include <string>
#include <vector>
#include "commandparse.h"

namespace SteamIRC {
	struct IRCOrigin {
		IRCOrigin(const std::string& str) {
			std::string::size_type iat(str.find("@"));
			if(iat == str.npos) {
				addr = str;
				return;
			}
			else addr = str.substr(iat + 1);

			std::string::size_type iexcl = str.find("!");
			if(iexcl == str.npos)
				user = str.substr(0,iat);
			else {
				nick = str.substr(0,iexcl);
				user = str.substr(iexcl+1, iat-iexcl-1);
			}
		}
		std::string toStr() const{
			return nick + std::string("!") + user + std::string("@") + addr;
		}
		std::string nick;
		std::string user;
		std::string addr;
	};

	class IRCMessage
	{
	public:
		typedef std::vector<std::string> param_vec;
		IRCMessage(void);
		IRCMessage(std::string& msgStr);
		IRCMessage(Command cmd);
		void ProcessString(const std::string& msgStr);
		std::string GetString(bool includeOrigin = true) const;
		virtual ~IRCMessage(void);
		void SetCommand(Command cmd);
		void SetParam(int index, std::string val);
		void SetParam(int index, int val);
		void AddParam(std::string val);
		void AddParam(int val);
		void Reset();

	protected:
		virtual Command LookupStringToCommand(const std::string& str) const;
		virtual std::string LookupCommandToString(Command cmd) const;
	public:
		IRCOrigin Origin;
		param_vec Parameters; // a message can have a maximum of 15 params
		Command Cmnd;
	private:
		static CommandParse command_parser_;
	};
}
