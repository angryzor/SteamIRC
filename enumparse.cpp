#include "enumparse.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

namespace angry_util {
	namespace enum_parse {
		template<typename enum_type>
		void enum_parser<enum_type>::add(enum_type e, std::string str) {
			table_[e] = str;
		}

		template<typename enum_type>
		std::string enum_parser<enum_type>::get_string(enum_type e) const {
			container_type::iterator found = table_.find(e);
			if(found == table_.end()) {
				ostringstream err;
				err << "could not find string for enum value " << e;
				throw std::logic_error(err.str());
			}
			else return found->second;
		}

		template<typename enum_type>
		enum_type enum_parser<enum_type>::get_enum(std::string str) const {
			for(container_type::const_iterator i = table_.begin(); i != table_.end(); i++)
				if(i->second == str)
					return i->first;
			throw std::logic_error(std::string("could not identify string ") + str);
		}
	}
}
