#pragma once

#include "Base.h"
#include "IComponent.h"

class SoundComponent : public IComponent
{
public:
	SoundComponent(Entity entity);

	void Refresh();
	void Tick(deltaTime_t dt);
	float m_ImpulseWait;
	
};

