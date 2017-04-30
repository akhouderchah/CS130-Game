#include "SoundComponent.h"

std::vector<SoundFileData> SoundComponent::s_SoundFileData;

std::unordered_map<std::string, int> SoundComponent::s_SourceToNameConnection;

SoundComponent::SoundComponent(Entity entity) : IComponent(entity), m_ImpulseWait(0.f)
{
	m_NumberOfSounds = 0;
	m_AreBuffersLoaded = false;

	m_SourcePos[0] = 0.0; m_SourcePos[1] = 0.0; m_SourcePos[2] = 0.0;
	m_SourceVel[0] = 0.0; m_SourceVel[1] = 0.0; m_SourceVel[2] = 0.0;
	m_ListenerPos[0] = 0.0; m_ListenerPos[1] = 0.0; m_ListenerPos[2] = 0.0;
	m_ListenerVel[0] = 0.0; m_ListenerVel[1] = 0.0; m_ListenerVel[2] = 0.0;
	m_ListenerOri[0] = 0.0; m_ListenerOri[1] = 0.0; m_ListenerOri[2] = -1.0;
	m_ListenerOri[3] = 0.0; m_ListenerOri[4] = 1.0; m_ListenerOri[5] = 0.0;
}

SoundComponent::~SoundComponent()
{
	alDeleteSources(m_NumberOfSounds, m_pSources);
	alDeleteBuffers(m_NumberOfSounds, m_pBuffers);
}


void SoundComponent::Refresh()
{
	//m_pMover = EntityManager::GetComponent<MovableComponent>(m_Entity);
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

		return "OK";
	}
	else
	{
		return soundFileData.errorCode;
	}
}

void SoundComponent::LoadBuffers()
{
	if (!m_AreBuffersLoaded)
	{
		m_AreBuffersLoaded = true;
		m_NumberOfSounds = static_cast<int>(s_SoundFileData.size());

		m_pBuffers = new ALuint[m_NumberOfSounds];
		m_pSources = new ALuint[m_NumberOfSounds];

		alGenBuffers(m_NumberOfSounds, m_pBuffers);
		alGenSources(m_NumberOfSounds, m_pSources);

		int i = 0;
		for (std::vector<SoundFileData>::iterator it = s_SoundFileData.begin(); it != s_SoundFileData.end(); ++it)
		{
			s_SourceToNameConnection.emplace(it->name, i);

			alBufferData(m_pBuffers[i], it->format, it->buf, it->dataSize, it->frequency);

			if (i == 0)
			{
				//listener
				alListenerfv(AL_POSITION, m_ListenerPos);
				alListenerfv(AL_VELOCITY, m_ListenerVel);
				alListenerfv(AL_ORIENTATION, m_ListenerOri);
			}

			//Source
			alSourcei(m_pSources[i], AL_PITCH, 1.0f);
			alSourcei(m_pSources[i], AL_GAIN, 1.0f);
			alSourcefv(m_pSources[i], AL_POSITION, m_SourcePos);
			alSourcefv(m_pSources[i], AL_VELOCITY, m_SourceVel);
			alSourcei(m_pSources[i], AL_BUFFER, m_pBuffers[i]);

			if (it->isLoop)
			{
				alSourcei(m_pSources[i], AL_LOOPING, AL_TRUE);
			}
			else
			{
				alSourcei(m_pSources[i], AL_LOOPING, AL_FALSE);
			}

			i++;
		}

	}

}

void SoundComponent::PlaySound(std::string name)
{
	if (!m_AreBuffersLoaded)
	{
		LoadBuffers();
	}

	std::unordered_map<std::string, int>::const_iterator iter = s_SourceToNameConnection.find(name);

	if (iter == s_SourceToNameConnection.end())
	{
		;;
	}
	else
	{
		alSourcePlay(m_pSources[iter->second]);
	}
	
}
void SoundComponent::PauseSound(std::string name)
{
	if (!m_AreBuffersLoaded)
	{
		LoadBuffers();
	}

	std::unordered_map<std::string, int>::const_iterator iter = s_SourceToNameConnection.find(name);

	if (iter == s_SourceToNameConnection.end())
	{
		;;
	}
	else
	{
		alSourcePause(m_pSources[iter->second]);
	}
}
void SoundComponent::StopSound(std::string name)
{
	if (!m_AreBuffersLoaded)
	{
		LoadBuffers();
	}

	std::unordered_map<std::string, int>::const_iterator iter = s_SourceToNameConnection.find(name);

	if (iter == s_SourceToNameConnection.end())
	{
		;;
	}
	else
	{
		alSourceStop(m_pSources[iter->second]);
	}
}

void SoundComponent::UpdatePositions()
{

}