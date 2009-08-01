#pragma once

#include <vector>
#include <boost/unordered/unordered_map.hpp>
#include <map>
#include <string>

namespace SteamIRC {
	using namespace std;
	class ICRMessage;
	class IRCNetworkCaps
	{
	public:
		enum CaseMappingType {
			CM_ASCII,
			CM_RFC1459,
			CM_RFC1459_STRICT
		};
		struct ChanStatusMode {
			char prefix;
			char mode;
		};
		struct ChanModes {
			vector<char> A;
			vector<char> B;
			vector<char> C;
			vector<char> D;
		};
		struct ChannelType {
			char prefix;
			int limit;
		};

		typedef boost::unordered_map<char, int> maxlistmodelength_type;
		typedef vector<ChannelType> channeltypes_type;
		typedef vector<ChanStatusMode*> statusdirectednoticemodes_type;
		typedef vector<ChanStatusMode> chanstatusmodes_type;

		IRCNetworkCaps(void);
		virtual ~IRCNetworkCaps(void);
		bool AreBanExceptsOn();
		bool AreInviteExceptsOn();
		void AddCapabilities(const IRCMessage& msg);


		CaseMappingType Casemapping;
		channeltypes_type ChannelTypes;
		int MaxChannelNameLength;
		char BanExceptsMode;
		char InviteExceptsMode;
		int MaxKickReasonLength;
		maxlistmodelength_type MaxListModeLength;
		int MaxModesCount;
		std::string NetworkName;
		int MaxNicknameLength;
		statusdirectednoticemodes_type StatusDirectedNoticeModes;
		chanstatusmodes_type ChanStatusModes;

	private:
		typedef map<string, string> tier1decomposition_type;
		tier1decomposition_type DecomposeTier1(const IRCMessage& msg);
		
	};
}
