#include <sstream>
#include "IRCMessage.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

namespace SteamIRC
{
	IRCMessage::IRCMessage(void) : Origin("unknown")
	{
	}
		
	IRCMessage::IRCMessage(std::string& msgStr) : Origin("unknown")
	{
		ProcessString(msgStr);
	}

	IRCMessage::IRCMessage(Command cmd) : Origin("unknown")
	{
		Cmnd = cmd;
	}
	void IRCMessage::ProcessString(const std::string& msgStr)
	{
		// A plain text message string is converted to and saved into this IRCMessage here
		
		// I'll be using an istringstream here, they're fun to parse simple stuff with :)
		std::istringstream iss;
		
		iss.str(msgStr);

		std::string tmp;
		std::string tmp2;
		unsigned short numeralCmd;

		// First, check if we have a prefix.
		if( msgStr[0] == ':' ) // yes, we have one
		{
			iss >> tmp; // stream it into tmp
			Origin = IRCOrigin(tmp.substr(1)); // and cut off the colon
		}
		else
			Origin = IRCOrigin("unknown");
		// There, we set the origin. Now for the actual command

		// This is a very easy way of determining if the next part is a numeral command or a textual one.
		// This will fail if it isn't.
		iss >> std::dec >> numeralCmd;
		if(!iss) // if it failed
		{
			// clear the stream's state
			iss.clear();

			iss >> tmp;

			Cmnd = LookupStringToCommand(tmp);
		} 
		else 
		{
			Cmnd = static_cast<Command>(numeralCmd);
		}

		// OK, we got the command. This is the last and hardest part,
		// the parameters
		while(!iss.eof())
		{
			if(!(iss >> tmp2)) break;

			if(tmp2[0] == ':') // This is a very special first character. It means that everything behind it,
			{							  // including spaces, is part of this parameter.
				tmp2 = tmp2.substr(1);
				tmp2.append(" ");
				
				while(!iss.eof())
				{
					if(!(iss >> tmp)) break;
					tmp2.append(tmp);
					tmp2.append(" ");
				}
				tmp2 = tmp2.substr(0, tmp2.length() - 1);
			}
			AddParam(tmp2);
		}
	}

	std::string IRCMessage::GetString(bool includeOrigin) const
	{
		std::string tmp;
		std::ostringstream oss;
		if(includeOrigin) oss << Origin.toStr() + " ";
		oss << LookupCommandToString(Cmnd);

		if(Parameters.size() > 0)
		{
			for(std::vector<std::string>::size_type i = 0; i < Parameters.size() - 1; i++)
			{
				oss << ' ' << Parameters[i];
			}
			oss << " :" << Parameters[Parameters.size() - 1];
		}

		oss << "\r\n";

		tmp = oss.str();
		return tmp;
	}

	void IRCMessage::AddParam(std::string val) {
		Parameters.insert(Parameters.end(), val);
	}

	void IRCMessage::AddParam(int val) {
		std::ostringstream oss;
		oss << val;
		Parameters.insert(Parameters.end(), oss.str());
	}

	void IRCMessage::SetParam(int index, std::string val)
	{
		Parameters[index] = val;
	}

	void IRCMessage::SetParam(int index, int val)
	{
		std::ostringstream oss;
		oss << val;
		Parameters[index] = oss.str();
	}

