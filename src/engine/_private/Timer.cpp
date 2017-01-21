#include "Timer.h"

const deltaTime_t MAX_DELTA_TIME = 0.1f;
const deltaTime_t DEFAULT_DELTA_TIME = 0.016666f;

Timer::Timer() :
	m_StartTime(0.0), m_CurrTime(0.0), m_PrevTime(0.0),
	m_Paused(true)
{
}

Timer::~Timer()
{
}

void Timer::Start()
{
	m_StartTime = (deltaTime_t)glfwGetTime();
	m_PrevTime = (deltaTime_t)m_StartTime;

	m_Paused = false;
}

void Timer::Pause()
{
	m_Paused = true;
}

void Timer::Resume()
{
	m_PrevTime = (deltaTime_t)glfwGetTime();

	m_Paused = false;
}

deltaTime_t Timer::Tick()
{
	if( m_Paused )
	{
		return (m_DeltaTime = 0.0);
	}
	m_CurrTime = (deltaTime_t)glfwGetTime();
	m_DeltaTime = m_CurrTime - m_PrevTime;
	m_PrevTime = m_CurrTime;

	return m_DeltaTime;
}

deltaTime_t Timer::GetTotalTime()
{
	return (m_CurrTime - m_StartTime);
}

