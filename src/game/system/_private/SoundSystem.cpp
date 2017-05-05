#include "SoundSystem.h"
#include "glm/vec3.hpp"
#include <glm/gtc/type_ptr.hpp>

SoundSystem::SoundSystem() :
	m_pSoundComponent(EntityManager::GetAll<SoundComponent>()), m_pTransformComp(nullptr)
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
	for (size_t i = 1; i < m_pSoundComponent.size(); ++i)
	{
		m_pSoundComponent[i]->Tick(dt);


		//Updates sound location based on its TransformComponent position
		m_pTransformComp = m_pSoundComponent[i]->getTransformComponent();

		std::vector<ALuint> sources = m_pSoundComponent[i]->getSource();

		for (unsigned int j = 0; j < sources.size(); j++)
		{
			alSourcefv(sources[j], AL_POSITION, glm::value_ptr(m_pTransformComp->GetPosition()));
		}
	}
}