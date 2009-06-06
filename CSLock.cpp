#include "CSLock.h"

namespace SteamIRC {

CSLock::CSLock(CRITICAL_SECTION& cs) : cs_(cs)
{
	EnterCriticalSection(&cs_);
}

CSLock::~CSLock(void)
{
	LeaveCriticalSection(&cs_);
}

}
