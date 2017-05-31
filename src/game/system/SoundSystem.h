#pragma once

#include "ISystem.h"
#include "SoundComponent.h"
#include "CameraComponent.h"

/**
 * @brief System to set up OpenAL and update sound positions
 */
class SoundSystem : public ISystem
{
public:
	SoundSystem();

	virtual bool Initialize();
	virtual void Shutdown();
	virtual void Tick(deltaTime_t dt);

	//@TODO add a way to update source and camera positions
private:

	ALCcontext *m_pContext;
	ALCdevice *m_pDevice;
	ConstVector<SoundComponent*> m_pSoundComponent;
};
