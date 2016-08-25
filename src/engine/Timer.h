#pragma once

#include "Base.h"

/**
 * @brief Handles timing details (particularily with pausing and getting delta times).
 *
 * Essentially abstracts away GLFW-specific details from the rest
 * of the game.
 */
class Timer
{
public:
	Timer();
	~Timer();

	void Start();
	void Pause();
	void Resume();

	deltaTime_t Tick();
	deltaTime_t GetTotalTime();
private:
	Timer(const Timer& timer);
	Timer& operator=(const Timer& timer);

private:
	deltaTime_t m_StartTime;

	deltaTime_t m_CurrTime;
	deltaTime_t m_PrevTime;
	deltaTime_t m_DeltaTime;

	bool m_Paused;
};
