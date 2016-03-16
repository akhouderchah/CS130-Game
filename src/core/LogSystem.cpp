#include "LogSystem.h"
#include "../base/Platform.h"

using namespace std;

EInfoLevel Log::s_InfoLevel = EIL_NORMAL;
#ifdef _DEBUG
bool Log::s_LogDebug = true;
#else
bool Log::s_LogDebug = false;
#endif

Log::Log()
{
}

Log::~Log()
{
}

ostream& Log::GetStream()
{
	// Set to delayed write if InfoLevel != EIL_IMPORTANT
	m_IsDelayed = InfoLevel;
}

