#include "IRCChannelUser.h"

namespace SteamIRC {
	IRCChannelUser::IRCChannelUser(std::string nick) {
		ParseDecoratedNick(nick);
	}

	std::string IRCChannelUser::GetDecoratedNick() const {
		return decs_ + nick_;
	}

	std::string IRCChannelUser::GetNick() const {
		return nick_;
	}

	std::string IRCChannelUser::GetDecorations() const {
		return decs_;
	}

	void IRCChannelUser::ParseDecoratedNick(std::string nick) {
		if(!(   (nick[0] >= 65 && nick[0] <= 93)
			|| (nick[0] >= 97 && nick[0] <= 125))) {
			decs_ = nick.substr(0,1);
			nick_ = nick.substr(1);
		}
		else {
			decs_ = "";
			nick_ = nick;
		}
	}

	bool IRCChannelUser::operator<(const IRCChannelUser& Other) const {
		return GetDecoratedNick() < Other.GetDecoratedNick();
	}

	IRCChannelUser::~IRCChannelUser(void)
	{
	}
}
