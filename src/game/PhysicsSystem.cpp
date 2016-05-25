#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() :
	m_pPhysicsComponents(ComponentManager<PhysicsComponent>::GetAll())
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
	for(size_t i = 0; i < m_pPhysicsComponents.size(); ++i)
	{
		m_pPhysicsComponents[i].first->Tick(dt);
	}
}
