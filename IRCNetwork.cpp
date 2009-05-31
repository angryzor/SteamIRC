#include "eiface.h"
#include "IRCNetwork.h"
#include "IRCChannel.h"

namespace SteamIRC {

	using namespace std;
	CIRCNetwork::CIRCNetwork(String uri, CIRCEnvironment& env, IRCUserInfo& usr) : CIRCContextWithCommands(uri), env_(env), usr_(usr)
	{
	}

	bool CIRCNetwork::AcceptIncoming(const IRCMessage& msg) {
		IRCMessage reply;
		switch(msg.Cmnd) {
		case JOIN: // Actually join a channel (channel join succesful)
			if(msg.Origin.nick != usr_.Nick) return false; // If this isn't a message about ourselves, then it's just someone else who joined a chnnel we're in.
													   // In that case, this message should be handled by the channel context.
			Join(msg.Parameters[0]);
			break;
		case PING: // Regular pingpong event
			Warning("Ping?");
			reply.SetCommand(PONG);
			reply.SetParam(0, msg.Parameters[0]);
			env_.Send(reply);
			Warning(" Pong!\r\n");
			break;
		default:
			return false;
		}
		return true;
	}
	
	bool CIRCNetwork::ProcessUserCommand(const String& cmnd, istringstream& params) {
		if(cmnd == "join") {
			String chan;
			if(!(params >> chan)) throw std::runtime_error("USAGE: /join #channame");

			IRCMessage msg(JOIN);
			msg.SetParam(0, chan);

			env_.Send(msg);
			return true;
		}
		return false;
	}

	void CIRCNetwork::Join(String chan) {
		CIRCChannel* newChan = new CIRCChannel(env_, chan);
		env_.Add(newChan);
		env_.SetActiveContext(newChan);
	}

	CIRCNetwork::~CIRCNetwork(void)
	{
	}

}
