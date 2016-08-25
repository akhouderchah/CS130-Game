#pragma once

#include "ISystem.h"
#include "PhysicsComponent.h"

class PhysicsSystem : public ISystem
{
public:
	PhysicsSystem();

	virtual bool Initialize();
	virtual void Shutdown();

	virtual void Tick(deltaTime_t dt);

private:
	ConstVector<PhysicsComponent*> m_pPhysicsComponents;
};
