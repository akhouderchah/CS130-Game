#include "Game.h"
#include <chrono>
#include "EntityManager.h"
#include "Entity.h"

/** System Includes */
#include "InputSystem.h"
#include "DrawSystem.h"

GameAttributes::GameAttributes(int32_t width, int32_t height, std::string title, bool fullscreen, uint8_t samples) :
	m_Width(width), m_Height(height), m_WindowTitle(title),
	m_Fullscreen(fullscreen), m_SampleCount(samples)
{
}

Game::Game() :
	m_pWindow(nullptr)
{
}

Game::~Game()
{
}

bool Game::Initialize(const GameAttributes& attributes)
{
	Random::SetSeed(static_cast<unsigned long>(std::chrono::system_clock::now().time_since_epoch().count()));

	if(!glfwInit())
	{
		// @TODO - Error
		return false;
	}

	// Create window
	if(!CreatePrimaryWindow(attributes))
	{
		// @TODO - Error
		return false;
	}

	glewExperimental = true;

	if(glewInit() != GLEW_OK)
	{
		// @TODO - Error
		return false;
	}

	glfwSetInputMode(m_pWindow, GLFW_STICKY_KEYS, GL_TRUE);

	glfwSetKeyCallback(m_pWindow,KeyCallback);

	ISystem::SetWindow(m_pWindow);

	// Initialize all systems here //
	EntityManager::Initialize();

	m_pSystems.push_back(new InputSystem);
	m_pSystems.push_back(new DrawSystem);

	// Sytsem initialization ends here //
	
	Entity test = EntityManager::CreateEntity();
	test.AddComponent(TransformComponent());
	test.AddComponent(DrawComponent());
	
	m_Timer.Start();

	return true;
}

void Game::Shutdown()
{
	for(size_t i = m_pSystems.size(); i > 0;)
	{
		m_pSystems[--i]->Shutdown();
		delete m_pSystems[i];
	}

	EntityManager::Shutdown();

	glfwTerminate();
}

void Game::Run()
{
	deltaTime_t deltaTime;
	while(!glfwWindowShouldClose(m_pWindow))
	{
		deltaTime = m_Timer.Tick();

		for(size_t i = 0; i < m_pSystems.size(); ++i)
		{
			m_pSystems[i]->Tick(deltaTime);
		}
	}
}

void Game::Reset()
{
}

bool Game::CreatePrimaryWindow(const GameAttributes& attributes)
{
	if(m_pWindow != nullptr)
	{
		glfwDestroyWindow(m_pWindow);
	}

	GLFWmonitor* pMonitor = nullptr;
	if(attributes.m_Fullscreen)
	{
		pMonitor = glfwGetPrimaryMonitor();
	}

	glfwWindowHint(GLFW_SAMPLES, attributes.m_SampleCount);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_pWindow = glfwCreateWindow(attributes.m_Width, attributes.m_Height, attributes.m_WindowTitle.c_str(), pMonitor, NULL);

	if(!m_pWindow)
	{
		// @TODO - Error
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_pWindow);
	
	return true;
}

