#include <sstream>
#include "IRCMessage.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

namespace SteamIRC
{
	IRCMessage::IRCMessage(void) : numParams(0)
	{
	}
		
	IRCMessage::IRCMessage(String& msgStr) : numParams(0)
	{
		ProcessString(msgStr);
	}

	IRCMessage::IRCMessage(Command cmd) : numParams(0)
	{
		Cmnd = cmd;
	}
	void IRCMessage::ProcessString(String& msgStr)
	{
		// A plain text message string is converted to and saved into this IRCMessage here
		
		// I'll be using an istringstream here, they're fun to parse simple stuff with :)
		std::istringstream iss;
		
		iss.str(msgStr);

		String tmp;
		unsigned short numeralCmd;

		// First, check if we have a prefix.
		if( msgStr[0] == ':' ) // yes, we have one
		{
			iss >> tmp; // stream it into tmp
			Origin = tmp.substr(1); // and cut off the colon
		}
		else
			Origin = "unknown";
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
		unsigned char i = 0;
		while(!iss.eof())
		{
			iss >> Parameters[i];

			if((Parameters[i])[0] == ':') // This is a very special first character. It means that everything behind it,
			{							  // including spaces, is part of this parameter.
				Parameters[i] = Parameters[i].substr(1);
				Parameters[i].append(" ");
				
				if(!iss.eof())
				{
					while(!iss.eof())
					{
						iss >> tmp;
						Parameters[i].append(tmp);
						Parameters[i].append(" ");
					}
					Parameters[i] = Parameters[i].substr(0, Parameters[i].length() - 1);
				}
			}
			i++;
		}
		numParams = i;
	}

	String IRCMessage::GetString()
	{
		String tmp;
		std::ostringstream oss;
		oss << LookupCommandToString(Cmnd);

		if(numParams > 0)
		{
			for(int i = 0; i < numParams - 1; i++)
			{
				oss << ' ' << Parameters[i];
			}
			oss << " :" << Parameters[numParams - 1];
		}

		oss << "\r\n";

		tmp = oss.str();
		return tmp;
	}

	void IRCMessage::SetParam(unsigned char index, String val)
	{
		Parameters[index] = val;
		if(index >= numParams) numParams = index + 1;
	}

	void IRCMessage::SetParam(unsigned char index, int val)
	{
		std::ostringstream oss;
		oss << val;
		Parameters[index] = oss.str();
		if(index >= numParams) numParams = index + 1;
	}

	Command IRCMessage::LookupStringToCommand(String& str)
	{
//==========================================================
// D= D= D= D=     D= D= D= D=
// OH NOES!!! TIZ CODE IS TEH UGLYZ!!! TIS CLEARLY POOR DESIGN!!!
// REPAIR ASAP!!!
//==========================================================
		if(str.Compare("PASS") == 0) return PASS;
		if(str.Compare("NICK") == 0) return NICK;
		if(str.Compare("USER") == 0) return USER;
		if(str.Compare("OPER") == 0) return OPER;
		if(str.Compare("MODE") == 0) return MODE;
		if(str.Compare("SERVICE") == 0) return SERVICE;
		if(str.Compare("QUIT") == 0) return QUIT;
		if(str.Compare("SQUIT") == 0) return SQUIT;
		if(str.Compare("JOIN") == 0) return JOIN;
		if(str.Compare("PART") == 0) return PART;
		if(str.Compare("TOPIC") == 0) return TOPIC;
		if(str.Compare("NAMES") == 0) return NAMES;
		if(str.Compare("LIST") == 0) return LIST;
		if(str.Compare("INVITE") == 0) return INVITE;
		if(str.Compare("KICK") == 0) return KICK;
		if(str.Compare("PRIVMSG") == 0) return PRIVMSG;
		if(str.Compare("NOTICE") == 0) return NOTICE;
		if(str.Compare("MOTD") == 0) return MOTD;
		if(str.Compare("LUSERS") == 0) return LUSERS;
		if(str.Compare("VERSION") == 0) return VERSION;
		if(str.Compare("STATS") == 0) return STATS;
		if(str.Compare("LINKS") == 0) return LINKS;
		if(str.Compare("TIME") == 0) return TIME;
		if(str.Compare("CONNECT") == 0) return CONNECT;
		if(str.Compare("TRACE") == 0) return TRACE;
		if(str.Compare("ADMIN") == 0) return ADMIN;
		if(str.Compare("INFO") == 0) return INFO;
		if(str.Compare("SQUERY") == 0) return SQUERY;
		if(str.Compare("WHO") == 0) return WHO;
		if(str.Compare("WHOIS") == 0) return WHOIS;
		if(str.Compare("WHOWAS") == 0) return WHOWAS;
		if(str.Compare("KILL") == 0) return KILL;
		if(str.Compare("PING") == 0) return PING;
		if(str.Compare("PONG") == 0) return PONG;
		if(str.Compare("ERROR") == 0) return ERROR_;
		if(str.Compare("AWAY") == 0) return AWAY;
		if(str.Compare("REHASH") == 0) return REHASH;
		if(str.Compare("DIE") == 0) return DIE;
		if(str.Compare("RESTART") == 0) return RESTART;
		if(str.Compare("SUMMON") == 0) return SUMMON;
		if(str.Compare("USERS") == 0) return USERS;
		if(str.Compare("WALLOPS") == 0) return WALLOPS;
		if(str.Compare("USERHOST") == 0) return USERHOST;
		if(str.Compare("ISON") == 0) return ISON;
		std::istringstream iss;
		unsigned short tmp = 0;
		iss >> tmp;
		return static_cast<Command>(tmp);
	}

	String IRCMessage::LookupCommandToString(Command cmd)
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
		numParams = 0;
	}

	IRCMessage::~IRCMessage(void)
	{
	}
}
