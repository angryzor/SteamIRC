#pragma once
#include <string>
namespace SteamIRC
{
	struct IRCUserInfo
	{
		std::string Nick;
		std::string UserName;
		unsigned long autoModeBM;
		std::string RealName;
	};
}
