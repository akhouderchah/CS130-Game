#include "SoundSystem.h"


SoundSystem::SoundSystem() :
	m_pSoundComponent(EntityManager::GetAll<SoundComponent>())
{
}

bool SoundSystem::Initialize()
{
	//initialize the OpenAl
	m_pDevice = alcOpenDevice(NULL);
	if (!m_pDevice)
	{
		return false;
	}
	m_pContext = alcCreateContext(m_pDevice, NULL);
	alcMakeContextCurrent(m_pContext);
	if (!m_pContext)
	{
		return false;
	}

	return true;
}

void SoundSystem::Shutdown()
{
	alcMakeContextCurrent(NULL);
	alcDestroyContext(m_pContext);
	alcCloseDevice(m_pDevice);
}

void SoundSystem::Tick(deltaTime_t dt)
{
	//@TODO add a way to update source and camera positions
	for (size_t i = 1; i < m_pSoundComponent.size(); ++i)
	{
		m_pSoundComponent[i]->Tick(dt);
		m_pSoundComponent[i]->UpdatePositions();
	}
}