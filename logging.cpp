#include "logging.h"
#include <fstream>

#include <boost/logging/format/named_write.hpp>
// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"

namespace SteamIRC {
	BOOST_DEFINE_LOG_FILTER(g_l_filter, filter_type ) 
	BOOST_DEFINE_LOG(g_l, logger_type) 


	void init_logging() {
		// Quickly erase the previous file (faster than having to learn the complete boost logging
		// library interface :P)
		{
			std::ofstream ofs("steamirc.log", std::ios::trunc);
			if(ofs.is_open()) {
				ofs << "SteamIRC Log\r\n------------------------\r\n";
				ofs.close();
			}
		}

		g_l()->writer().write("[%time%($hh:$mm.$ss.$mili)] |\n", "file(steamirc.log)");
		g_l()->mark_as_initialized();
	}
}
