#pragma once

#include "ISystem.h"
#include "PhysicsComponent.h"
#include "btBulletDynamicsCommon.h"


/**
 * @brief System to perform physics simulations on relevant components
 *
 * The main relevant components are:
 * * Physics Component - for rigid bodies
 *
 * This system will do the necessary calculations involved to make the above components
 * act as they should. The system uses Bullet Physics API to integrate physical functionality
 * such as gravity, friction, collision, ect.
 *
 */

class TransformComponent;

class PhysicsSystem : public ISystem
{
public:
	PhysicsSystem();

	virtual bool Initialize();
	virtual void Shutdown();

	virtual void Tick(deltaTime_t dt);


	static btDynamicsWorld * GetWorld() { return m_pWorld; }

private:
	ConstVector<PhysicsComponent*> m_pPhysicsComponents;

	static btBroadphaseInterface * m_pBroadphase;
	static btCollisionConfiguration * m_pCollisionConfig;
	static btDispatcher * m_pDispatcher;
	static btConstraintSolver * m_pSolver;
	static btDynamicsWorld * m_pWorld;

	static bool s_IsWorldInitialized;

	void updatePlane(bulletObject *);
	void updateSphere(bulletObject *, Entity );
	void updateBox(bulletObject *, Entity );
	void updateCylinder(bulletObject *, Entity );
};


