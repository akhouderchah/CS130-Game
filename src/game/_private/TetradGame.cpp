#include "TetradGame.h"

#include "EntityManager.h"
#include "ErrorSystem.h"
#include "EventSystem.h"
#include "ObserverComponent.h"
#include "PhysicsSystem.h"
#include "CollisionComponent.h"

TetradGame::TetradGame() :
	m_pDrawSystem(nullptr), m_pSystemObserver(nullptr),
	m_pInputSystem(nullptr), m_pSoundSystem(nullptr)
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

	// Load user-provided key configurations
	EventSystem::SetInputConfig(KEY_PATH + "test.conf");

	// Create background
	// TODO - transform values only work for this particular aspect ratio
	// Only leaving the code like this in case we want to do something different
	// after the midterm demo
	Entity entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(0, 0, 1), glm::vec3(1.5f, 1.15f, 1));
	entity.Add<MovableComponent>();
	DrawComponent *pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(BACKGROUND_PATH, TextureType::RGB);
	entity.Add<MaterialComponent>()->SetScrollRate(-0.2f);

	// Create floor
	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(0, -1.5f, 0.0f), glm::vec3(2.5f, 0.2f, 100)); //Set to 100 so we wont fall off of Z coordinate
	entity.Add<MovableComponent>();
	entity.Add<PhysicsComponent>();
	pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(FLOOR_PATH, TextureType::RGBA);
	entity.Add<MaterialComponent>()->SetScrollRate(-0.75f);
	CollisionComponent * pCol = entity.Add<CollisionComponent>();
	//pCol->addPlane();
	pCol->addBox(0);
	pCol->setBounciness(1.0);

	// Create camera
	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(0, 0, 3));
	entity.Add<MovableComponent>();
	CameraComponent *pCamera = entity.Add<CameraComponent>();
	pCamera->SetCurrentCamera(pCamera);
	SoundComponent *pSound = entity.Add<SoundComponent>();
	pSound->LoadSound("backgroundMusic", SOUND_PATH + "backgroundMusic.wav", IS_LOOP);
	pSound->PlaySound("backgroundMusic");

	// Create jumping boxes
	for(int i = 0; i < 1; ++i)
	{
		entity = EntityManager::CreateEntity();
		entity.Add<TransformComponent>()->Init(glm::vec3(0.f, 1.f, 0.f),
			glm::vec3(.2f, .2f, .2f));
		entity.Add<MovableComponent>();
		pDraw = entity.Add<DrawComponent>();
		pDraw->SetGeometry(ShapeType::PLANE);
		pDraw->SetTexture(TEXTURE_PATH + "bird.tga", TextureType::RGBA);
		entity.Add<PhysicsComponent>();
		ObserverComponent *pObserver = entity.Add<ObserverComponent>();
		pObserver->Subscribe(*m_pInputSystem);
		pObserver->AddEvent(EGameEvent(EGE_PLAYER1_JUMP), new Action_Jump(entity));
		pObserver->AddEvent(EGameEvent(EGE_PLAYER1_LEFT), new Action_Left_Right(entity, LEFT));
		pObserver->AddEvent(EGameEvent(EGE_PLAYER1_RIGHT), new Action_Left_Right(entity, RIGHT));
		SoundComponent *pSound = entity.Add<SoundComponent>();
		pSound->LoadSound("wingsFlap", SOUND_PATH + "wingSound.wav", !IS_LOOP);
		CollisionComponent * pCol = entity.Add<CollisionComponent>();
		pCol->addSphere(1);
		//pCol->addBox(1);
		pCol->setBounciness(0.5);
		pCol->setMovement(XY);
	}

	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(0.5f, 1.f, 0.f),
		glm::vec3(.2f, .2f, .2f));
	entity.Add<MovableComponent>();
	pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "bird.tga", TextureType::RGBA);
	entity.Add<PhysicsComponent>();
	pCol = entity.Add<CollisionComponent>();
	pCol->addBox(1.0f);
	pCol->setMovement(XY);


	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(-1.f, 1.f, 0.f),
		glm::vec3(.5f, .5f, .5f));
	entity.Add<MovableComponent>();
	pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "Black.tga", TextureType::RGBA);
	entity.Add<PhysicsComponent>();
	pCol = entity.Add<CollisionComponent>();
	pCol->addBox(5.0f);
	pCol->setMovement(XY);

	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(-1.f, 1.8f, 0.f),
		glm::vec3(.2f, .2f, .2f));
	entity.Add<MovableComponent>();
	pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "bird.tga", TextureType::RGBA);
	entity.Add<PhysicsComponent>();
	pCol = entity.Add<CollisionComponent>();
	pCol->addSphere(1.0f);
	pCol->setMovement(XY);






	// Create fade screen entity
	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(0, 0, 1));
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

	m_pSoundSystem = new SoundSystem;
	m_pSystems.push_back(m_pSoundSystem);

	m_pCollisionSystem = new CollisionSystem;
	m_pSystems.push_back(m_pCollisionSystem);

	// Create the system observer
	m_pSystemObserver = EntityManager::CreateEntity().Add<ObserverComponent>();
	m_pSystemObserver->Subscribe(*m_pInputSystem);
	m_pSystemObserver->AddEvent(EGE_PAUSE, new Action_PauseGame(m_pWindow));
}
