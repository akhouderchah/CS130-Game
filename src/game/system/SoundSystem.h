#pragma once

#include "ISystem.h"
#include "SoundComponent.h"

class SoundSystem : public ISystem
{
public:
	SoundSystem();

	virtual bool Initialize();
	virtual void Shutdown();

	virtual void Tick(deltaTime_t dt);

private:
	ConstVector<SoundComponent*> m_pSoundComponent;
};