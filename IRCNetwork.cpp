#include "IRCNetwork.h"
#include "IRCChannel.h"

namespace SteamIRC {

	CIRCNetwork::CIRCNetwork(CIRCEnvironment& env, String nick) : env_(env), nick_(nick)
	{
	}

	bool CIRCNetwork::AcceptIncoming(IRCMessage& msg) {
		IRCMessage reply;
		switch(msg.Cmnd) {
		case JOIN: // Actually join a channel (channel join succesful)
			if(msg.Origin.nick != nick_) return false;
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
	
	bool CIRCNetwork::UserInput(String txt) {
		try {
			if(txt[0] == '/') {
				std::istringstream iss;
				iss.str(txt.substr(1));

				String cmnd;
				iss >> cmnd;

				if(cmnd == "join") {
					String chan;
					if(!(iss >> chan)) throw std::runtime_error("USAGE: /join #channame");

					IRCMessage msg(JOIN);
					msg.SetParam(0, chan);

					env_.Send(msg);
					return true;
				}
			}
			return false;
		}
		catch(std::runtime_error e) {
			buf += String("ERROR: ") + e.what();
		}
	}

	void CIRCNetwork::Join(String chan) {
		env_.Add(new CIRCChannel(env_, chan));
	}

	CIRCNetwork::~CIRCNetwork(void)
	{
	}

}
