// @TODO Get ERROR working properly (give us file and line number at least!)

#pragma once
#include <string>
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

