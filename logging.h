#pragma once

#include <boost/logging/format/named_write_fwd.hpp>

namespace SteamIRC {
	namespace bl = boost::logging;
	typedef bl::named_logger<>::type logger_type;
	typedef bl::filter::no_ts filter_type;

	BOOST_DECLARE_LOG_FILTER(g_l_filter, filter_type)
	BOOST_DECLARE_LOG(g_l, logger_type)

#define L_ BOOST_LOG_USE_LOG_IF_FILTER( g_l(), g_l_filter()->is_enabled() ) 

	void init_logging();
}

