#pragma once

#include "../base/Base.h"

struct GameAttributes
{
	GameAttributes(uint32_t width = 1024, uint32_t height = 768, sf::String title = "Test Game", sf::ContextSettings settings = sf::ContextSettings());

	sf::VideoMode m_VideoMode;
	std::string m_WindowTitle;
	sf::ContextSettings m_ContextSettings;
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
	void Tick();

private:
	sf::RenderWindow m_Window;
	sf::Clock m_Clock;
	deltaTime_t m_DeltaTime;
};

