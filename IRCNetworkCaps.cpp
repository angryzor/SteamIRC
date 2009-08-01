#include "IRCNetworkCaps.h"
#include "IRCMessage.h"

namespace SteamIRC {
	namespace IRCCaps {
		IRCNetworkCaps::IRCNetworkCaps(void)
		{
		}

		IRCNetworkCaps::~IRCNetworkCaps(void)
		{
		}
		tier1decomposition_type IRCNetworkCaps::DecomposeTier1(const IRCMessage& msg) {
			tier1decomposition_type tmp;
			for(IRCMessage::param_vec::size_type i = 0; i < msg.Parameters.size() - 1; i++) { // The last param is just a string like "are supported by this server"
				std::string::size_type pos(Parameters[i].find_first_of('='));
				tmp.insert(ICRMessage::param_vec::value_type(	Parameters[i].substr(0, pos),
																Parameters[i].substr(pos+1)));
			}
			return tmp;
		}
		void IRCNetworkCaps::AddCapabilities(const IRCMessage& msg) {
			tier1decomposition_type t1(DecomposeTier1(msg));
			

	}
}
