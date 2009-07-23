#include <sstream>
#include "IRCMessage.h"
#include <xstring>
#include "logging.h"
#include <boost/logging/format/named_write.hpp>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

namespace SteamIRC
{
	IRCMessage::IRCMessage(void) : Origin("unknown")
	{
	}
		
	IRCMessage::IRCMessage(std::string& msgStr) : Origin("unknown")
	{
		ProcessString(msgStr);
	}

	IRCMessage::IRCMessage(Command cmd) : Origin("unknown")
	{
		Cmnd = cmd;
	}
	void IRCMessage::ProcessString(const std::string& msgStr)
	{
		L_ << "IRCMessage::ProcessString() - About to process string " << msgStr;
		// A plain text message string is converted to and saved into this IRCMessage here
		
		// I'll be using an istringstream here, they're fun to parse simple stuff with :)
		std::istringstream iss;
		
		iss.str(msgStr);

		std::string tmp;
		std::string tmp2;
		unsigned short numeralCmd;

		// First, check if we have a prefix.
		if( msgStr[0] == ':' ) // yes, we have one
		{
			iss >> tmp; // stream it into tmp
			Origin = IRCOrigin(tmp.substr(1)); // and cut off the colon
		}
		else
			Origin = IRCOrigin("unknown");
		// There, we set the origin. Now for the actual command

		// This is a very easy way of determining if the next part is a numeral command or a textual one.
		// This will fail if it isn't.
		iss >> std::dec >> numeralCmd;
		if(!iss) // if it failed
		{
			// clear the stream's state
			iss.clear();

			iss >> tmp;

			Cmnd = LookupStringToCommand(tmp);
		} 
		else 
		{
			Cmnd = static_cast<Command>(numeralCmd);
		}

		// OK, we got the command. This is the last and hardest part,
		// the parameters
		while(!iss.eof())
		{
			if(!(iss >> tmp2)) break;

			if(tmp2[0] == ':') // This is a very special first character. It means that everything behind it,
			{							  // including spaces, is part of this parameter.
				tmp2 = tmp2.substr(1);
				std::getline(iss, tmp);
				if(iss) tmp2 += tmp;
/*				tmp2.append(" ");
				
				while(!iss.eof())
				{
					if(!(iss >> tmp)) break;
					tmp2.append(tmp);
					tmp2.append(" ");
				}
				tmp2 = tmp2.substr(0, tmp2.length() - 1);
*/
			}
			AddParam(tmp2);
		}
	}

	std::string IRCMessage::GetString(bool includeOrigin) const
	{
		std::string tmp;
		std::ostringstream oss;
		if(includeOrigin) oss << Origin.toStr() + " ";
		oss << LookupCommandToString(Cmnd);

		if(Parameters.size() > 0)
		{
			for(std::vector<std::string>::size_type i = 0; i < Parameters.size() - 1; i++)
			{
				oss << ' ' << Parameters[i];
			}
			oss << " :" << Parameters[Parameters.size() - 1];
		}

		oss << "\r\n";

		tmp = oss.str();
		return tmp;
	}

	void IRCMessage::AddParam(std::string val) {
		Parameters.insert(Parameters.end(), val);
	}

	void IRCMessage::AddParam(int val) {
		std::ostringstream oss;
		oss << val;
		Parameters.insert(Parameters.end(), oss.str());
	}

	void IRCMessage::SetParam(int index, std::string val)
	{
		Parameters[index] = val;
	}

	void IRCMessage::SetParam(int index, int val)
	{
		std::ostringstream oss;
		oss << val;
		Parameters[index] = oss.str();
	}

	Command IRCMessage::LookupStringToCommand(const std::string& str) const 
	{
		std::istringstream iss;
		unsigned short tmp = 0;
		iss >> std::dec >> tmp;
		if(iss) return static_cast<Command>(tmp);
		else return command_parser_.get_enum(str);
	}

	std::string IRCMessage::LookupCommandToString(Command cmd) const
	{
		if(cmd < 600) {
			std::ostringstream oss;
			oss << static_cast<unsigned short>(cmd);
			return oss.str();
		}
		else return command_parser_.get_string(cmd);
	}

	void IRCMessage::SetCommand(Command cmd)
	{
		Cmnd = cmd;
	}

	void IRCMessage::Reset()
	{
		Cmnd = NICK;
		Parameters.clear();
	}

	IRCMessage::~IRCMessage(void)
	{
	}


}
