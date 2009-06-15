#include "CSLock.h"
#include "logging.h"
#include <boost/logging/format/named_write.hpp>

// memdbgon must be the last include file in a .cpp file!!! 
#include "tier0/memdbgon.h"

namespace SteamIRC {

CSLock::CSLock(CRITICAL_SECTION& cs) : cs_(cs)
{
	L_ << "CSLock::CSLock() - Before lock";
	EnterCriticalSection(&cs_);
	L_ << "CSLock::CSLock() - After lock";
}

CSLock::~CSLock(void)
{
	L_ << "CSLock::~CSLock() - Before release";
	LeaveCriticalSection(&cs_);
	L_ << "CSLock::~CSLock() - After release";
}

}
