#pragma once

#include <string>
#include <vector>

namespace SteamIRC
{
	enum Command
	{
		// Connection registration
		PASS = 600, NICK = 601, USER = 602, OPER = 603,	MODE = 604, SERVICE = 605, QUIT = 606, SQUIT = 607,
		// Channel operations
		JOIN = 608, PART = 609, TOPIC = 610, NAMES = 611, LIST = 612, INVITE = 613, KICK = 614,
		// Sending messages
		PRIVMSG = 615, NOTICE = 616, 
		// Server queries and commands
		MOTD = 617, LUSERS = 618, VERSION = 619, STATS = 620, LINKS = 621, TIME = 622, CONNECT = 623, TRACE = 624,
		ADMIN = 625, INFO = 626, SQUERY = 627, WHO = 628, WHOIS = 629, WHOWAS = 630, KILL = 631, PING = 632, PONG = 633,
		ERROR_ = 634,
		// Optional features
		AWAY = 635, REHASH = 636, DIE = 637, RESTART = 638, SUMMON = 639, USERS = 640, WALLOPS = 641,
		USERHOST = 642, ISON = 643, 
		// NUMERIC REPLIES
		// Command responses
		RPL_WELCOME = 1, RPL_YOURHOST = 2, RPL_CREATED = 3, RPL_MYINFO = 4, RPL_ISUPPORT = 5, // RPL_BOUNCE = 5, // sometime, somehow, for some shady reason, people started using num 5 as a list of supported features instead of a redirect

		RPL_USERHOST = 302, RPL_ISON = 303, RPL_AWAY = 301, RPL_UNAWAY = 305, RPL_NOWAWAY = 306,
		RPL_WHOISUSER = 311, RPL_WHOISSERVER = 312, RPL_WHOISOPERATOR = 313, RPL_WHOISIDLE = 317,
		RPL_ENDOFWHOIS = 318, RPL_WHOISCHANNELS = 319, RPL_WHOWASUSER = 314, RPL_ENDOFWHOWAS = 369,
		RPL_LISTSTART = 321, RPL_LIST = 322, RPL_LISTEND = 323, RPL_UNIQOPIS = 325, RPL_CHANNELMODEIS = 324,
		RPL_NOTOPIC = 331, RPL_TOPIC = 332, RPL_TOPICTIMESTAMP = 333, RPL_INVITING = 341, RPL_SUMMONING = 342, RPL_INVITELIST = 346,
		RPL_ENDOFINVITELIST = 347, RPL_EXCEPTLIST = 348, RPL_ENDOFEXCEPTLIST = 349, RPL_VERSION = 351,
		RPL_WHOREPLY = 352, RPL_ENDOFWHO = 315, RPL_NAMREPLY = 353, RPL_ENDOFNAMES = 366, RPL_LINKS = 364,
		RPL_ENDOFLINKS = 365, RPL_BANLIST = 367, RPL_ENDOFBANLIST = 368, RPL_INFO = 371, RPL_ENDOFINFO = 374,
		RPL_MOTDSTART = 375, RPL_MOTD = 372, RPL_ENDOFMOTD = 376, RPL_YOUREOPER = 381, RPL_REHASHING = 382,
		RPL_YOURESERVICE = 383, RPL_TIME = 391, RPL_USERSSTART = 392, RPL_USERS = 393, RPL_ENDOFUSERS = 394,
		RPL_NOUSERS = 395, 

		RPL_TRACELINK = 200, RPL_TRACECONNECTING = 201, RPL_TRACEHANDSHAKE = 202, RPL_TRACEUNKNOWN = 203,
		RPL_TRACEOPERATOR = 204, RPL_TRACEUSER = 205, RPL_TRACESERVER = 206, RPL_TRACESERVICE = 207,
		RPL_TRACENEWTYPE = 208, RPL_TRACECLASS = 209, RPL_TRACERECONNECT = 210, RPL_TRACELOG = 261,
		RPL_TRACEEND = 262, RPL_STATSLINKINFO = 211, RPL_STATSCOMMANDS = 212, RPL_ENDOFSTATS = 219,
		RPL_STATSUPTIME = 242, RPL_STATSOLINE = 243, RPL_UMODEIS = 221, RPL_SERVLIST = 234, RPL_SERVLISTEND = 235,
		RPL_LUSERCLIENT = 251, RPL_LUSEROP = 252, RPL_LUSERUNKNOWN = 253, RPL_LUSERCHANNELS = 254, RPL_LUSERME = 255,
		RPL_ADMINME = 256, RPL_ADMINLOC1 = 257, RPL_ADMINLOC2 = 258, RPL_ADMINEMAIL = 259, RPL_TRYAGAIN = 263,

		// Error replies
		ERR_NOSUCHNICK = 401, ERR_NOSUCHSERVER = 402, ERR_NOSUCHCHANNEL = 403, ERR_CANNOTSENDTOCHAN = 404,
		ERR_TOOMANYCHANNELS = 405, ERR_WASNOSUCHNICK = 406, ERR_TOOMANYTARGETS = 407, ERR_NOSUCHSERVICE = 408,
		ERR_NOORIGIN = 409, ERR_NORECIPIENT = 411, ERR_NOTEXTTOSEND = 412, ERR_NOTOPLEVEL = 413, ERR_WILDTOPLEVEL = 414, ERR_BADMASK = 415, ERR_UNKNOWNCOMMAND = 421, ERR_NOMOTD = 422,
		ERR_NOADMININFO = 423, ERR_FILEERROR = 424, ERR_NONICKNAMEGIVEN = 431, ERR_ERRONEUSNICKNAME = 432, ERR_NICKNAMEINUSE = 433, 
		ERR_NICKCOLLISION = 436, ERR_UNAVAILRESOURCE = 437, ERR_USERNOTINCHANNEL = 441, ERR_NOTONCHANNEL = 442, ERR_USERONCHANNEL = 443, 
		ERR_NOLOGIN = 444, ERR_SUMMONDISABLED = 445, ERR_USERSDISABLED = 446, ERR_NOTREGISTERED = 451, ERR_NEEDMOREPARAMS = 461, 
		ERR_ALREADYREGISTRED = 462, ERR_NOPERMFORHOST = 463, ERR_PASSWDMISMATCH = 464, ERR_YOUREBANNEDCREEP = 465, ERR_YOUWILLBEBANNED = 466, 
		ERR_KEYSET = 467, ERR_CHANNELISFULL = 471, ERR_UNKNOWNMODE = 472, ERR_INVITEONLYCHAN = 473, ERR_BANNEDFROMCHAN = 474, ERR_BADCHANNELKEY = 475, 
		ERR_BADCHANMASK = 476, ERR_NOCHANMODES = 477, ERR_BANLISTFULL = 478, ERR_NOPRIVILEGES = 481, ERR_CHANOPRIVSNEEDED = 482, ERR_CANTKILLSERVER = 483, 
		ERR_RESTRICTED = 484, ERR_UNIQOPPRIVSNEEDED = 485, ERR_NOOPERHOST = 491, ERR_UMODEUNKNOWNFLAG = 501, ERR_USERSDONTMATCH = 502
	};

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
//		unsigned char numParams;
	};
}
