#include "eiface.h"
#include "IRCNetwork.h"
#include "IRCChannel.h"

#include "tier0/memdbgon.h"
namespace SteamIRC {

	using namespace std;
	CIRCNetwork::CIRCNetwork(std::string uri, CIRCEnvironment& env) : CIRCContextWithCommands(uri, env)
	{
	}

	bool CIRCNetwork::AcceptIncoming(const IRCMessage& msg) {
		IRCMessage reply;
		switch(msg.Cmnd) {
		case JOIN: // Actually join a channel (channel join succesful)
			if(msg.Origin.nick != env_.GetUInfo()->Nick) return CIRCContextWithCommands::AcceptIncoming(msg); // If this isn't a message about ourselves, then it's just someone else who joined a chnnel we're in.
													   // In that case, this message should be handled by the channel context.
			Join(msg.Parameters[0]);
			break;
		case PING: // Regular pingpong event
			buffer_ += "Ping?";
			reply.SetCommand(PONG);
			reply.AddParam(msg.Parameters[0]);
			env_.Send(reply);
			buffer_ += " Pong!\n-\n";
			break;
		case PRIVMSG:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return CIRCContextWithCommands::AcceptIncoming(msg);

			buffer_ += "\x03" "04[";
			buffer_ += msg.Origin.nick;
			buffer_ += " ";
			buffer_ += msg.Parameters[1];
			buffer_ += "]\x03\n-\n";
			break;
		case NOTICE:
			if(    msg.Parameters[0] != env_.GetUInfo()->Nick
				&& msg.Parameters[0] != "AUTH") return CIRCContextWithCommands::AcceptIncoming(msg);

			buffer_ += "\x03" "05-";
			if(msg.Origin.nick == "") buffer_ += msg.Origin.addr;
			else buffer_ += msg.Origin.nick;
			buffer_ += "- ";
			buffer_ += msg.Parameters[1];
			buffer_ += "\x03\n-\n";
			break;
		case RPL_WELCOME:
		case RPL_YOURHOST:
		case RPL_CREATED:
		case RPL_MYINFO:
		case RPL_BOUNCE:
		case RPL_USERHOST:
		case RPL_ISON:
		case RPL_UNAWAY:
		case RPL_NOWAWAY:
		case RPL_MOTDSTART:
		case RPL_ENDOFMOTD:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += msg.Parameters[1];
			buffer_ += "\n-\n";
			break;
		case RPL_MOTD:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += msg.Parameters[1];
			buffer_ += "\n";
			break;
		case RPL_WHOISUSER:
		case RPL_WHOWASUSER:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += msg.Parameters[1];
			buffer_ += " ";
			buffer_ += msg.Parameters[2];
			buffer_ += "@";
			buffer_ += msg.Parameters[3];
			buffer_ += " ";
			buffer_ += msg.Parameters[4];
			buffer_ += " ";
			buffer_ += msg.Parameters[5];
			buffer_ += "\n";
			break;
		case RPL_WHOISSERVER:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += msg.Parameters[1];
			buffer_ += " is connecting from ";
			buffer_ += msg.Parameters[2];
			buffer_ += " ";
			buffer_ += msg.Parameters[3];
			buffer_ += "\n";
			break;
		case RPL_WHOISOPERATOR:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += msg.Parameters[1];
			buffer_ += " ";
			buffer_ += msg.Parameters[2];
			buffer_ += "\n";
			break;
		case RPL_WHOISIDLE:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += msg.Parameters[1];
			buffer_ += " ";
			buffer_ += msg.Parameters[2];
			buffer_ += " ";
			buffer_ += msg.Parameters[3];
			buffer_ += "\n";
			break;
		case RPL_ENDOFWHOIS:
		case RPL_ENDOFWHOWAS:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += msg.Parameters[1];
			buffer_ += " ";
			buffer_ += msg.Parameters[2];
			buffer_ += "\n-\n";
			break;
		case RPL_WHOISCHANNELS:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += "Channels: ";
			buffer_ += msg.Parameters[2];
			buffer_ += "\n";
			break;
		default:
			return false;
		}
		return true;
	}
	
	bool CIRCNetwork::ProcessUserCommand(const std::string& cmnd, istringstream& params) {
		if(cmnd == "join") {
			std::string chan;
			if(!(params >> chan)) throw std::runtime_error("USAGE: /join #channame");

			IRCMessage msg(JOIN);
			msg.AddParam(chan);

			env_.Send(msg);
			return true;
		}
		return false;
	}

	void CIRCNetwork::Join(std::string chan) {
		CIRCChannel* newChan = new CIRCChannel(env_, chan);
		env_.Add(newChan);
		env_.SetActiveContext(newChan);
	}

	CIRCNetwork::~CIRCNetwork(void)
	{
	}

}
