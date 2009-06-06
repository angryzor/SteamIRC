#include "eiface.h"
#include "IRCNetwork.h"
#include "IRCChannel.h"
#include "IRCPMContext.h"

#include "tier0/memdbgon.h"
namespace SteamIRC {

	using namespace std;
	CIRCNetwork::CIRCNetwork(std::string uri, CIRCEnvironment& env) : CIRCContextWithCommands(uri, env)
	{
	}

	CIRCContext::AcceptReturnValue CIRCNetwork::AcceptIncoming(const IRCMessage& msg) {
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

			if( msg.Origin.nick == "StatServ" ||
				msg.Origin.nick == "NickServ" ||
				msg.Origin.nick == "ChanServ") {
				buffer_ += "\x03" "04[";
				buffer_ += msg.Origin.nick;
				buffer_ += " ";
				buffer_ += msg.Parameters[1];
				buffer_ += "]\x03\n-\n";
			}
			else if(env_.ContextExists(msg.Origin.nick)) return CIRCContextWithCommands::AcceptIncoming(msg);
			else BuildPMContext(msg.Origin.nick, msg.Parameters[1]);
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
		case MODE:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick) return CIRCContextWithCommands::AcceptIncoming(msg);

			if(msg.Origin.nick == "") {
				buffer_ += "\x03" "03* User mode is ";
			}
			else {
				buffer_ += "\x03" "03* ";
				buffer_ += msg.Origin.nick;
				buffer_ += " sets mode ";
			}
			for(IRCMessage::param_vec::const_iterator i = msg.Parameters.begin() + 1; i != msg.Parameters.end(); i++) {
				buffer_ += *i;
				buffer_ += " ";
			}
			buffer_ += "\x03\n-\n";
			break;
		case RPL_MYINFO:
		case RPL_BOUNCE:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick && msg.Parameters[0] != "*") return CIRCContextWithCommands::AcceptIncoming(msg);
			for(IRCMessage::param_vec::const_iterator i = msg.Parameters.begin() + 1; i != msg.Parameters.end(); i++) {
				buffer_ += *i;
				buffer_ += " ";
			}
			buffer_ += "\n-\n";
			break;
		// Standard replies with 1 param
		case RPL_WELCOME:
		case RPL_YOURHOST:
		case RPL_CREATED:
		case RPL_USERHOST:
		case RPL_ISON:
		case RPL_UNAWAY:
		case RPL_NOWAWAY:
		case RPL_MOTDSTART:
		case RPL_ENDOFMOTD:
		case RPL_UMODEIS:
		case RPL_LUSERCLIENT:
		case RPL_LUSERME:
		case ERR_NOMOTD:
		case ERR_NONICKNAMEGIVEN:
		case ERR_NOTREGISTERED:
		case ERR_YOUREBANNEDCREEP:
		case ERR_NORECIPIENT:
		case ERR_NOTEXTTOSEND:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick && msg.Parameters[0] != "*") return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += msg.Parameters[1];
			buffer_ += "\n-\n";
			break;
		case RPL_MOTD:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick && msg.Parameters[0] != "*") return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += msg.Parameters[1];
			buffer_ += "\n";
			break;
		case RPL_WHOISUSER:
		case RPL_WHOWASUSER:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick && msg.Parameters[0] != "*") return CIRCContextWithCommands::AcceptIncoming(msg);
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
			if(msg.Parameters[0] != env_.GetUInfo()->Nick && msg.Parameters[0] != "*") return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += msg.Parameters[1];
			buffer_ += " is connecting from ";
			buffer_ += msg.Parameters[2];
			buffer_ += " ";
			buffer_ += msg.Parameters[3];
			buffer_ += "\n";
			break;
		case RPL_WHOISOPERATOR:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick && msg.Parameters[0] != "*") return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += msg.Parameters[1];
			buffer_ += " ";
			buffer_ += msg.Parameters[2];
			buffer_ += "\n";
			break;
		case RPL_WHOISIDLE:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick && msg.Parameters[0] != "*") return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += msg.Parameters[1];
			buffer_ += " ";
			buffer_ += msg.Parameters[2];
			buffer_ += " ";
			buffer_ += msg.Parameters[3];
			buffer_ += "\n";
			break;
		// Standard replies with 2 params
		case RPL_ENDOFWHOIS:
		case RPL_ENDOFWHOWAS:
		case RPL_LUSEROP:
		case RPL_LUSERUNKNOWN:
		case RPL_LUSERCHANNELS:
		case ERR_TOOMANYTARGETS:
		case ERR_NOSUCHNICK:
		case ERR_NOSUCHSERVER:
		case ERR_NOSUCHCHANNEL:
		case ERR_CANNOTSENDTOCHAN:
		case ERR_TOOMANYCHANNELS:
		case ERR_UNKNOWNCOMMAND:
		case ERR_NEEDMOREPARAMS:
		case ERR_CHANNELISFULL:
		case ERR_UNKNOWNMODE:
		case ERR_INVITEONLYCHAN:
		case ERR_BANNEDFROMCHAN:
		case ERR_BADCHANNELKEY:
		case ERR_BADCHANMASK:
		case ERR_NOCHANMODES:
		case ERR_NOTOPLEVEL:
		case ERR_WILDTOPLEVEL:
		case ERR_BADMASK:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick && msg.Parameters[0] != "*") return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += msg.Parameters[1];
			buffer_ += " ";
			buffer_ += msg.Parameters[2];
			buffer_ += "\n-\n";
			break;
		case ERR_ERRONEUSNICKNAME:
		case ERR_UNAVAILRESOURCE:
		case ERR_NICKNAMEINUSE:
		case ERR_NICKCOLLISION:
			if(msg.Parameters[1] != env_.GetUInfo()->Nick) return CIRCContextWithCommands::AcceptIncoming(msg);
			// Correct our name back to what it was...
			env_.GetUInfo()->Nick = msg.Parameters[0];
			buffer_ += msg.Parameters[1];
			buffer_ += " ";
			buffer_ += msg.Parameters[2];
			buffer_ += "\n-\n";
			break;
		case RPL_INVITING:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick && msg.Parameters[0] != "*") return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += "Invited ";
			buffer_ += msg.Parameters[1];
			buffer_ += " to ";
			buffer_ += msg.Parameters[2];
			buffer_ += ".\n-\n";
			break;
		// Standard replies with 3 params
		case ERR_USERNOTINCHANNEL:
		case ERR_BANLISTFULL:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick && msg.Parameters[0] != "*") return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += msg.Parameters[1];
			buffer_ += " ";
			buffer_ += msg.Parameters[2];
			buffer_ += " ";
			buffer_ += msg.Parameters[3];
			buffer_ += "\n-\n";
			break;
		case RPL_WHOISCHANNELS:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick && msg.Parameters[0] != "*") return CIRCContextWithCommands::AcceptIncoming(msg);
			buffer_ += "Channels: ";
			buffer_ += msg.Parameters[2];
			buffer_ += "\n";
			break;
		default:
			return CIRCContextWithCommands::AcceptIncoming(msg);
		}
		return ARV_PROCESSED;
	}
	
	bool CIRCNetwork::ProcessUserCommand(const std::string& cmnd, istringstream& params) {
		if(cmnd == "mode") {
			std::string tmp;
			IRCMessage msg(MODE);
			msg.AddParam(env_.GetUInfo()->Nick);
			if(!(params >> tmp)) throw std::runtime_error("USAGE: /mode [<mode commands>]");
			do {
				msg.AddParam(tmp);
			} while(params >> tmp);
			env_.Send(msg);
			return true;
		}
		return CIRCContextWithCommands::ProcessUserCommand(cmnd, params);
	}

	void CIRCNetwork::Join(std::string chan) {
		CIRCChannel* newChan = new CIRCChannel(env_, chan);
		env_.Add(newChan);
		env_.SetActiveContext(newChan);
	}

	void CIRCNetwork::BuildPMContext(std::string other, std::string msg) {
		CIRCPMContext* newPMC = new CIRCPMContext(env_, other);
		env_.Add(newPMC);
		env_.SetActiveContext(newPMC);
		newPMC->Message(other, msg);
	}

	CIRCNetwork::~CIRCNetwork(void)
	{
	}

}
