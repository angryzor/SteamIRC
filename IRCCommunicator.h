#pragma once
#include "IRCContextWithCommands.h"
#include "IRCMessage.h"
#include "IRCEnvironment.h"

namespace SteamIRC {

	class CIRCCommunicator :
		public CIRCContextWithCommands
	{
	public:
		virtual bool UserInput(const std::string& txt);
		virtual AcceptReturnValue AcceptIncoming(const IRCMessage& msg);
		virtual bool ProcessUserCommand(std::string cmnd, std::istringstream& params);
		virtual void Message(std::string from, std::string msg);
		void SetID(std::string id);
		virtual ~CIRCCommunicator(void);
	protected:
		CIRCCommunicator(std::string id, CIRCEnvironment& env);
		std::string id_;
	private:
		CIRCCommunicator& operator=(CIRCCommunicator&);
	};

}
