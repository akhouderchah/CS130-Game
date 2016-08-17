#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() :
	m_pPhysicsComponents(EntityManager::GetAll<PhysicsComponent>())
{
}

bool PhysicsSystem::Initialize()
{
	return true;
}

void PhysicsSystem::Shutdown()
{
}

void PhysicsSystem::Tick(deltaTime_t dt)
{
	for(size_t i = 1; i < m_pPhysicsComponents.size(); ++i)
	{
		m_pPhysicsComponents[i]->Tick(dt);
	}
}

