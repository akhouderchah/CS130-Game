#pragma once

#include <sstream>

/**
 * Set of macro functions to abstract away the Log class details.
 *
 * Debug logs are written to stderr and (optionally) a file.
 * Regular logs are written only to a file.
 *
 * Note: Unix-based systems could do input redirection in place of this,
 * but this allows for finer control over where things are logged.
 */
#define DEBUG_LOG() \
	if(!Log::IsDebug() || Log::GetInfoLevel() < EIL_NORMAL) ;	\
	else Log().GetStream()
#define DEBUG_LOG_VERBOSE() \
	if(!Log::IsDebug() || Log::GetInfoLevel() < EIL_VERBOSE) ; \
	else Log().GetStream()
#define DEBUG_LOG_IMPORTANT() \
	if(!Log::IsDebug() || Log::GetInfoLevel() < EIL_IMPORTANT) ; \
	else Log().GetStream()
#define LOG() \
	if(Log::GetInfoLevel() < EIL_NORMAL) ; \
	else Log().GetStream()
#define LOG_VERBOSE() \
	if(Log::GetInfoLevel() < EIL_VERBOSE) ; \
	else Log().GetStream()
#define LOG_IMPORTANT() \
	if(Log::GetInfoLevel() < EIL_IMPORTANT) ; \
	else Log().GetStream()
			 
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
	Log();
	~Log();

	enum EInfoLevel
	{
		EIL_NONE = -1,
		EIL_IMPORTANT,
		EIL_NORMAL,
		EIL_VERBOSE
	};
	
	std::ostream& GetStream(EInfoLevel InfoLevel);

	static inline EInfoLevel GetInfoLevel(){ return s_InfoLevel; }
	static inline bool IsDebug(){ return s_LogDebug; }
private:
	Log(const Log&);
	Log& operator=(const Log&);
	
private:
	static EInfoLevel s_InfoLevel;
	static bool s_LogDebug;
	static std::string s_DefaultFile;
	bool m_IsDelayed;
	std::ostream m_Stream;
};

