#include "SoundComponent.h"



SoundComponent::SoundComponent(Entity entity) : IComponent(entity), m_ImpulseWait(0.f)
{

}


void SoundComponent::Refresh()
{
	//m_pMover = EntityManager::GetComponent<MovableComponent>(m_Entity);
}

void SoundComponent::Tick(deltaTime_t dt)
{
	m_ImpulseWait -= dt;
}