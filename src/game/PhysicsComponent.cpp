#include "PhysicsComponent.h"
#include "MovableComponent.h"

float PhysicsComponent::s_Gravity = -0.175f;
float PhysicsComponent::s_ImpulseSpeed = 0.05f;
float PhysicsComponent::s_TerminalSpeed = 0.09f;

float PhysicsComponent::s_ImpulseWaitTime = 0.0f;

PhysicsComponent::PhysicsComponent(Entity entity) :
	IComponent(entity), m_Velocity(0,0,0), m_ImpulseWait(0.f),
	m_pMover(nullptr)
{
}

void PhysicsComponent::Refresh()
{
	m_pMover = EntityManager::GetComponent<MovableComponent>(m_Entity);
}

void PhysicsComponent::Tick(deltaTime_t dt)
{
	m_ImpulseWait -= dt;
	
	if(m_Velocity[1] >= -s_TerminalSpeed)
	{
		m_Velocity[1] += s_Gravity * dt;
	}
	m_pMover->Move(m_Velocity);
}

bool PhysicsComponent::Impulse()
{
	if(m_ImpulseWait > 0.f)
	{
		return false;
	}

	m_ImpulseWait = s_ImpulseWaitTime;
	m_Velocity[1] = s_ImpulseSpeed;
	return true;
}
