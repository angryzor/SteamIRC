#pragma once
#include <windows.h>

/*
RIAA Class that enters and leaves a critical section
*/


namespace SteamIRC {

class CSLock
{
public:
	CSLock(CRITICAL_SECTION& cs);
	virtual ~CSLock(void);
private:
	CSLock(CSLock&);
	CSLock& operator=(CSLock&);
	CRITICAL_SECTION& cs_;
};

}
