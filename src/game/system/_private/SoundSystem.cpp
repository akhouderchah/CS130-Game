#include "SoundSystem.h"


SoundSystem::SoundSystem() :
	m_pSoundComponent(EntityManager::GetAll<SoundComponent>())
{
}


bool SoundSystem::Initialize()
{
	return true;
}

void SoundSystem::Shutdown()
{
}

void SoundSystem::Tick(deltaTime_t dt)
{
	for (size_t i = 1; i < m_pSoundComponent.size(); ++i)
	{
		m_pSoundComponent[i]->Tick(dt);
	}
}