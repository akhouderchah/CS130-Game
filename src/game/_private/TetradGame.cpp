#include "TetradGame.h"

#include "EntityManager.h"
#include "ErrorSystem.h"
#include "EventSystem.h"
#include "ObserverComponent.h"
#include "PhysicsSystem.h"

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
	entity.Add<TransformComponent>()->Init(glm::vec3(0, 0, 1), glm::vec3(3.f, 2.3f, 1));
	entity.Add<MovableComponent>();
	DrawComponent *pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(BACKGROUND_PATH, TextureType::RGB);
	entity.Add<MaterialComponent>()->SetScrollRate(-0.2f);

	for (int i = 0; i < 20; i++)
	{
		// Create floor
		entity = EntityManager::CreateEntity();
		entity.Add<TransformComponent>()->Init(glm::vec3(-3.5 + 0.4*i, -2.6f, 0.0f),
			glm::vec3(0.2f, 0.2f, 100)); //Set to 100 so we wont fall off of Z coordinate
		entity.Add<MovableComponent>();
		pDraw = entity.Add<DrawComponent>();
		pDraw->SetGeometry(ShapeType::PLANE);
		pDraw->SetTexture(TEXTURE_PATH + "platform_ground.tga", TextureType::RGBA);
		//entity.Add<MaterialComponent>()->SetScrollRate(-0.75f);
		PhysicsComponent * pPhysics = entity.Add<PhysicsComponent>();
		//pPhysics->addPlane();
		pPhysics->addBox(0);
		pPhysics->setBounciness(1.0);
	}
		
	

	// Create camera
	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(0, 0, 5));
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
			glm::vec3(.2f, .15f, .2f));
		entity.Add<MovableComponent>();
		pDraw = entity.Add<DrawComponent>();
		pDraw->SetGeometry(ShapeType::PLANE);
		pDraw->SetTexture(TEXTURE_PATH + "bird.tga", TextureType::RGBA);
		ObserverComponent *pObserver = entity.Add<ObserverComponent>();
		pObserver->Subscribe(*m_pInputSystem);
		pObserver->AddEvent(EGameEvent(EGE_PLAYER1_JUMP), new Action_Jump(entity));
		pObserver->AddEvent(EGameEvent(EGE_PLAYER1_LEFT), new Action_Left_Right(entity, left));
		pObserver->AddEvent(EGameEvent(EGE_PLAYER1_RIGHT), new Action_Left_Right(entity, right));
		SoundComponent *pSound = entity.Add<SoundComponent>();
		pSound->LoadSound("wingsFlap", SOUND_PATH + "wingSound.wav", !IS_LOOP);
		PhysicsComponent * pPhysics = entity.Add<PhysicsComponent>();
		pPhysics->addBox(1);
		pPhysics->setBounciness(0.3);
		pPhysics->setRotation(btVector3(0, 0, 1));
		pPhysics->setMovement(btVector3(1, 1, 0));
	}
	////////////////////////////////////////////////////////////////////////////
	

	for (int i = 0; i < 5; i++)
	{
		entity = EntityManager::CreateEntity();
		entity.Add<TransformComponent>()->Init(glm::vec3(1.f, -2.2f + i*0.5, 0.f),
			glm::vec3(.05f, .2f, .2f));
		entity.Add<MovableComponent>();
		pDraw = entity.Add<DrawComponent>();
		pDraw->SetGeometry(ShapeType::PLANE);
		pDraw->SetTexture(TEXTURE_PATH + "brick_v.tga", TextureType::RGBA);
		PhysicsComponent *pPhysics = entity.Add<PhysicsComponent>();
		pPhysics->addBox(1.0f);
		pPhysics->setRotation(btVector3(0, 0, 1));
		pPhysics->setMovement(btVector3(1, 1, 0));


		entity = EntityManager::CreateEntity();
		entity.Add<TransformComponent>()->Init(glm::vec3(1.3f, -2.2f + i*0.5, 0.f),
			glm::vec3(.05f, .2f, .2f));
		entity.Add<MovableComponent>();
		pDraw = entity.Add<DrawComponent>();
		pDraw->SetGeometry(ShapeType::PLANE);
		pDraw->SetTexture(TEXTURE_PATH + "brick_v.tga", TextureType::RGBA);
		pPhysics = entity.Add<PhysicsComponent>();
		pPhysics->addBox(1.0f);
		pPhysics->setRotation(btVector3(0, 0, 1));
		pPhysics->setMovement(btVector3(1, 1, 0));

		entity = EntityManager::CreateEntity();
		entity.Add<TransformComponent>()->Init(glm::vec3(1.6f, -2.2f + i*0.5, 0.f),
			glm::vec3(.05f, .2f, .2f));
		entity.Add<MovableComponent>();
		pDraw = entity.Add<DrawComponent>();
		pDraw->SetGeometry(ShapeType::PLANE);
		pDraw->SetTexture(TEXTURE_PATH + "brick_v.tga", TextureType::RGBA);
		pPhysics = entity.Add<PhysicsComponent>();
		pPhysics->addBox(1.0f);
		pPhysics->setRotation(btVector3(0, 0, 1));
		pPhysics->setMovement(btVector3(1, 1, 0));


		entity = EntityManager::CreateEntity();
		entity.Add<TransformComponent>()->Init(glm::vec3(1.1f, -1.95f + i*0.5, 0.f),
			glm::vec3(.2f, .05f, .2f));
		entity.Add<MovableComponent>();
		pDraw = entity.Add<DrawComponent>();
		pDraw->SetGeometry(ShapeType::PLANE);
		pDraw->SetTexture(TEXTURE_PATH + "brick_h.tga", TextureType::RGBA);
		pPhysics = entity.Add<PhysicsComponent>();
		pPhysics->addBox(1.0f);
		pPhysics->setRotation(btVector3(0, 0, 1));
		pPhysics->setMovement(btVector3(1, 1, 0));


		entity = EntityManager::CreateEntity();
		entity.Add<TransformComponent>()->Init(glm::vec3(1.5f, -1.95f + i*0.5, 0.f),
			glm::vec3(.2f, .05f, .2f));
		entity.Add<MovableComponent>();
		pDraw = entity.Add<DrawComponent>();
		pDraw->SetGeometry(ShapeType::PLANE);
		pDraw->SetTexture(TEXTURE_PATH + "brick_h.tga", TextureType::RGBA);
		pPhysics = entity.Add<PhysicsComponent>();
		pPhysics->addBox(1.0f);
		pPhysics->setRotation(btVector3(0, 0, 1));
		pPhysics->setMovement(btVector3(1, 1, 0));
	}
	////////////////////////////////////////////////////////////////////////////

	//Box
	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(-2.f, 1.f, 0.f),
		glm::vec3(.5f, .5f, .5f));
	entity.Add<MovableComponent>();
	pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "box.tga", TextureType::RGBA);
	PhysicsComponent *pPhysics = entity.Add<PhysicsComponent>();
	pPhysics->addBox(20.0f);
	pPhysics->setRotation(btVector3(0, 0, 1));
	pPhysics->setMovement(btVector3(1, 1, 0));


	//Rolling object
	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(-2.f, 1.8f, 0.f),
		glm::vec3(.2f, .2f, .2f));
	entity.Add<MovableComponent>();
	pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "planet.tga", TextureType::RGBA);
	pPhysics = entity.Add<PhysicsComponent>();
	pPhysics->addSphere(1.0f);
	pPhysics->setRotation(btVector3(0, 0, 1));
	pPhysics->setMovement(btVector3(1, 1, 0));


	//Left barrier(Preventing objects from going out of camera view)
	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(-3.7f, -1.f, 0.f),
		glm::vec3(.1f, 5.5f, .5f));
	entity.Add<MovableComponent>();
	pPhysics = entity.Add<PhysicsComponent>();
	pPhysics->addBox(0.0f);
	pPhysics->setRotation(btVector3(0, 0, 1));
	pPhysics->setMovement(btVector3(1, 1, 0));

	//Right barrier(Preventing objects from going out of camera view)
	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(3.7f, -1.f, 0.f),
		glm::vec3(.1f, 5.5f, .5f));
	entity.Add<MovableComponent>();
	pPhysics = entity.Add<PhysicsComponent>();
	pPhysics->addBox(0.0f);
	pPhysics->setRotation(btVector3(0, 0, 1));
	pPhysics->setMovement(btVector3(1, 1, 0));


	// Create fade screen entity
	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(0, 0, 1));
	pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(PAUSE_BACKGROUND_PATH, TextureType::RGBA);
	entity.Add<MaterialComponent>()->SetOpacity(0.f);
	Action_PauseGame::SetFadeScreen(entity);

	m_pSystemObserver->AddEvent(EGameEvent(EGE_TOGGLE_HITBOX_VIEW), new Action_ToggleHitboxView());

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

	// Create the system observer
	m_pSystemObserver = EntityManager::CreateEntity().Add<ObserverComponent>();
	m_pSystemObserver->Subscribe(*m_pInputSystem);
	m_pSystemObserver->AddEvent(EGE_PAUSE, new Action_PauseGame(m_pWindow));
}
