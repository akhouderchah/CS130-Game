#pragma once

#include "Base.h"
#include "IComponent.h"

class SoundComponent : public IComponent
{
public:
	SoundComponent(Entity entity);
	~SoundComponent();

	void Refresh();
	void Tick(deltaTime_t dt);
	float m_ImpulseWait;

	void PlaySound(std::string);
	void PauseSound(std::string);
	void StopSound(std::string);
	void LoadBuffers();

	std::string LoadSound(std::string, std::string, bool);

	void UpdatePositions();

private:
	static std::vector<SoundFileData> s_SoundFileData;
	static std::unordered_map<std::string, int> s_SourceToNameConnection;
	int m_NumberOfSounds;

	ALuint * m_pBuffers;
	ALuint * m_pSources;
	//orientation of the listener is described by two vectors, one pointing in the direction the listener
	//looks at and a second pointing up
	ALfloat m_SourcePos[3];
	ALfloat m_SourceVel[3];
	ALfloat m_ListenerPos[3];
	ALfloat m_ListenerVel[3];
	ALfloat m_ListenerOri[6];

	bool m_AreBuffersLoaded;
};

