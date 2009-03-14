#pragma once

#include <stdexcept>
#include "StdString.h"
#include <sstream>

namespace SteamIRC
{
	namespace WinSock2
	{
// abstract errors
		class wsa_runtime_error : public std::runtime_error
		{
		public:
			wsa_runtime_error(String what) : std::runtime_error(what) {
				err = WSAGetLastError();
			}
			virtual const char* what() {
				std::ostringstream os;
				os << std::runtime_error::what() << std::endl << _T("Error code:") << std::hex << err;
				return os.str().c_str();
			}
		private:
			int err;
		};
	}
}
