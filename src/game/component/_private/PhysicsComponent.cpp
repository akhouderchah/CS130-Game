#include "PhysicsComponent.h"
#include "MovableComponent.h"
#include "CollisionComponent.h"

float PhysicsComponent::s_Gravity = -6.45f;
float PhysicsComponent::s_ImpulseSpeed = 2.85f;
float PhysicsComponent::s_TerminalSpeed = 4.75f;

float PhysicsComponent::s_ImpulseWaitTime = 0.0f;


PhysicsComponent::PhysicsComponent(Entity entity) :
	IComponent(entity), m_IsGravityOn(true), m_Velocity(0, 0, 0),
	m_ImpulseWait(0.f), m_pMover(nullptr), m_pCollisionComp(EntityManager::GetAll<CollisionComponent>()),
	m_pColission(nullptr)
{
}

void PhysicsComponent::Refresh()
{
	m_pMover = EntityManager::GetComponent<MovableComponent>(m_Entity);
	m_pColission = EntityManager::GetComponent<CollisionComponent>(m_Entity);

	m_pCollisionComp = EntityManager::GetAll<CollisionComponent>();
}

void PhysicsComponent::Tick(deltaTime_t dt)
{
	m_ImpulseWait -= dt;

	float currentTop = m_pColission->getHitboxPosition()[1] + (m_pColission->getHitboxScale()[1]);
	float currentBottom = m_pColission->getHitboxPosition()[1] - (m_pColission->getHitboxScale()[1]);
	float currentLeft = m_pColission->getHitboxPosition()[0] - (m_pColission->getHitboxScale()[0]);
	float currentRight = m_pColission->getHitboxPosition()[0] + (m_pColission->getHitboxScale()[0]);
	

	bool collision = false;
	for (size_t i = 1; i < m_pCollisionComp.size(); ++i)
	{
		if (m_pCollisionComp[i]->GetID() != m_pColission->GetID())
		{
			float top = m_pCollisionComp[i]->getHitboxPosition()[1] + (m_pCollisionComp[i]->getHitboxScale()[1]);
			float bottom = m_pCollisionComp[i]->getHitboxPosition()[1] - (m_pCollisionComp[i]->getHitboxScale()[1]);
			float left = m_pCollisionComp[i]->getHitboxPosition()[0] - (m_pCollisionComp[i]->getHitboxScale()[0]);
			float right = m_pCollisionComp[i]->getHitboxPosition()[0] + (m_pCollisionComp[i]->getHitboxScale()[0]);

			if (currentBottom <= top && (currentRight <= right && currentLeft >= left))
			{
				collision = true;
			}
		}
	}

	if (collision == false || m_Velocity[1] >= 0)
	{
		if (m_Velocity[1] >= -s_TerminalSpeed)
		{
			m_Velocity[1] += m_IsGravityOn * s_Gravity * dt;
		}
		m_pMover->Move(m_Velocity * dt);
		m_pColission->moveBox(m_Velocity * dt);
	}
}

bool PhysicsComponent::Impulse()
{
	if (m_ImpulseWait > 0.f)
	{
		return false;
	}

	m_ImpulseWait = s_ImpulseWaitTime;
	m_Velocity[1] = s_ImpulseSpeed;

	return true;
}
