#pragma once
#include <string>

namespace SteamIRC {
	class IRCChannelUser
	{
	public:
		IRCChannelUser(std::string nick);
		std::string GetDecoratedNick() const;
		std::string GetNick() const;
		std::string GetDecorations() const;
		void ParseDecoratedNick(std::string nick);
		virtual ~IRCChannelUser(void);
		bool operator<(const IRCChannelUser& Other) const;
	private:
		std::string decs_;
		std::string nick_;
	};
}
