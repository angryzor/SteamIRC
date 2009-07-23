#include "CommandParse.h"

namespace SteamIRC {

	CommandParse::CommandParse(void)
	{
		add(PASS, "PASS");
		add(NICK, "NICK");
		add(USER, "USER");
		add(OPER, "OPER");
		add(MODE, "MODE");
		add(SERVICE, "SERVICE");
		add(QUIT, "QUIT");
		add(SQUIT, "SQUIT");
		add(JOIN, "JOIN");
		add(PART, "PART");
		add(TOPIC, "TOPIC");
		add(NAMES, "NAMES");
		add(LIST, "LIST");
		add(INVITE, "INVITE");
		add(KICK, "KICK");
		add(PRIVMSG, "PRIVMSG");
		add(NOTICE, "NOTICE");
		add(MOTD, "MOTD");
		add(LUSERS, "LUSERS");
		add(VERSION, "VERSION");
		add(STATS, "STATS");
		add(LINKS, "LINKS");
		add(TIME, "TIME");
		add(CONNECT, "CONNECT");
		add(TRACE, "TRACE");
		add(ADMIN, "ADMIN");
		add(INFO, "INFO");
		add(SQUERY, "SQUERY");
		add(WHO, "WHO");
		add(WHOIS, "WHOIS");
		add(WHOWAS, "WHOWAS");
		add(KILL, "KILL");
		add(PING, "PING");
		add(PONG, "PONG");
		add(ERROR_, "ERROR");
		add(AWAY, "AWAY");
		add(REHASH, "REHASH");
		add(DIE, "DIE");
		add(RESTART, "RESTART");
		add(SUMMON, "SUMMON");
		add(USERS, "USERS");
		add(WALLOPS, "WALLOPS");
		add(USERHOST, "USERHOST");
		add(ISON, "ISON");
	}

	CommandParse::~CommandParse(void)
	{
	}
}
