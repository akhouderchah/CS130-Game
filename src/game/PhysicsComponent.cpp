#include "PhysicsComponent.h"
#include "MovableComponent.h"

float PhysicsComponent::s_Gravity = -0.025f;

PhysicsComponent::PhysicsComponent(MovableComponent& mover) :
	m_Velocity(0,0,0), m_pMover(&mover)
{
}

void PhysicsComponent::Tick(deltaTime_t dt)
{
	m_Velocity[1] += s_Gravity * dt;
	m_pMover->Move(m_Velocity);
}

bool PhysicsComponent::Impulse()
{
	m_Velocity[1] = 0.2f;
	return true;
}
