#include "IRCChannel.h"
#include <xstring>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"
#include <ctime>

namespace SteamIRC
{
	CIRCChannel::CIRCChannel(CIRCEnvironment& env, std::string name) : CIRCCommunicator(name, env), sendingNames_(false)
	{
	}

	void CIRCChannel::Part(std::istream& params)
	{
		std::string byemsg;
		IRCMessage msg(PART);
		msg.AddParam(id_);
		std::getline(params, byemsg);
		if(byemsg != "") {
			msg.AddParam(byemsg);
		}
		env_.Send(msg);
	}
	
	bool CIRCChannel::AcceptIncoming(const IRCMessage& msg) {
		IRCMessage reply;
		std::string tmp;
		std::istringstream iss;
		chanfolk_set::iterator iter;
		switch(msg.Cmnd) {
		case JOIN:
			if(msg.Parameters[0] != id_) return CIRCCommunicator::AcceptIncoming(msg);

			chanfolk_.insert(chanfolk_.end(), IRCChannelUser(msg.Origin.nick));
			buffer_ += "\x03" "03* ";
			buffer_ += msg.Origin.nick;
			buffer_ += " has joined ";
			buffer_ += id_;
			buffer_ += ".\x03\n";
			reply.Reset();
			reply.SetCommand(NAMES);
			reply.AddParam(id_);
			env_.Send(reply);
			break;
		case PART:
			if(msg.Parameters[0] != id_) return CIRCCommunicator::AcceptIncoming(msg);

			EraseNick(msg.Origin.nick);
			if(msg.Origin.nick == env_.GetUInfo()->Nick) DestroyChannel();
			else {
				buffer_ += "\x03" "03* ";
				buffer_ += msg.Origin.nick;
				buffer_ += " left ";
				buffer_ += id_;
				buffer_ += ".";
				if(msg.Parameters.size() > 1) {
					buffer_ += " (";
					buffer_ += msg.Parameters[1];
					buffer_ += ")";
				}
				buffer_ += "\x03\n";
			}
			break;
		case QUIT:
			if((iter = FindByNick(msg.Origin.nick)) != chanfolk_.end()) { 
				chanfolk_.erase(iter);
				buffer_ += "\x03" "02* ";
				buffer_ += msg.Origin.nick;
				buffer_ += " quit.";
				if(msg.Parameters.size() > 0) {
					buffer_ += " (";
					buffer_ += msg.Parameters[0];
					buffer_ += ")";
				}
				buffer_ += "\x03\n";
			}
			return CIRCCommunicator::AcceptIncoming(msg);
		case NOTICE:
			if(msg.Parameters[0] != id_) return CIRCCommunicator::AcceptIncoming(msg);

			buffer_ += "\x03" "05-";
			buffer_ += msg.Origin.nick;
			buffer_ += ":";
			buffer_ += msg.Parameters[0];
			buffer_ += "- ";
			buffer_ += msg.Parameters[1];
			buffer_ += "\x03\n";
			break;
		case TOPIC:
			if(msg.Parameters[0] != id_) return CIRCCommunicator::AcceptIncoming(msg);

			if(msg.Origin.nick == "") {
				buffer_ += "\x03" "03* Channel topic is ";
			}
			else {
				buffer_ += "\x03" "03* ";
				buffer_ += msg.Origin.nick;
				buffer_ += " sets topic ";
			}
			buffer_ += msg.Parameters[1];
			buffer_ += "\x03\n";
			break;
		case MODE:
			if(msg.Parameters[0] != id_) return CIRCCommunicator::AcceptIncoming(msg);

			if(msg.Origin.nick == "") {
				buffer_ += "\x03" "03* Channel mode is ";
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
			buffer_ += "\x03\n";
			reply.Reset();
			reply.SetCommand(NAMES);
			reply.AddParam(id_);
			env_.Send(reply);
			break;
		case RPL_TOPIC:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick
				|| msg.Parameters[1] != id_) return CIRCCommunicator::AcceptIncoming(msg);
			buffer_ += "\x03" "03* Topic is: ";
			buffer_ += msg.Parameters[2];
			buffer_ += "\x03\n";
			break;
		case RPL_TOPICTIMESTAMP:
			{
				if(msg.Parameters[0] != env_.GetUInfo()->Nick
					|| msg.Parameters[1] != id_) return CIRCCommunicator::AcceptIncoming(msg);
				iss.str(msg.Parameters[3]);
				time_t rawtime;
				if(!(iss >> rawtime)) return CIRCCommunicator::AcceptIncoming(msg);
				tm* timeinfo;
				timeinfo = localtime(&rawtime);
				buffer_ += "\x03" "03* Topic set by ";
				buffer_ += msg.Parameters[2];
				buffer_ += " at ";
				buffer_ += asctime(timeinfo); // No pretty conventional decolor--newline... This bitchs adds a newline itself.
			}
			break;
		case RPL_NOTOPIC:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick
				|| msg.Parameters[1] != id_) return CIRCCommunicator::AcceptIncoming(msg);
			buffer_ += "\x03" "03* No topic is set.\n";
			break;
		case RPL_NAMREPLY:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick
				|| msg.Parameters[2] != id_) return CIRCCommunicator::AcceptIncoming(msg);
			iss.str(msg.Parameters[3]);
			if(!sendingNames_) chanfolk_.clear();
			sendingNames_ = true;
			while(!iss.eof()) {
				iss >> tmp;
				chanfolk_.insert(chanfolk_.end(), tmp);
			}
			break;
		case RPL_ENDOFNAMES:
			if(msg.Parameters[0] != env_.GetUInfo()->Nick
				|| msg.Parameters[1] != id_) return CIRCCommunicator::AcceptIncoming(msg);
			sendingNames_ = false;
			break;
		default:
			return CIRCCommunicator::AcceptIncoming(msg);
		}
		return true;
	}

	bool CIRCChannel::ProcessUserCommand(const std::string& cmnd, std::istringstream& params) {
		if(cmnd == "part" || cmnd == "leave") {
			Part(params);
			return true;
		}
		return CIRCCommunicator::ProcessUserCommand(cmnd, params);
	}

	CIRCChannel::chanfolk_set::iterator CIRCChannel::FindByNick(std::string nick) {
		for( chanfolk_set::iterator i = chanfolk_.begin(); i != chanfolk_.end(); i++)
			if(i->GetNick() == nick) {
				return i;
			}
		return chanfolk_.end();
	}

	void CIRCChannel::EraseNick(std::string nick) {
		for( chanfolk_set::iterator i = chanfolk_.begin(); i != chanfolk_.end(); i++)
			if(i->GetNick() == nick) {
				chanfolk_.erase(i);
				break;
			}
	}

	void CIRCChannel::DestroyChannel() {
		if(env_.GetActiveContext() == this) {
			CIRCEnvironment::context_set::iterator i = env_.GetContexts()->find(this);
			i--;
			env_.SetActiveContext(*i);
		}
		env_.Remove(this);
		delete this;
	}

	CIRCChannel::chanfolk_set* CIRCChannel::GetChanFolk() {
		return &chanfolk_;
	}
		
	CIRCChannel::~CIRCChannel(void)
	{
	}
}
