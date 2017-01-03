#include "LogSystem.h"
#include "Platform.h"

#include <iostream>
#include <string>

using namespace std;

ostream* g_pConsoleStream = &clog;
ostream* g_pDebugConsoleStream = &cerr;

const streampos Log::s_MAX_DELAY_SIZE = 4096;

const string GetTimeStr()
{
	static time_t t;
	struct tm * ptm;
	t = time(nullptr);
	ptm = localtime(&t);

	char result[20];
	sprintf(result, "%02d/%02d/%04d %02d:%02d:%02d", ptm->tm_mon+1, ptm->tm_mday,
			1900 + ptm->tm_year, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);

	return result;
}

const string LOG_HEADER(const string& title)
{
	return "[" + title + " : " + GetTimeStr() + "] - ";
}

Log::Log(const string& logPath, EInfoLevel minLog, EInfoLevel maxDelay) :
	m_MinLogLevel(minLog), m_MaxDelayLevel(maxDelay), m_HasOpenFile(true)
{
	// Note that we can still show debug logs in a release build, we
	// just have to set it manually when it's needed
	#ifdef _DEBUG
	m_WriteDebug = true;
	#else
	m_WriteDebug = false;
	#endif

	//// Atempt to open the log file
	// Note that we assume the user knows what they're doing and won't make
	// multiple logs going to the same file (all bets are off if that happens)
	m_pImmediateStream = new ofstream(logPath);
	if(!*m_pImmediateStream)
	{
		cerr << "Failed to open " << logPath << " for use as a log file!\n";
		cerr << "\tUsing cerr for this log instead\n";
		delete m_pImmediateStream;
		m_pImmediateStream = &cerr;
		m_HasOpenFile = false;
	}
	m_pDelayedStream = new ostringstream;
}

Log::~Log()
{
	//// Flush any remaining messages in the delayed stream
	(*m_pImmediateStream) << m_pDelayedStream->str();

	if(m_HasOpenFile)
	{
		((ofstream*)m_pImmediateStream)->close();
		delete m_pImmediateStream;
	}
	delete m_pDelayedStream;
}

ostream& Log::GetStream(EInfoLevel infoLevel)
{
	if(infoLevel > m_MaxDelayLevel) // If we are doing an immediate write
	{
		// Flush delay stream
		(*m_pImmediateStream) << m_pDelayedStream->str();
		m_pDelayedStream->str("");

		return *m_pImmediateStream;
	}

	if(m_pDelayedStream->tellp() >= s_MAX_DELAY_SIZE)
	{
		(*m_pImmediateStream) << m_pDelayedStream->str();
		m_pDelayedStream->str("");
	}

	return *m_pDelayedStream;
}

