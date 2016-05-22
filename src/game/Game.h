#pragma once

#include "../base/Base.h"
#include "ISystem.h"
#include "DrawSystem.h"
#include "Timer.h"

struct GameAttributes
{
	GameAttributes(int32_t width = 1024, int32_t height = 768, std::string title = "Test Game", bool fullscreen = false, uint8_t samples = 4);

	int32_t m_Width, m_Height;
	std::string m_WindowTitle;
	bool m_Fullscreen;
	uint8_t m_SampleCount;
};

/*
 * @brief - Highest-level abstraction of a game
 *
 * Should be used as a base class for specific games, but generally
 * contains window dimensions/attributes, global game data, game systems, etc.
 */
class Game
{
public:
	Game();
	~Game();

	bool Initialize(const GameAttributes& attributes);
	void Shutdown();
	void Run();
	void Reset();

private:
	bool CreatePrimaryWindow(const GameAttributes& attributes);

private:
	Timer m_Timer;
	GLFWwindow* m_pWindow;

	DrawSystem* m_pDrawSystem;
	std::vector<ISystem*> m_pSystems;
};

