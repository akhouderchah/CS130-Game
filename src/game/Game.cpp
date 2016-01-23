#include "Game.h"
#include <chrono>
#include "EntityManager.h"

/** System Includes */
#include "InputSystem.h"
#include "DrawSystem.h"

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
	Random::SetSeed(static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));

	m_Window.create(sf::VideoMode(attributes.m_VideoMode), attributes.m_WindowTitle, Style::Close | Style::Titlebar, attributes.m_ContextSettings);

	ISystem::SetWindow(&m_Window);

	// Initialize all systems here //
	EntityManager::Initialize();
	
	try
	{
		m_pSystems.push_back(new InputSystem);
		m_pSystems.push_back(new DrawSystem);
	}
	catch (std::bad_alloc& ba)
	{
		m_Window.close();
		EntityManager::Shutdown();
		return false;
	}
	// Sytsem initialization ends here //
	
	m_Clock.restart();

	return true;
}

void Game::Shutdown()
{
	m_Window.close();

	for(size_t i = m_pSystems.size(); i > 0;)
	{
		m_pSystems[--i]->Shutdown();
		delete m_pSystems[i];
	}

	EntityManager::Shutdown();
}

void Game::Run()
{
	while(m_Window.isOpen())
	{
		m_DeltaTime = m_Clock.restart().asSeconds();

		Tick();
	}
}

void Game::Reset()
{
}

void Game::Tick()
{
	// Tick all of the systems
	for(size_t i = 0; i < m_pSystems.size(); ++i)
	{
		m_pSystems[i]->Tick(m_DeltaTime);
	}
}

