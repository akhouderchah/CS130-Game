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

	// Create background
	Entity entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(0,0,1));
	entity.Add<MovableComponent>();
	DrawComponent *pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(BACKGROUND_PATH, TextureType::RGB);
	entity.Add<MaterialComponent>()->SetTimeRate(-0.2f);

	// Create floor
	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(0,-0.9f,1), glm::vec3(1,0.1f,1));
	entity.Add<MovableComponent>();
	pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(FLOOR_PATH, TextureType::RGBA);
	entity.Add<MaterialComponent>()->SetTimeRate(-0.75f);

	// Create jumping boxes
	for(int i = 0; i < 2; ++i)
	{
		entity = EntityManager::CreateEntity();
		entity.Add<TransformComponent>()->Init(glm::vec3(i*.5, 0.f, 1.f),
											 glm::vec3(.2f, .2f, .2f));
		entity.Add<MovableComponent>();
		pDraw = entity.Add<DrawComponent>();
		pDraw->SetGeometry(ShapeType::PLANE);
		pDraw->SetTexture(TEXTURE_PATH + "Black.tga", TextureType::RGB);
		entity.Add<PhysicsComponent>();
		ObserverComponent* pObserver = entity.Add<ObserverComponent>();
		pObserver->Subscribe(*m_pInputSystem);
		pObserver->AddEvent(EGameEvent(EGE_PLAYER1_JUMP), new Action_Jump(entity));
	}

	// Create fade screen entity
	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(0,0,1));
	pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(PAUSE_BACKGROUND_PATH, TextureType::RGBA);
	entity.Add<MaterialComponent>()->SetOpacity(0.f);
	Action_PauseGame::SetFadeScreen(entity);

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
