#include "SoundComponent.h"
#include "TransformComponent.h"


SoundComponent::SoundComponent(Entity entity) : IComponent(entity), m_ImpulseWait(0.f), m_pTransformComp(nullptr)
{
	m_SourcePos[0] = 0.0; m_SourcePos[1] = 0.0; m_SourcePos[2] = 0.0;
	m_SourceVel[0] = 0.0; m_SourceVel[1] = 0.0; m_SourceVel[2] = 0.0;
	m_ListenerPos[0] = 0.0; m_ListenerPos[1] = 0.0; m_ListenerPos[2] = 0.0;
	m_ListenerVel[0] = 0.0; m_ListenerVel[1] = 0.0; m_ListenerVel[2] = 0.0;
	m_ListenerOri[0] = 0.0; m_ListenerOri[1] = 0.0; m_ListenerOri[2] = -1.0;
	m_ListenerOri[3] = 0.0; m_ListenerOri[4] = 1.0; m_ListenerOri[5] = 0.0;
}

SoundComponent::~SoundComponent()
{
	for (unsigned int i = 0; i < m_Sources.size(); i++)
	{
		alDeleteSources(1, &m_Sources[i]);
		alDeleteBuffers(1, &m_Buffers[i]);
	}
}

void SoundComponent::Refresh()
{
	m_pTransformComp = EntityManager::GetComponent<TransformComponent>(m_Entity);
}

void SoundComponent::Tick(deltaTime_t dt)
{
	m_ImpulseWait -= dt;
}

std::string SoundComponent::LoadSound(std::string name, std::string filePath, bool isLoop)
{
	//@TODO make sure there is no memory leak here
	SoundFileData soundFileData = ResourceManager::LoadSound(name, filePath, isLoop);
	if (soundFileData.errorCode == "OK")
	{
		s_SourceToNameConnection.emplace(name, s_SoundFileData.size());
		s_SoundFileData.push_back(soundFileData);

		ALuint tempBuffer;
		ALuint tempSource;

		alGenBuffers(1, &tempBuffer);
		alGenSources(1, &tempSource);

		s_SourceToNameConnection.emplace(soundFileData.name, m_Sources.size());

		alBufferData(tempBuffer, soundFileData.format, soundFileData.buf, soundFileData.dataSize, soundFileData.frequency);

		//listener
		alListenerfv(AL_POSITION, m_ListenerPos);
		alListenerfv(AL_VELOCITY, m_ListenerVel);
		alListenerfv(AL_ORIENTATION, m_ListenerOri);

		//Source
		alSourcei(tempSource, AL_PITCH, 1);
		alSourcei(tempSource, AL_GAIN, 1);

		alSourcefv(tempSource, AL_POSITION, m_SourcePos);
		alSourcefv(tempSource, AL_VELOCITY, m_SourceVel);
		alSourcei(tempSource, AL_BUFFER, tempBuffer);

		if (soundFileData.isLoop)
		{
			alSourcei(tempSource, AL_LOOPING, AL_TRUE);
		}
		else
		{
			alSourcei(tempSource, AL_LOOPING, AL_FALSE);
		}

		m_Buffers.push_back(tempBuffer);
		m_Sources.push_back(tempSource);

		return "OK";
	}
	else
	{
		return soundFileData.errorCode;
	}
}

void SoundComponent::PlaySound(std::string name)
{
	std::unordered_map<std::string, int>::const_iterator iter = s_SourceToNameConnection.find(name);

	if (iter == s_SourceToNameConnection.end())
	{
		;;
	}
	else
	{
		alSourcePlay(m_Sources[iter->second]);
	}
}

void SoundComponent::PauseSound(std::string name)
{
	std::unordered_map<std::string, int>::const_iterator iter = s_SourceToNameConnection.find(name);

	if (iter == s_SourceToNameConnection.end())
	{
		;;
	}
	else
	{
		alSourcePause(m_Sources[iter->second]);
	}
}

void SoundComponent::StopSound(std::string name)
{
	std::unordered_map<std::string, int>::const_iterator iter = s_SourceToNameConnection.find(name);

	if (iter == s_SourceToNameConnection.end())
	{
		;;
	}
	else
	{
		alSourceStop(m_Sources[iter->second]);
	}
}

TransformComponent *SoundComponent::getTransformComponent()
{
	return m_pTransformComp;
}


std::vector<ALuint> & SoundComponent::getSource()
{
	return m_Sources;
}