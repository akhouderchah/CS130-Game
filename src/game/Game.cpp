#include "Game.h"

using namespace sf;

GameAttributes::GameAttributes(uint32_t width, uint32_t height, String title, ContextSettings settings) :
	m_VideoMode(width, height), m_WindowTitle(title), 
	m_ContextSettings(settings)
{
}

Game::Game() :
	m_DeltaTime(0.0)
{

}

Game::~Game()
{
}

bool Game::Initialize(const GameAttributes& attributes)
{
	m_Window.create(sf::VideoMode(attributes.m_VideoMode), attributes.m_WindowTitle, Style::Close | Style::Titlebar, attributes.m_ContextSettings);

	// Initialize all systems here //
	

	// Sytsem initialization ends here //
	
	m_Clock.restart();

	return true;
}

void Game::Shutdown()
{
	m_Window.close();
}

void Game::Run()
{
	while(m_Window.isOpen())
	{
		m_DeltaTime = m_Clock.restart().asSeconds();

		// Handle Events
		// TODO - Input system should handle this!
		Event event;
		while(m_Window.pollEvent(event))
		{
			switch(event.type)
			{
				case Event::Closed:
					m_Window.close();
					break;
				default:
					break;
			}
		}
		
		Tick();
	}
}

void Game::Reset()
{
}

void Game::Tick()
{
	// Tick all of the systems
	
	m_Window.clear();
	// Render system goes here
	
	m_Window.display();
}

