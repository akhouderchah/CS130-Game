#pragma once

#include "Config.h"

enum EPlatforms
{
	EP_INVALID = 0,
	EP_LINUX = 1,
	EP_WINDOWS = 2,
	EP_MAC_OSX = 3
};

enum EBuildTypes
{
	EBT_DEBUG = 0,
	EBT_RELEASE
};

#if(BUILD_TYPE == EBT_DEBUG)
#define _DEBUG
#else
#undef _DEBUG
#endif

#if(SYSTEM_TYPE == EP_WINDOWS)
#include <Windows.h>
#elif(SYSTEM_TYPE == EP_LINUX)
// Linux-specific includes
#elif(SYSTEM_TYPE == EP_MAC_OSX)
// OSX-specific includes
#else
// Platform-indendent alternatives only
#endif

// Run at the beginning of execution to do platform-specific initialization
bool ProgramInitialize();

