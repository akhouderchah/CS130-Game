// TODO - Controlled creation of log files (as opposed to regular static init)
// TODO - More macro defs
// TODO - Error logging

#pragma once

#include <sstream>
#include <fstream>
#include <ctime>

/**
 * Set of macro functions to abstract away the Log class details.
 *
 * Logs are written to a file, and optionally written to console as well.
 *
 * The log macro naming convention is that the "general" macros are prefixed with
 * an underscore (general meaning made for use in other macros, not directly by
 * the user), while macros for the user are not prefixed by an underscore.
 *
 * Note: Unix-based systems could do input redirection in place of this,
 * but this allows for finer control over where things are logged.
 *
 * Note: "Flexible" logging means the decision of what and when to write (debug
 * and info levels) happens at runtime. If LOG_NOT_FLEXIBLE is #defined, that
 * decision will be made at compile-time instead, losing flexibility but removing
 * extra branches from the codebase.
 */

//#define LOG_NOT_FLEXIBLE
//#define LOG_ALLOW_DEBUG
//#define LOG_ALLOW_VERBOSE
//#define LOG_CONSOLE
#define LOG_DEBUG_CONSOLE

extern std::ostream* g_pConsoleStream;
extern std::ostream* g_pDebugConsoleStream;

const std::string GetTimeStr();

const std::string LOG_HEADER(const std::string& title);
const std::string LOG_DEBUG_HEADER = "{DEBUG : " + GetTimeStr() + "] - ";
const std::string LOG_ERROR_HEADER = "[ERROR : " + GetTimeStr() + "] - ";

#ifdef LOG_CONSOLE
#define _CONSOLE_PRINT(stream) ; (*pConsole) << stream
#else
#define _CONSOLE_PRINT(stream)
#endif

#ifdef LOG_DEBUG_CONSOLE
#define _CONSOLE_DEBUG_PRINT(stream) ; (*g_pDebugConsoleStream) << stream
#else
#define _CONSOLE_DEBUG_PRINT(stream)
#endif

#ifndef LOG_NOT_FLEXIBLE
#define _LOG(logger, infoLevel, title, stream) \
	if(logger.GetMinLevel() > infoLevel) {} \
	else logger.GetStream(infoLevel) << LOG_HEADER(title) << stream _CONSOLE_PRINT(stream)
#define _DEBUG_LOG(logger, infoLevel, stream) \
	if(!logger.DebugEnabled() || logger.GetMinLevel() > infoLevel) {} \
	else logger.GetStream(infoLevel) << LOG_DEBUG_HEADER << stream _CONSOLE_DEBUG_PRINT(stream)
#define LOG(stream) _LOG(MainLog, Log::EIL_NORMAL, "log", stream)
#define LOG_SPECIAL(title, stream) _LOG(MainLog, Log::EIL_NORMAL, title, stream)
#define DEBUG_LOG(stream) _DEBUG_LOG(MainLog, Log::EIL_NORMAL, "log", stream)

#else
#define _LOG(logger, infoLevel, title, stream) \
	logger.GetStream(infoLevel) << LOG_HEADER(title) << stream _CONSOLE_PRINT(stream)
#if LOG_ALLOW_DEBUG
#define _DEBUG_LOG(logger, infoLevel, stream) logger.GetStream(infoLevel) << LOG_DEBUG_HEADER << stream _CONSOLE_DEBUG_PRINT(stream)
#else
#define _DEBUG_LOG(logger, infoLevel, stream)
#endif

#define LOG(stream) _LOG(MainLog, Log::EIL_NORMAL, "log", stream)
#define LOG_SPECIAL(title, stream) _LOG(MainLog, Log::EIL_NORMAL, title, stream)
#define DEBUG_LOG(stream) _DEBUG_LOG(MainLog, Log::EIL_NORMAL, stream)
#endif

// We want all errors to be logged, so error writes are unconditional
#define _LOG_ERROR(logger, infoLevel, stream)					   \
	logger.GetStream(infoLevel) << LOG_ERROR_HEADER << stream;			   \
	(*g_pConsoleStream) << LOG_ERROR_HEADER << stream;
			 
/**
 * @brief Class that handles the heavy-lifting of logging
 *
 * Has two write streams: Delayed and Immediate. Immediate streams
 * write directly to files, while delayed streams are string streams
 * that will periodically be written to files.
 *
 * @note Users should be using the #defines above instead of directly
 * working with this class.
 */
class Log
{
public:
	enum EInfoLevel
	{
		EIL_NONE = -1,
		EIL_VERBOSE,
		EIL_NORMAL,
		EIL_IMPORTANT
	};

	Log(const std::string& logPath, EInfoLevel minLog=EIL_NORMAL, EInfoLevel maxDelay=EIL_NORMAL);
	~Log();

	EInfoLevel GetMinLevel() const{ return m_MinLogLevel; }
	bool DebugEnabled() const{ return m_WriteDebug; }

	std::ostream& GetStream(EInfoLevel infoLevel);
private:
	Log(const Log&);
	Log& operator=(const Log&);

private:
	EInfoLevel m_MinLogLevel; // We won't log any messages below this level
	EInfoLevel m_MaxDelayLevel; // We won't do delayed writes on anything above this level
	static const std::streampos s_MAX_DELAY_SIZE;
	bool m_WriteDebug;
	bool m_HasOpenFile;
	std::ostream* m_pImmediateStream;
	std::ostringstream* m_pDelayedStream;
};

static Log MainLog("test.log");
