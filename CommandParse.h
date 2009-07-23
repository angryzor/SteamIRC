#pragma once

#include "enumparse.h"
#include "ircmessages.h"

namespace SteamIRC {
	class CommandParse :
		public angry_util::enum_parse::enum_parser<Command>
	{
	public:
		CommandParse(void);
		virtual ~CommandParse(void);
	};
}
