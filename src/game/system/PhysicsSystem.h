#pragma once

#include "ISystem.h"
#include "PhysicsComponent.h"


/**
 * @brief System to perform physics simulations on relevant components
 *
 * The main relevant components are:
 * * Physics Component - for rigid bodies
 * * Collision Component - for objects that will block entities with physics components
 *
 * This system will do the necessary calculations involved to make the above components
 * act as they should.
 *
 * @TODO Use some sort of space partitioning so that the work involved for this system is
 * decreased (and also more easily parallelizable).
 */
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