	Command IRCMessage::LookupStringToCommand(const std::string& str) const 
	{
//==========================================================
// D= D= D= D=     D= D= D= D=
// OH NOES!!! TIZ CODE IS TEH UGLYZ!!! TIS CLEARLY POOR DESIGN!!!
// REPAIR ASAP!!!
//==========================================================
		if(str.compare("PASS") == 0) return PASS;
		if(str.compare("NICK") == 0) return NICK;
		if(str.compare("USER") == 0) return USER;
		if(str.compare("OPER") == 0) return OPER;
		if(str.compare("MODE") == 0) return MODE;
		if(str.compare("SERVICE") == 0) return SERVICE;
		if(str.compare("QUIT") == 0) return QUIT;
		if(str.compare("SQUIT") == 0) return SQUIT;
		if(str.compare("JOIN") == 0) return JOIN;
		if(str.compare("PART") == 0) return PART;
		if(str.compare("TOPIC") == 0) return TOPIC;
		if(str.compare("NAMES") == 0) return NAMES;
		if(str.compare("LIST") == 0) return LIST;
		if(str.compare("INVITE") == 0) return INVITE;
		if(str.compare("KICK") == 0) return KICK;
		if(str.compare("PRIVMSG") == 0) return PRIVMSG;
		if(str.compare("NOTICE") == 0) return NOTICE;
		if(str.compare("MOTD") == 0) return MOTD;
		if(str.compare("LUSERS") == 0) return LUSERS;
		if(str.compare("VERSION") == 0) return VERSION;
		if(str.compare("STATS") == 0) return STATS;
		if(str.compare("LINKS") == 0) return LINKS;
		if(str.compare("TIME") == 0) return TIME;
		if(str.compare("CONNECT") == 0) return CONNECT;
		if(str.compare("TRACE") == 0) return TRACE;
		if(str.compare("ADMIN") == 0) return ADMIN;
		if(str.compare("INFO") == 0) return INFO;
		if(str.compare("SQUERY") == 0) return SQUERY;
		if(str.compare("WHO") == 0) return WHO;
		if(str.compare("WHOIS") == 0) return WHOIS;
		if(str.compare("WHOWAS") == 0) return WHOWAS;
		if(str.compare("KILL") == 0) return KILL;
		if(str.compare("PING") == 0) return PING;
		if(str.compare("PONG") == 0) return PONG;
		if(str.compare("ERROR") == 0) return ERROR_;
		if(str.compare("AWAY") == 0) return AWAY;
		if(str.compare("REHASH") == 0) return REHASH;
		if(str.compare("DIE") == 0) return DIE;
		if(str.compare("RESTART") == 0) return RESTART;
		if(str.compare("SUMMON") == 0) return SUMMON;
		if(str.compare("USERS") == 0) return USERS;
		if(str.compare("WALLOPS") == 0) return WALLOPS;
		if(str.compare("USERHOST") == 0) return USERHOST;
		if(str.compare("ISON") == 0) return ISON;
		std::istringstream iss;
		unsigned short tmp = 0;
		iss >> tmp;
		return static_cast<Command>(tmp);
	}

	std::string IRCMessage::LookupCommandToString(Command cmd) const
	{
		std::ostringstream oss;
		switch(cmd)
		{
			case PASS: return "PASS";
			case NICK: return "NICK";
			case USER: return "USER";
			case OPER: return "OPER";
			case MODE: return "MODE";
			case SERVICE: return "SERVICE";
			case QUIT: return "QUIT";
			case SQUIT: return "SQUIT";
			case JOIN: return "JOIN";
			case PART: return "PART";
			case TOPIC: return "TOPIC";
			case NAMES: return "NAMES";
			case LIST: return "LIST";
			case INVITE: return "INVITE";
			case KICK: return "KICK";
			case PRIVMSG: return "PRIVMSG";
			case NOTICE: return "NOTICE";
			case MOTD: return "MOTD";
			case LUSERS: return "LUSERS";
			case VERSION: return "VERSION";
			case STATS: return "STATS";
			case LINKS: return "LINKS";
			case TIME: return "TIME";
			case CONNECT: return "CONNECT";
			case TRACE: return "TRACE";
			case ADMIN: return "ADMIN";
			case INFO: return "INFO";
			case SQUERY: return "SQUERY";
			case WHO: return "WHO";
			case WHOIS: return "WHOIS";
			case WHOWAS: return "WHOWAS";
			case KILL: return "KILL";
			case PING: return "PING";
			case PONG: return "PONG";
			case ERROR_: return "ERROR";
			case AWAY: return "AWAY";
			case REHASH: return "REHASH";
			case DIE: return "DIE";
			case RESTART: return "RESTART";
			case SUMMON: return "SUMMON";
			case USERS: return "USERS";
			case WALLOPS: return "WALLOPS";
			case USERHOST: return "USERHOST";
			case ISON: return "ISON";
			default:
				oss << static_cast<unsigned short>(cmd);
				return oss.str();
		}
	}

	void IRCMessage::SetCommand(Command cmd)
	{
		Cmnd = cmd;
	}

	void IRCMessage::Reset()
	{
		Cmnd = NICK;
		Parameters.clear();
	}

	IRCMessage::~IRCMessage(void)
	{
	}
}
