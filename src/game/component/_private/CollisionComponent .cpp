#include "MovableComponent.h"
#include "CollisionComponent.h"
#include "DrawComponent.h"

#include "EntityManager.h"
#include "MaterialComponent.h"


CollisionComponent::CollisionComponent(Entity entity) :
	IComponent(entity),  m_pMover(nullptr), m_pDraw(nullptr)
{
	isCollisionViewEnabled = true;
}

void CollisionComponent::Refresh()
{
	m_pMover = EntityManager::GetComponent<MovableComponent>(m_Entity);
	m_pDraw = EntityManager::GetComponent<DrawComponent>(m_Entity);
}

void CollisionComponent::Tick(deltaTime_t dt)
{
}

bool CollisionComponent::init(const glm::vec3& hitboxPosition, const glm::vec3& hitboxScale)
{
	m_HitboxPosition = hitboxPosition;
	m_HitboxScale = hitboxScale;

	return true;
}


glm::vec3 &  CollisionComponent::getHitboxPosition()
{
	return m_HitboxPosition;
}

glm::vec3 &  CollisionComponent::getHitboxScale()
{
	return m_HitboxScale;
}

void CollisionComponent::createEntities()
{
	entity = EntityManager::CreateEntity();
	entity.Add<TransformComponent>()->Init(glm::vec3(m_HitboxPosition), glm::vec3(m_HitboxScale));
	entity.Add<MovableComponent>();
	entity.Add<MaterialComponent>()->SetOpacity(0.5f);
	DrawComponent *pDraw = entity.Add<DrawComponent>();
	pDraw->SetGeometry(ShapeType::PLANE);
	pDraw->SetTexture(TEXTURE_PATH + "green.tga", TextureType::RGB);
	
	toggleHitboxView();
}

void CollisionComponent::moveBox(glm::vec3 moveAmmount)
{
	MovableComponent *moveBoxPlease = EntityManager::GetComponent<MovableComponent>(entity);
	moveBoxPlease -> Move(moveAmmount);
	m_HitboxPosition += moveAmmount;
}


void CollisionComponent::toggleHitboxView()
{
	MovableComponent * move = EntityManager::GetComponent<MovableComponent>(entity);
	glm::vec3 vec;
	if (isCollisionViewEnabled == true)
	{
		vec = { 100, 100, 100 };
		isCollisionViewEnabled = false;
	}
	else
	{
		vec = { -100, -100, -100 };
		isCollisionViewEnabled = true;
	}
	
	move->Move(vec);
}