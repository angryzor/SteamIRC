#pragma once

#include <boost/unordered/unordered_map.hpp>
#include <stdexcept>
#include <sstream>
#include <string>

namespace angry_util {
	namespace enum_parse {
		template<typename enum_type>
		class enum_parser {
		private:
			typedef boost::unordered_map<enum_type, std::string> container_type;
			container_type table_;
		public:
			enum_parser(){}
			virtual ~enum_parser(){}
			void add(enum_type e, std::string str);
			std::string get_string (enum_type e) const;
			enum_type get_enum (std::string str) const;
		};
	}
}
