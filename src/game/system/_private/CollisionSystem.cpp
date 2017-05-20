#include "CollisionSystem.h"

CollisionSystem::CollisionSystem() :
	m_pCollisionComponent(EntityManager::GetAll<CollisionComponent>())
{
}

bool CollisionSystem::Initialize()
{
	return true;
}

void CollisionSystem::Shutdown()
{
}

void CollisionSystem::Tick(deltaTime_t dt)
{
	for (size_t i = 1; i < m_pCollisionComponent.size(); ++i)
	{
		m_pCollisionComponent[i]->Tick(dt);
	}
}