// @TODO Get ERROR working properly (give us file and line number at least!)

#pragma once
#include <string>
#include "Platform.h"
#include <cassert>
#include "LogSystem.h"

enum EErrorBehavior
{
	EEB_CONTINUE,
	EEB_EXIT
};

void EXIT();

/**
 * @brief Function to handle errors (log them and then decide what to do)
 */
#define ERROR(stream, errorBehavior) \
	_LOG_ERROR(MainLog, Log::EIL_NORMAL, stream); \
	if(errorBehavior == EEB_EXIT){ EXIT(); }

// @TODO - make own assert!
#define RELEASE_ASSERT(cond) assert(cond)

#ifdef _DEBUG
#define DEBUG_ASSERT(cond) RELEASE_ASSERT(cond)
#else
#define DEBUG_ASSERT(cond)
#endif

