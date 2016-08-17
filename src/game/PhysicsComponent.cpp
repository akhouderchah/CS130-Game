#include "PhysicsComponent.h"
#include "MovableComponent.h"

float PhysicsComponent::s_Gravity = -0.35f;
float PhysicsComponent::s_TerminalSpeed = 0.1f;

PhysicsComponent::PhysicsComponent(Entity entity) :
	IComponent(entity), m_Velocity(0,0,0), m_pMover(nullptr)
{
}

void PhysicsComponent::Refresh()
{
	m_pMover = EntityManager::GetComponent<MovableComponent>(m_Entity);
}

void PhysicsComponent::Tick(deltaTime_t dt)
{
	if(m_Velocity[1] >= -s_TerminalSpeed)
	{
		m_Velocity[1] += s_Gravity * dt;
	}
	m_pMover->Move(m_Velocity);
}

bool PhysicsComponent::Impulse()
{
	m_Velocity[1] = 0.09f;
	return true;
}
