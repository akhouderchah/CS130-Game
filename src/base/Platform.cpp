#include "Platform.h"

bool ProgramInitialize()
{
	bool success = true;
#if(SYSTEM_TYPE == EP_WINDOWS)
#ifndef _DEBUG
	success *= FreeConsole();
#endif//_DEBUG

#elif(SYSTEM_TYPE == EP_LINUX)

#elif(SYSTEM_TYPE == EP_MAC_OSX)

#endif
	return success;
}

