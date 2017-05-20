#pragma once

#include "ISystem.h"
#include "CollisionComponent.h"

class CollisionSystem : public ISystem
{
public:
	CollisionSystem();

	virtual bool Initialize();
	virtual void Shutdown();
	virtual void Tick(deltaTime_t dt);

private:
	ConstVector<CollisionComponent*> m_pCollisionComponent;
};