#include "TetradGame.h"

#include "EntityManager.h"
#include "ErrorSystem.h"
#include "EventSystem.h"
#include "ObserverComponent.h"
#include "PhysicsSystem.h"

TetradGame::TetradGame() :
	m_pDrawSystem(nullptr), m_pSystemObserver(nullptr),
	m_pInputSystem(nullptr)
{
}

bool TetradGame::Initialize(const GameAttributes& attributes)
{
	// Game class contains important initializing functionality
	if(!Game::Initialize(attributes))
	{
		ERROR("Failed to initialize engine systems!\n", EEB_CONTINUE);
		return false;
	}

	// Create entities
	Entity test = EntityManager::CreateEntity();
	test.Add<TransformComponent>()->Init(glm::vec3(0,0,1));
	test.Add<MovableComponent>();
	DrawComponent *pDraw = test.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture("../../assets/textures/Background.tga");

	for(int i = 0; i < 200; ++i)
	{
		test = EntityManager::CreateEntity();
		test.Add<TransformComponent>()->Init(glm::vec3(i*.5, 0.f, 1.f),
											 glm::vec3(.2f, .2f, .2f));
		test.Add<MovableComponent>();
		pDraw = test.Add<DrawComponent>();
		pDraw->SetGeometry(ShapeType::PLANE);
		pDraw->SetTexture("../../assets/textures/City.tga");
		test.Add<PhysicsComponent>();
		ObserverComponent* pObserver = test.Add<ObserverComponent>();
		pObserver->Subscribe(*m_pInputSystem);
		pObserver->AddEvent(EGameEvent(EGE_PLAYER1_JUMP), new Action_Jump(test));
	}

	// Create fade screen entity
	test = EntityManager::CreateEntity();
	test.Add<TransformComponent>()->Init(glm::vec3(0,0,1));
	pDraw = test.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture("../../assets/textures/PauseGradient.tga", true);
	pDraw->SetOpacity(0.f);
	test.Add<MaterialComponent>();
	Action_PauseGame::SetFadeScreen(test);

	m_Timer.Start();

	return true;
}

void TetradGame::AddSystems()
{
	m_pInputSystem = new EventSystem;
	m_pInputSystem->MakeInputSystem();
	m_pSystems.push_back(m_pInputSystem);

	m_pSystems.push_back(new PhysicsSystem);

	m_pDrawSystem = new DrawSystem;
	m_pSystems.push_back(m_pDrawSystem);

	// Create the system observer
	m_pSystemObserver = EntityManager::CreateEntity().Add<ObserverComponent>();
	m_pSystemObserver->Subscribe(*m_pInputSystem);
	m_pSystemObserver->AddEvent(EGE_PAUSE, new Action_PauseGame(m_pWindow));
}
