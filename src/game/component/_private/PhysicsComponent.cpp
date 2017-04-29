#include "PhysicsComponent.h"
#include "MovableComponent.h"

float PhysicsComponent::s_Gravity = -6.45f;
float PhysicsComponent::s_ImpulseSpeed = 2.85f;
float PhysicsComponent::s_TerminalSpeed = 4.75f;

float PhysicsComponent::s_ImpulseWaitTime = 0.0f;

PhysicsComponent::PhysicsComponent(Entity entity) :
	IComponent(entity), m_IsGravityOn(true), m_Velocity(0,0,0),
	m_ImpulseWait(0.f), m_pMover(nullptr), sound(ResourceManager::returnSound())
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
		m_Velocity[1] += m_IsGravityOn * s_Gravity * dt;
	}
	m_pMover->Move(m_Velocity * dt);
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
